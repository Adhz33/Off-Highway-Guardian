/*
 * Off-Highway Guardian - Rescue Receiver Unit
 * RF beacon receiver with RSSI homing capability
 * 
 * Hardware: ESP32 / Arduino + HC-12 RF Module + 16x2 LCD
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "config.h"
#include "../common/protocol.h"

// LCD setup
LiquidCrystal_I2C lcd(LCD_I2C_ADDRESS, LCD_COLS, LCD_ROWS);

// State
bool alertActive = false;
EmergencyPacket lastPacket;
unsigned long lastPacketTime = 0;
int rssiValue = 0;

// Alert types as strings
const char* alertTypeStr[] = {
    "UNKNOWN",
    "IMPACT",
    "ROLLOVER",
    "SPIN",
    "COMBINED"
};

void setup() {
    Serial.begin(SERIAL_BAUD);
    Serial2.begin(RF_BAUD, SERIAL_8N1, RF_RX_PIN, RF_TX_PIN); // RF module
    
    // Initialize pins
    pinMode(LED_SIGNAL, OUTPUT);
    pinMode(LED_ALERT, OUTPUT);
    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(RSSI_LED_1, OUTPUT);
    pinMode(RSSI_LED_2, OUTPUT);
    pinMode(RSSI_LED_3, OUTPUT);
    pinMode(RSSI_LED_4, OUTPUT);
    pinMode(RSSI_LED_5, OUTPUT);
    
    // Initialize LCD
    lcd.init();
    lcd.backlight();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Off-Highway");
    lcd.setCursor(0, 1);
    lcd.print("Guardian RX");
    delay(2000);
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Monitoring...");
    
    Serial.println("Off-Highway Guardian - Receiver Unit Ready");
}

void loop() {
    // Check for incoming RF packets
    if (Serial2.available() >= sizeof(EmergencyPacket)) {
        receivePacket();
    }
    
    // Update display and indicators
    updateDisplay();
    
    // Timeout alert after configured duration
    if (alertActive && (millis() - lastPacketTime > ALERT_TIMEOUT_MS)) {
        clearAlert();
    }
    
    delay(DISPLAY_UPDATE_INTERVAL_MS);
}

void receivePacket() {
    EmergencyPacket packet;
    Serial2.readBytes((uint8_t*)&packet, sizeof(EmergencyPacket));
    
    // Verify packet
    if (packet.version != PROTOCOL_VERSION) {
        Serial.println("Invalid protocol version");
        return;
    }
    
    if (!verifyChecksum(&packet)) {
        Serial.println("Checksum failed");
        return;
    }
    
    // Valid packet received
    digitalWrite(LED_SIGNAL, HIGH);
    lastPacket = packet;
    lastPacketTime = millis();
    
    if (!alertActive) {
        alertActive = true;
        activateAlert();
    }
    
    // Read RSSI (if supported by RF module)
    rssiValue = readRSSI();
    updateRSSIIndicator(rssiValue);
    
    Serial.print("Packet received - Vehicle: ");
    printVehicleId(packet.vehicleId);
    Serial.print(" Type: ");
    Serial.println(getAlertTypeString(packet.alertType));
    
    digitalWrite(LED_SIGNAL, LOW);
}

void activateAlert() {
    // Sound buzzer
    for (int i = 0; i < BUZZER_BEEP_COUNT; i++) {
        digitalWrite(BUZZER_PIN, HIGH);
        delay(BUZZER_BEEP_DURATION_MS);
        digitalWrite(BUZZER_PIN, LOW);
        delay(BUZZER_BEEP_DURATION_MS);
    }
    
    digitalWrite(LED_ALERT, HIGH);
    
    Serial.println("ALERT ACTIVATED");
}

void clearAlert() {
    alertActive = false;
    digitalWrite(LED_ALERT, LOW);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Monitoring...");
    Serial.println("Alert cleared");
}

void updateDisplay() {
    if (!alertActive) return;
    
    lcd.clear();
    
    // Line 1: Vehicle ID and alert type
    lcd.setCursor(0, 0);
    lcd.print("V:");
    printVehicleIdLCD(lastPacket.vehicleId);
    lcd.print(" ");
    lcd.print(getAlertTypeString(lastPacket.alertType));
    
    // Line 2: RSSI for homing
    lcd.setCursor(0, 1);
    lcd.print("RSSI:");
    lcd.print(rssiValue);
    lcd.print(" ");
    
    // Signal strength bars
    int bars = map(rssiValue, -120, -30, 0, 5);
    for (int i = 0; i < bars && i < 5; i++) {
        lcd.print((char)0xFF); // Full block character
    }
}

void updateRSSIIndicator(int rssi) {
    // Map RSSI to LED bar (5 LEDs)
    int level = map(rssi, RSSI_MIN, RSSI_MAX, 0, 5);
    level = constrain(level, 0, 5);
    
    digitalWrite(RSSI_LED_1, level >= 1 ? HIGH : LOW);
    digitalWrite(RSSI_LED_2, level >= 2 ? HIGH : LOW);
    digitalWrite(RSSI_LED_3, level >= 3 ? HIGH : LOW);
    digitalWrite(RSSI_LED_4, level >= 4 ? HIGH : LOW);
    digitalWrite(RSSI_LED_5, level >= 5 ? HIGH : LOW);
}

int readRSSI() {
    // For HC-12: Send AT+RX command to read RSSI
    // This is a simplified version - actual implementation depends on RF module
    
    #if RSSI_SIMULATED
    // Simulated RSSI based on packet reception quality
    // In real implementation, query the RF module
    static int simulatedRSSI = -80;
    simulatedRSSI += random(-10, 10);
    simulatedRSSI = constrain(simulatedRSSI, RSSI_MIN, RSSI_MAX);
    return simulatedRSSI;
    #else
    // Real RSSI implementation for HC-12
    // Uncomment and modify for your RF module
    /*
    Serial2.print("AT+RX");
    delay(100);
    if (Serial2.available()) {
        String response = Serial2.readString();
        int rssiStart = response.indexOf(":-") + 2;
        String rssiStr = response.substring(rssiStart);
        return -rssiStr.toInt();
    }
    */
    return -100; // Default if no response
    #endif
}

const char* getAlertTypeString(uint8_t type) {
    if (type >= 1 && type <= 4) {
        return alertTypeStr[type];
    }
    return alertTypeStr[0];
}

void printVehicleId(const uint8_t* id) {
    for (int i = 0; i < 4; i++) {
        if (id[i] < 0x10) Serial.print("0");
        Serial.print(id[i], HEX);
    }
}

void printVehicleIdLCD(const uint8_t* id) {
    #if SHOW_FULL_VEHICLE_ID
    // Print full vehicle ID (4 bytes)
    for (int i = 0; i < 4; i++) {
        if (id[i] < 0x10) lcd.print("0");
        lcd.print(id[i], HEX);
    }
    #else
    // Print shortened vehicle ID (last 2 bytes)
    for (int i = 2; i < 4; i++) {
        if (id[i] < 0x10) lcd.print("0");
        lcd.print(id[i], HEX);
    }
    #endif
}
