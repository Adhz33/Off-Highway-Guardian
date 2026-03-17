/*
 * Configuration file for Off-Highway Guardian Vehicle Unit
 * Modify these values for your specific deployment
 */

#ifndef CONFIG_H
#define CONFIG_H

// ============================================
// VEHICLE IDENTIFICATION
// ============================================
// IMPORTANT: Each vehicle must have a unique ID
// Format: 4 bytes (hex values)
// Example: Vehicle 1 = {0x01, 0x00, 0x00, 0x01}
//          Vehicle 2 = {0x01, 0x00, 0x00, 0x02}
const uint8_t VEHICLE_ID[4] = {0x01, 0x00, 0x00, 0x01};

// ============================================
// DETECTION THRESHOLDS
// ============================================
// Impact Detection: Acceleration threshold in g-force
// Default: 3.0g (suitable for most off-road vehicles)
// Increase if too many false positives on rough terrain
// Decrease if missing real impacts (not recommended < 2.5g)
#define IMPACT_THRESHOLD_G 3.0

// Rollover Detection: Tilt angle threshold in degrees
// Default: 45° (vehicle tilted beyond normal operation)
// Adjust based on vehicle type and terrain
#define ROLLOVER_ANGLE_DEG 45.0

// Spin Detection: Angular velocity threshold in degrees/second
// Default: 180°/s (rapid uncontrolled rotation)
// Increase if false positives during sharp turns
#define SPIN_THRESHOLD_DPS 180.0

// Multi-condition confirmation window in milliseconds
// Requires 2 of 3 conditions within this window
// Default: 300ms (prevents single-spike false positives)
#define CONFIRMATION_WINDOW_MS 300

// ============================================
// BEACON SETTINGS
// ============================================
// Beacon transmission interval in milliseconds
// Default: 5000ms (5 seconds between transmissions)
// Shorter interval = faster updates but more power consumption
#define BEACON_INTERVAL_MS 5000

// Manual override hold time in milliseconds
// Driver must hold button this long to cancel beacon
// Default: 5000ms (prevents accidental cancellation)
#define OVERRIDE_HOLD_TIME_MS 5000

// ============================================
// HARDWARE PIN ASSIGNMENTS
// ============================================
// Adjust these if using different GPIO pins

// Status LEDs
#define LED_SYSTEM_ACTIVE 2      // Green LED - System operational
#define LED_ACCIDENT_DETECTED 4  // Yellow LED - Accident confirmed
#define LED_BEACON_ACTIVE 5      // Red LED - Beacon transmitting

// Manual override button
#define BUTTON_OVERRIDE 15       // Push button (active low with pull-up)

// RF Module (Serial2)
#define RF_TX_PIN 17             // ESP32 TX2 -> RF Module RX
#define RF_RX_PIN 16             // ESP32 RX2 -> RF Module TX

// I2C for MPU6050 (default pins)
// SDA: GPIO 21
// SCL: GPIO 22

// ============================================
// IMU CONFIGURATION
// ============================================
// Accelerometer full scale range
// Options: MPU6050_ACCEL_FS_2, _4, _8, _16 (±2g, ±4g, ±8g, ±16g)
// Default: ±8g (suitable for off-road impacts)
#define ACCEL_RANGE MPU6050_ACCEL_FS_8

// Gyroscope full scale range
// Options: MPU6050_GYRO_FS_250, _500, _1000, _2000 (±250°/s to ±2000°/s)
// Default: ±1000°/s (suitable for rollover detection)
#define GYRO_RANGE MPU6050_GYRO_FS_1000

// IMU sampling rate in Hz
// Default: 50Hz (20ms between samples)
// Higher rate = more responsive but more processing
#define IMU_SAMPLE_RATE_HZ 50
#define IMU_SAMPLE_INTERVAL_MS (1000 / IMU_SAMPLE_RATE_HZ)

// ============================================
// POWER MANAGEMENT
// ============================================
// Watchdog timer timeout in milliseconds
// System will auto-restart if firmware hangs
// Default: 5000ms (5 seconds)
#define WATCHDOG_TIMEOUT_MS 5000

// Low voltage threshold (12V system)
// Alert if vehicle battery drops below this
// Default: 10.5V (typical 12V battery low threshold)
#define LOW_VOLTAGE_THRESHOLD 10.5

// ============================================
// DEBUGGING
// ============================================
// Enable serial debug output
// Set to false for production to reduce overhead
#define DEBUG_ENABLED true

// Serial baud rate
#define SERIAL_BAUD 115200

// RF module baud rate (HC-12 default: 9600)
#define RF_BAUD 9600

// ============================================
// ADVANCED SETTINGS
// ============================================
// Enable event logging to EEPROM
// Stores last 10 accident events
#define ENABLE_EVENT_LOGGING false

// Enable encryption for RF packets
// Requires AES library and key configuration
#define ENABLE_ENCRYPTION false

// Beacon auto-shutoff after this many minutes
// 0 = never auto-shutoff (requires manual override)
// Default: 0 (continuous until manually cancelled)
#define BEACON_AUTO_SHUTOFF_MIN 0

#endif // CONFIG_H
