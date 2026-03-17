/*
 * Off-Highway Guardian - Calibration Tool
 * Use this to calibrate IMU thresholds for your specific vehicle
 * 
 * Upload this to vehicle unit and follow on-screen instructions
 */

#include <Wire.h>
#include "../vehicle-unit/MPU6050.h"

MPU6050 mpu;

// Calibration data storage
struct CalibrationData {
    float maxAccelX, maxAccelY, maxAccelZ;
    float maxGyroX, maxGyroY, maxGyroZ;
    float minAccelX, minAccelY, minAccelZ;
    float minGyroX, minGyroY, minGyroZ;
} calData;

void setup() {
    Serial.begin(115200);
    while (!Serial) delay(10);
    
    Serial.println("\n\n=================================");
    Serial.println("Off-Highway Guardian");
    Serial.println("IMU Calibration Tool");
    Serial.println("=================================\n");
    
    Wire.begin();
    mpu.initialize();
    
    if (!mpu.testConnection()) {
        Serial.println("ERROR: MPU6050 not found!");
        while(1) delay(1000);
    }
    
    mpu.setFullScaleAccelRange(MPU6050_ACCEL_FS_8);
    mpu.setFullScaleGyroRange(MPU6050_GYRO_FS_1000);
    
    Serial.println("MPU6050 initialized successfully\n");
    
    resetCalibration();
    showMenu();
}

void loop() {
    if (Serial.available()) {
        char cmd = Serial.read();
        handleCommand(cmd);
    }
    
    delay(100);
}

void handleCommand(char cmd) {
    switch(cmd) {
        case '1':
            calibrateStationary();
            break;
        case '2':
            calibrateNormalDriving();
            break;
        case '3':
            testImpact();
            break;
        case '4':
            testRollover();
            break;
        case '5':
            testSpin();
            break;
        case '6':
            showResults();
            break;
        case '7':
            resetCalibration();
            Serial.println("Calibration data reset\n");
            break;
        case 'm':
        case 'M':
            showMenu();
            break;
        default:
            Serial.println("Invalid command. Press 'M' for menu.");
    }
}

void showMenu() {
    Serial.println("\n=== CALIBRATION MENU ===");
    Serial.println("1. Calibrate Stationary (baseline)");
    Serial.println("2. Calibrate Normal Driving (30 sec)");
    Serial.println("3. Test Impact Detection");
    Serial.println("4. Test Rollover Detection");
    Serial.println("5. Test Spin Detection");
    Serial.println("6. Show Results & Recommendations");
    Serial.println("7. Reset Calibration Data");
    Serial.println("M. Show this menu");
    Serial.println("\nEnter command: ");
}

void calibrateStationary() {
    Serial.println("\n=== STATIONARY CALIBRATION ===");
    Serial.println("Place vehicle on level ground.");
    Serial.println("Do not move for 10 seconds.");
    Serial.println("Starting in 3 seconds...\n");
    delay(3000);
    
    resetCalibration();
    
    for (int i = 0; i < 500; i++) {
        int16_t ax, ay, az, gx, gy, gz;
        mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
        
        float accelX = ax / 4096.0;
        float accelY = ay / 4096.0;
        float accelZ = az / 4096.0;
        float gyroX = gx / 32.8;
        float gyroY = gy / 32.8;
        float gyroZ = gz / 32.8;
        
        updateCalibration(accelX, accelY, accelZ, gyroX, gyroY, gyroZ);
        
        if (i % 50 == 0) Serial.print(".");
        delay(20);
    }
    
    Serial.println("\n\nStationary calibration complete!");
    Serial.println("Baseline noise levels recorded.\n");
    showCurrentRanges();
}

