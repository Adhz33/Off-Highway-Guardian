/*
 * Configuration file for Off-Highway Guardian Receiver Unit
 * Modify these values for your specific deployment
 */

#ifndef CONFIG_H
#define CONFIG_H

// ============================================
// HARDWARE PIN ASSIGNMENTS
// ============================================

// RF Module (Serial2)
#define RF_TX_PIN 17             // ESP32 TX2 -> RF Module RX
#define RF_RX_PIN 16             // ESP32 RX2 -> RF Module TX

// Status LEDs
#define LED_SIGNAL 2             // Blue LED - Signal received
#define LED_ALERT 4              // Red LED - Active alert

// Buzzer
#define BUZZER_PIN 5             // Active buzzer

// RSSI Bar LEDs (signal strength indicator)
#define RSSI_LED_1 12            // Weakest signal
#define RSSI_LED_2 13
#define RSSI_LED_3 14
#define RSSI_LED_4 27
#define RSSI_LED_5 26            // Strongest signal

// I2C for LCD (default pins)
// SDA: GPIO 21
// SCL: GPIO 22

// ============================================
// LCD CONFIGURATION
// ============================================
// LCD I2C address (common values: 0x27 or 0x3F)
// Use I2C scanner if unsure
#define LCD_I2C_ADDRESS 0x27

// LCD dimensions
#define LCD_COLS 16
#define LCD_ROWS 2

// ============================================
// ALERT SETTINGS
// ============================================
// Alert timeout in milliseconds
// Alert clears if no signal received for this duration
// Default: 30000ms (30 seconds)
#define ALERT_TIMEOUT_MS 30000

// Buzzer beep count on new alert
// Default: 3 beeps
#define BUZZER_BEEP_COUNT 3

// Buzzer beep duration in milliseconds
// Default: 200ms on, 200ms off
#define BUZZER_BEEP_DURATION_MS 200

// ============================================
// RSSI SETTINGS
// ============================================
// RSSI range for mapping to LED bar
// Typical values: -120 dBm (weak) to -30 dBm (strong)
#define RSSI_MIN -120
#define RSSI_MAX -30

// RSSI update interval in milliseconds
// How often to refresh RSSI reading
// Default: 1000ms (1 second)
#define RSSI_UPDATE_INTERVAL_MS 1000

// Enable simulated RSSI (for testing without real RSSI)
// Set to false when using actual RF module RSSI
#define RSSI_SIMULATED true

// ============================================
// COMMUNICATION SETTINGS
// ============================================
// Serial baud rate for debugging
#define SERIAL_BAUD 115200

// RF module baud rate (HC-12 default: 9600)
#define RF_BAUD 9600

// ============================================
// DISPLAY SETTINGS
// ============================================
// Display update interval in milliseconds
// Default: 100ms (10Hz refresh)
#define DISPLAY_UPDATE_INTERVAL_MS 100

// Show full vehicle ID (true) or shortened (false)
// Full: 01020304, Shortened: 0304 (last 2 bytes)
#define SHOW_FULL_VEHICLE_ID false

// ============================================
// MULTI-VEHICLE SUPPORT
// ============================================
// Maximum number of vehicles to track simultaneously
// Default: 1 (display most recent alert only)
// Future: Implement queue for multiple alerts
#define MAX_TRACKED_VEHICLES 1

// ============================================
// DEBUGGING
// ============================================
// Enable serial debug output
#define DEBUG_ENABLED true

// Log all received packets to serial
#define LOG_ALL_PACKETS true

// ============================================
// ADVANCED SETTINGS
// ============================================
// Enable packet statistics tracking
// Tracks: total packets, errors, success rate
#define ENABLE_STATISTICS true

// Backlight auto-off timeout (0 = always on)
// Default: 0 (always on for emergency visibility)
#define LCD_BACKLIGHT_TIMEOUT_MS 0

#endif // CONFIG_H
