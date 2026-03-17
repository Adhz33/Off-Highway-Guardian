/*
 * Off-Highway Guardian - Vehicle Unit
 * Autonomous accident detection and RF beacon system
 * 
 * Hardware: ESP32 / Arduino + MPU-6050 + HC-12 RF Module
 */

#include <Wire.h>
#include "MPU6050.h"
#include "config.h"
#include "../common/protocol.h"

// Global state
MPU6050 mpu;
bool systemActive = false;
bool accidentDetected = false;
bool beaconActive = false;
unsigned long lastBeaconTime = 0;
unsigned long buttonPressStart = 0;

// Detection state
struct DetectionState {
    bool impactDetected;
    bool rolloverDetected;
    bool spinDetected;
    unsigned long detectionTime;
} detectionState;

void setup() {
    Serial.begin(SERIAL_BAUD);
    Serial2.begin(RF_BAUD, SERIAL_8N1, RF_RX_PIN, RF_TX_PIN); // RF module
    
    // Initialize pins
    pinMode(LED_SYSTEM_ACTIVE, OUTPUT);
    pinMode(LED_ACCIDENT_DETECTED, OUTPUT);
    pinMode(LED_BEACON_ACTIVE, OUTPUT);
    pinMode(BUTTON_OVERRIDE, INPUT_PULLUP);
    
    // Initialize I2C and MPU6050
    Wire.begin();
    mpu.initialize();
    
    if (mpu.testConnection()) {
        Serial.println("MPU6050 connected");
        systemActive = true;
        digitalWrite(LED_SYSTEM_ACTIVE, HIGH);
    } else {
        Serial.println("MPU6050 connection failed!");
        while(1) {
            digitalWrite(LED_SYSTEM_ACTIVE, !digitalRead(LED_SYSTEM_ACTIVE));
            delay(200);
        }
    }
    
    // Configure MPU6050
    mpu.setFullScaleAccelRange(ACCEL_RANGE);
    mpu.setFullScaleGyroRange(GYRO_RANGE);
    
    resetDetectionState();
    Serial.println("Off-Highway Guardian - Vehicle Unit Ready");
}

void loop() {
    // Check for manual override
    checkManualOverride();
    
    if (!systemActive) return;
    
    // Read IMU data at ~50Hz
    int16_t ax, ay, az, gx, gy, gz;
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    
    // Convert to physical units
    float accelX = ax / 4096.0; // g
    float accelY = ay / 4096.0;
    float accelZ = az / 4096.0;
    float gyroX = gx / 32.8;    // degrees/sec
    float gyroY = gy / 32.8;
    float gyroZ = gz / 32.8;
    
    // Run accident detection
    if (!accidentDetected) {
        detectAccident(accelX, accelY, accelZ, gyroX, gyroY, gyroZ);
    }
    
    // Handle beacon transmission
    if (beaconActive && (millis() - lastBeaconTime >= BEACON_INTERVAL_MS)) {
        transmitBeacon();
        lastBeaconTime = millis();
    }
    
    delay(IMU_SAMPLE_INTERVAL_MS); // Configured sampling rate
}