void calibrateNormalDriving() {
    Serial.println("\n=== NORMAL DRIVING CALIBRATION ===");
    Serial.println("Drive normally on your typical terrain.");
    Serial.println("Include: turns, bumps, acceleration, braking.");
    Serial.println("Duration: 30 seconds");
    Serial.println("Starting in 5 seconds...\n");
    delay(5000);
    
    Serial.println("Recording... (30 seconds)");
    
    unsigned long startTime = millis();
    int samples = 0;
    
    while (millis() - startTime < 30000) {
        int16_t ax, ay, az, gx, gy, gz;
        mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
        
        float accelX = ax / 4096.0;
        float accelY = ay / 4096.0;
        float accelZ = az / 4096.0;
        float gyroX = gx / 32.8;
        float gyroY = gy / 32.8;
        float gyroZ = gz / 32.8;
        
        updateCalibration(accelX, accelY, accelZ, gyroX, gyroY, gyroZ);
        
        samples++;
        if (samples % 50 == 0) Serial.print(".");
        
        delay(20);
    }
    
    Serial.println("\n\nNormal driving calibration complete!");
    Serial.print("Samples recorded: ");
    Serial.println(samples);
    Serial.println();
    showCurrentRanges();
}

void testImpact() {
    Serial.println("\n=== IMPACT DETECTION TEST ===");
    Serial.println("Simulate an impact (shake or drop the unit).");
    Serial.println("Monitoring for 10 seconds...\n");
    
    unsigned long startTime = millis();
    float maxImpact = 0;
    
    while (millis() - startTime < 10000) {
        int16_t ax, ay, az, gx, gy, gz;
        mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
        
        float accelX = ax / 4096.0;
        float accelY = ay / 4096.0;
        float accelZ = az / 4096.0;
        
        float accelMagnitude = sqrt(accelX*accelX + accelY*accelY + accelZ*accelZ);
        float accelDelta = abs(accelMagnitude - 1.0);
        
        if (accelDelta > maxImpact) {
            maxImpact = accelDelta;
        }
        
        if (accelDelta > 2.0) {
            Serial.print("Impact detected! Magnitude: ");
            Serial.print(accelDelta);
            Serial.println("g");
        }
        
        delay(20);
    }
    
    Serial.println("\nTest complete.");
    Serial.print("Maximum impact detected: ");
    Serial.print(maxImpact);
    Serial.println("g");
    Serial.println();
}

void testRollover() {
    Serial.println("\n=== ROLLOVER DETECTION TEST ===");
    Serial.println("Tilt the unit to various angles.");
    Serial.println("Monitoring for 15 seconds...\n");
    
    unsigned long startTime = millis();
    float maxPitch = 0, maxRoll = 0;
    
    while (millis() - startTime < 15000) {
        int16_t ax, ay, az, gx, gy, gz;
        mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
        
        float accelX = ax / 4096.0;
        float accelY = ay / 4096.0;
        float accelZ = az / 4096.0;
        
        float pitch = atan2(accelY, sqrt(accelX*accelX + accelZ*accelZ)) * 180.0 / PI;
        float roll = atan2(accelX, sqrt(accelY*accelY + accelZ*accelZ)) * 180.0 / PI;
        
        if (abs(pitch) > abs(maxPitch)) maxPitch = pitch;
        if (abs(roll) > abs(maxRoll)) maxRoll = roll;
        
        if (abs(pitch) > 30 || abs(roll) > 30) {
            Serial.print("Tilt detected - Pitch: ");
            Serial.print(pitch);
            Serial.print("° Roll: ");
            Serial.print(roll);
            Serial.println("°");
        }
        
        delay(100);
    }
    
    Serial.println("\nTest complete.");
    Serial.print("Maximum pitch: ");
    Serial.print(maxPitch);
    Serial.println("°");
    Serial.print("Maximum roll: ");
    Serial.print(maxRoll);
    Serial.println("°\n");
}

void testSpin() {
    Serial.println("\n=== SPIN DETECTION TEST ===");
    Serial.println("Rotate the unit rapidly.");
    Serial.println("Monitoring for 10 seconds...\n");
    
    unsigned long startTime = millis();
    float maxSpin = 0;
    
    while (millis() - startTime < 10000) {
        int16_t ax, ay, az, gx, gy, gz;
        mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
        
        float gyroX = gx / 32.8;
        float gyroY = gy / 32.8;
        float gyroZ = gz / 32.8;
        
        float gyroMagnitude = sqrt(gyroX*gyroX + gyroY*gyroY + gyroZ*gyroZ);
        
        if (gyroMagnitude > maxSpin) {
            maxSpin = gyroMagnitude;
        }
        
        if (gyroMagnitude > 100) {
            Serial.print("Spin detected! Rate: ");
            Serial.print(gyroMagnitude);
            Serial.println("°/s");
        }
        
        delay(20);
    }
    
    Serial.println("\nTest complete.");
    Serial.print("Maximum spin rate: ");
    Serial.print(maxSpin);
    Serial.println("°/s\n");
}

void showResults() {
    Serial.println("\n=== CALIBRATION RESULTS ===\n");
    showCurrentRanges();
    
    Serial.println("\n=== RECOMMENDED THRESHOLDS ===\n");
    
    // Calculate recommended thresholds with safety margins
    float normalAccelRange = max(abs(calData.maxAccelX), max(abs(calData.maxAccelY), abs(calData.maxAccelZ)));
    float normalGyroRange = max(abs(calData.maxGyroX), max(abs(calData.maxGyroY), abs(calData.maxGyroZ)));
    
    float recommendedImpact = normalAccelRange * 2.0 + 1.0; // 2x normal + margin
    float recommendedSpin = normalGyroRange * 2.0 + 50.0;   // 2x normal + margin
    
    Serial.println("Add these to your vehicle-unit.ino:");
    Serial.println();
    Serial.print("#define IMPACT_THRESHOLD_G ");
    Serial.println(recommendedImpact, 1);
    Serial.print("#define SPIN_THRESHOLD_DPS ");
    Serial.println(recommendedSpin, 1);
    Serial.println("#define ROLLOVER_ANGLE_DEG 45.0  // Standard");
    Serial.println();
    
    Serial.println("=== NOTES ===");
    Serial.println("- Test these thresholds in actual driving conditions");
    Serial.println("- Adjust if false positives occur");
    Serial.println("- Lower thresholds = more sensitive (more false positives)");
    Serial.println("- Higher thresholds = less sensitive (may miss real accidents)");
    Serial.println();
}

void resetCalibration() {
    calData.maxAccelX = calData.maxAccelY = calData.maxAccelZ = -999;
    calData.maxGyroX = calData.maxGyroY = calData.maxGyroZ = -999;
    calData.minAccelX = calData.minAccelY = calData.minAccelZ = 999;
    calData.minGyroX = calData.minGyroY = calData.minGyroZ = 999;
}

void updateCalibration(float ax, float ay, float az, float gx, float gy, float gz) {
    if (ax > calData.maxAccelX) calData.maxAccelX = ax;
    if (ay > calData.maxAccelY) calData.maxAccelY = ay;
    if (az > calData.maxAccelZ) calData.maxAccelZ = az;
    if (gx > calData.maxGyroX) calData.maxGyroX = gx;
    if (gy > calData.maxGyroY) calData.maxGyroY = gy;
    if (gz > calData.maxGyroZ) calData.maxGyroZ = gz;
    
    if (ax < calData.minAccelX) calData.minAccelX = ax;
    if (ay < calData.minAccelY) calData.minAccelY = ay;
    if (az < calData.minAccelZ) calData.minAccelZ = az;
    if (gx < calData.minGyroX) calData.minGyroX = gx;
    if (gy < calData.minGyroY) calData.minGyroY = gy;
    if (gz < calData.minGyroZ) calData.minGyroZ = gz;
}

void showCurrentRanges() {
    Serial.println("Current recorded ranges:");
    Serial.println("Accelerometer (g):");
    Serial.print("  X: ");
    Serial.print(calData.minAccelX, 2);
    Serial.print(" to ");
    Serial.println(calData.maxAccelX, 2);
    Serial.print("  Y: ");
    Serial.print(calData.minAccelY, 2);
    Serial.print(" to ");
    Serial.println(calData.maxAccelY, 2);
    Serial.print("  Z: ");
    Serial.print(calData.minAccelZ, 2);
    Serial.print(" to ");
    Serial.println(calData.maxAccelZ, 2);
    
    Serial.println("Gyroscope (°/s):");
    Serial.print("  X: ");
    Serial.print(calData.minGyroX, 1);
    Serial.print(" to ");
    Serial.println(calData.maxGyroX, 1);
    Serial.print("  Y: ");
    Serial.print(calData.minGyroY, 1);
    Serial.print(" to ");
    Serial.println(calData.maxGyroY, 1);
    Serial.print("  Z: ");
    Serial.print(calData.minGyroZ, 1);
    Serial.print(" to ");
    Serial.println(calData.maxGyroZ, 1);
}