void detectAccident(float ax, float ay, float az, float gx, float gy, float gz) {
    unsigned long now = millis();
    bool conditionMet = false;
    
    // 1. Impact detection - sudden acceleration change
    float accelMagnitude = sqrt(ax*ax + ay*ay + az*az);
    float accelDelta = abs(accelMagnitude - 1.0); // Delta from 1g (gravity)
    
    if (accelDelta >= IMPACT_THRESHOLD_G) {
        if (!detectionState.impactDetected) {
            detectionState.impactDetected = true;
            detectionState.detectionTime = now;
            Serial.println("Impact detected!");
        }
        conditionMet = true;
    }
    
    // 2. Rollover detection - tilt angle
    float pitch = atan2(ay, sqrt(ax*ax + az*az)) * 180.0 / PI;
    float roll = atan2(ax, sqrt(ay*ay + az*az)) * 180.0 / PI;
    
    if (abs(pitch) >= ROLLOVER_ANGLE_DEG || abs(roll) >= ROLLOVER_ANGLE_DEG) {
        if (!detectionState.rolloverDetected) {
            detectionState.rolloverDetected = true;
            detectionState.detectionTime = now;
            Serial.println("Rollover detected!");
        }
        conditionMet = true;
    }
    
    // 3. Spin detection - high angular velocity
    float gyroMagnitude = sqrt(gx*gx + gy*gy + gz*gz);
    
    if (gyroMagnitude >= SPIN_THRESHOLD_DPS) {
        if (!detectionState.spinDetected) {
            detectionState.spinDetected = true;
            detectionState.detectionTime = now;
            Serial.println("Spin detected!");
        }
        conditionMet = true;
    }
    
    // Multi-condition confirmation
    if (conditionMet) {
        int conditionsCount = 0;
        if (detectionState.impactDetected) conditionsCount++;
        if (detectionState.rolloverDetected) conditionsCount++;
        if (detectionState.spinDetected) conditionsCount++;
        
        // Require at least 2 of 3 conditions within confirmation window
        if (conditionsCount >= 2 && (now - detectionState.detectionTime <= CONFIRMATION_WINDOW_MS)) {
            confirmAccident();
        }
    }
    
    // Reset detection state if window expired
    if (now - detectionState.detectionTime > CONFIRMATION_WINDOW_MS) {
        resetDetectionState();
    }
}

void confirmAccident() {
    accidentDetected = true;
    beaconActive = true;
    digitalWrite(LED_ACCIDENT_DETECTED, HIGH);
    digitalWrite(LED_BEACON_ACTIVE, HIGH);
    
    Serial.println("ACCIDENT CONFIRMED - ACTIVATING BEACON");
    
    // Immediate first transmission
    transmitBeacon();
    lastBeaconTime = millis();
}

void transmitBeacon() {
    EmergencyPacket packet;
    packet.version = PROTOCOL_VERSION;
    memcpy(packet.vehicleId, VEHICLE_ID, 4);
    
    // Determine alert type
    if (detectionState.impactDetected && detectionState.rolloverDetected) {
        packet.alertType = ALERT_TYPE_COMBINED;
    } else if (detectionState.rolloverDetected) {
        packet.alertType = ALERT_TYPE_ROLLOVER;
    } else if (detectionState.spinDetected) {
        packet.alertType = ALERT_TYPE_SPIN;
    } else {
        packet.alertType = ALERT_TYPE_IMPACT;
    }
    
    packet.timestamp = millis();
    packet.checksum = calculateChecksum(&packet);
    
    // Transmit via RF module
    Serial2.write((uint8_t*)&packet, sizeof(EmergencyPacket));
    
    Serial.print("Beacon transmitted - Type: 0x");
    Serial.println(packet.alertType, HEX);
    
    // Blink beacon LED
    digitalWrite(LED_BEACON_ACTIVE, LOW);
    delay(50);
    digitalWrite(LED_BEACON_ACTIVE, HIGH);
}

void checkManualOverride() {
    if (digitalRead(BUTTON_OVERRIDE) == LOW) {
        if (buttonPressStart == 0) {
            buttonPressStart = millis();
        } else if (millis() - buttonPressStart >= OVERRIDE_HOLD_TIME_MS) {
            // Cancel beacon
            if (beaconActive) {
                beaconActive = false;
                accidentDetected = false;
                digitalWrite(LED_ACCIDENT_DETECTED, LOW);
                digitalWrite(LED_BEACON_ACTIVE, LOW);
                resetDetectionState();
                Serial.println("Beacon cancelled by manual override");
            }
            buttonPressStart = 0;
        }
    } else {
        buttonPressStart = 0;
    }
}

void resetDetectionState() {
    detectionState.impactDetected = false;
    detectionState.rolloverDetected = false;
    detectionState.spinDetected = false;
    detectionState.detectionTime = 0;
}
