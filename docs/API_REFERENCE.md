# API Reference

## Protocol Specification

### Emergency Packet Structure

```cpp
typedef struct __attribute__((packed)) {
    uint8_t version;        // Protocol version (currently 1)
    uint8_t vehicleId[4];   // Unique vehicle identifier
    uint8_t alertType;      // Type of accident detected
    uint32_t timestamp;     // Milliseconds since boot
    uint8_t checksum;       // XOR checksum for validation
} EmergencyPacket;
```

**Total Size:** 11 bytes

### Alert Types

| Value | Constant | Description |
|-------|----------|-------------|
| 0x01 | ALERT_TYPE_IMPACT | Sudden impact detected (>3g acceleration) |
| 0x02 | ALERT_TYPE_ROLLOVER | Vehicle rollover (>45° tilt) |
| 0x03 | ALERT_TYPE_SPIN | Loss of control (>180°/s rotation) |
| 0x04 | ALERT_TYPE_COMBINED | Multiple conditions detected |

### Checksum Calculation

```cpp
uint8_t calculateChecksum(const EmergencyPacket* packet) {
    uint8_t checksum = 0;
    const uint8_t* data = (const uint8_t*)packet;
    for (size_t i = 0; i < sizeof(EmergencyPacket) - 1; i++) {
        checksum ^= data[i];
    }
    return checksum;
}
```

### Packet Validation

```cpp
bool verifyChecksum(const EmergencyPacket* packet) {
    return packet->checksum == calculateChecksum(packet);
}
```

## Vehicle Unit API

### Configuration Functions

#### setVehicleId()
```cpp
void setVehicleId(uint8_t id[4])
```
Sets unique vehicle identifier. Must be called before system initialization.

**Parameters:**
- `id`: 4-byte array containing unique vehicle ID

**Example:**
```cpp
uint8_t myId[4] = {0x01, 0x00, 0x00, 0x01};
setVehicleId(myId);
```

#### setThresholds()
```cpp
void setThresholds(float impact, float rollover, float spin)
```
Dynamically adjust detection thresholds.

**Parameters:**
- `impact`: Impact threshold in g-force (default: 3.0)
- `rollover`: Rollover angle in degrees (default: 45.0)
- `spin`: Spin rate in degrees/second (default: 180.0)

### Detection Functions

#### detectAccident()
```cpp
void detectAccident(float ax, float ay, float az, float gx, float gy, float gz)
```
Main accident detection algorithm. Called continuously in loop.

**Parameters:**
- `ax, ay, az`: Accelerometer readings in g-force
- `gx, gy, gz`: Gyroscope readings in degrees/second

**Returns:** void (updates global state)

#### confirmAccident()
```cpp
void confirmAccident()
```
Called when accident is confirmed (2 of 3 conditions met). Activates beacon.

### Beacon Functions

#### transmitBeacon()
```cpp
void transmitBeacon()
```
Transmits emergency packet via RF module.

**Transmission Format:**
- Baud rate: 9600
- Protocol: Serial (UART)
- Packet size: 11 bytes
- Interval: 5 seconds (configurable)

#### cancelBeacon()
```cpp
void cancelBeacon()
```
Manually cancels beacon transmission. Called by override button handler.

### State Functions

#### resetDetectionState()
```cpp
void resetDetectionState()
```
Resets detection flags and timers. Called after beacon cancellation.

## Receiver Unit API

### Reception Functions

#### receivePacket()
```cpp
void receivePacket()
```
Receives and validates emergency packet from RF module.

**Returns:** void (updates global state)

**Side Effects:**
- Updates `lastPacket` with received data
- Sets `alertActive` flag
- Triggers buzzer and LED alerts

#### verifyPacket()
```cpp
bool verifyPacket(EmergencyPacket* packet)
```
Validates received packet.

**Parameters:**
- `packet`: Pointer to received packet

**Returns:**
- `true`: Packet valid (correct version and checksum)
- `false`: Packet invalid

### Display Functions

#### updateDisplay()
```cpp
void updateDisplay()
```
Updates LCD with current alert information.

**Display Format:**
```
Line 1: V:XXXX TYPE
Line 2: RSSI:XX ■■■□□
```

#### showVehicleId()
```cpp
void showVehicleId(uint8_t id[4])
```
Displays vehicle ID on LCD.

**Parameters:**
- `id`: 4-byte vehicle identifier

### RSSI Functions

#### readRSSI()
```cpp
int readRSSI()
```
Reads signal strength from RF module.

**Returns:**
- RSSI value in dBm (typically -120 to -30)

**Note:** Current implementation uses simulated RSSI. Implement actual RF module query for production.

#### updateRSSIIndicator()
```cpp
void updateRSSIIndicator(int rssi)
```
Updates 5-LED bar graph based on RSSI value.

**Parameters:**
- `rssi`: Signal strength in dBm

**LED Mapping:**
- 1 LED: Very weak (-120 to -100 dBm)
- 2 LEDs: Weak (-100 to -80 dBm)
- 3 LEDs: Medium (-80 to -60 dBm)
- 4 LEDs: Strong (-60 to -40 dBm)
- 5 LEDs: Very strong (-40 to -30 dBm)

### Alert Functions

#### activateAlert()
```cpp
void activateAlert()
```
Activates alert indicators (buzzer, LEDs, LCD).

**Behavior:**
- Sounds buzzer 3 times (200ms on/off)
- Turns on red alert LED
- Updates LCD display

#### clearAlert()
```cpp
void clearAlert()
```
Clears active alert after timeout or manual cancellation.

## MPU6050 Driver API

### Initialization

#### initialize()
```cpp
void initialize()
```
Initializes MPU6050 sensor. Wakes up device and sets default configuration.

#### testConnection()
```cpp
bool testConnection()
```
Tests I2C connection to MPU6050.

**Returns:**
- `true`: Sensor connected and responding
- `false`: Sensor not found

### Configuration

#### setFullScaleAccelRange()
```cpp
void setFullScaleAccelRange(uint8_t range)
```
Sets accelerometer sensitivity range.

**Parameters:**
- `MPU6050_ACCEL_FS_2`: ±2g
- `MPU6050_ACCEL_FS_4`: ±4g
- `MPU6050_ACCEL_FS_8`: ±8g (default)
- `MPU6050_ACCEL_FS_16`: ±16g

#### setFullScaleGyroRange()
```cpp
void setFullScaleGyroRange(uint8_t range)
```
Sets gyroscope sensitivity range.

**Parameters:**
- `MPU6050_GYRO_FS_250`: ±250°/s
- `MPU6050_GYRO_FS_500`: ±500°/s
- `MPU6050_GYRO_FS_1000`: ±1000°/s (default)
- `MPU6050_GYRO_FS_2000`: ±2000°/s

### Data Reading

#### getMotion6()
```cpp
void getMotion6(int16_t* ax, int16_t* ay, int16_t* az, 
                int16_t* gx, int16_t* gy, int16_t* gz)
```
Reads all 6 axes (accelerometer + gyroscope) in one operation.

**Parameters:**
- `ax, ay, az`: Pointers to store accelerometer raw values
- `gx, gy, gz`: Pointers to store gyroscope raw values

**Raw Value Conversion:**
```cpp
// For ±8g range
float accelG = rawValue / 4096.0;

// For ±1000°/s range
float gyroDPS = rawValue / 32.8;
```

## Configuration Constants

### Vehicle Unit (config.h)

| Constant | Default | Description |
|----------|---------|-------------|
| VEHICLE_ID | {0x01,0x00,0x00,0x01} | Unique vehicle identifier |
| IMPACT_THRESHOLD_G | 3.0 | Impact detection threshold (g) |
| ROLLOVER_ANGLE_DEG | 45.0 | Rollover angle threshold (degrees) |
| SPIN_THRESHOLD_DPS | 180.0 | Spin rate threshold (°/s) |
| CONFIRMATION_WINDOW_MS | 300 | Multi-condition window (ms) |
| BEACON_INTERVAL_MS | 5000 | Beacon transmission interval (ms) |
| OVERRIDE_HOLD_TIME_MS | 5000 | Override button hold time (ms) |
| IMU_SAMPLE_RATE_HZ | 50 | IMU sampling frequency (Hz) |

### Receiver Unit (config.h)

| Constant | Default | Description |
|----------|---------|-------------|
| LCD_I2C_ADDRESS | 0x27 | LCD I2C address |
| ALERT_TIMEOUT_MS | 30000 | Alert auto-clear timeout (ms) |
| BUZZER_BEEP_COUNT | 3 | Number of beeps on alert |
| RSSI_MIN | -120 | Minimum RSSI value (dBm) |
| RSSI_MAX | -30 | Maximum RSSI value (dBm) |
| RSSI_SIMULATED | true | Use simulated RSSI |
| SHOW_FULL_VEHICLE_ID | false | Display full or short ID |

## Error Codes

### Vehicle Unit

| Code | Description | Resolution |
|------|-------------|------------|
| MPU6050 connection failed | IMU not detected on I2C bus | Check wiring, verify 3.3V power |
| Watchdog timeout | Firmware hung | System auto-restarts |

### Receiver Unit

| Code | Description | Resolution |
|------|-------------|------------|
| Invalid protocol version | Packet from incompatible version | Update firmware |
| Checksum failed | Corrupted packet | Ignore packet, wait for next |
| LCD not found | I2C communication error | Check address, verify wiring |

## Serial Debug Output

### Vehicle Unit

```
MPU6050 connected
Off-Highway Guardian - Vehicle Unit Ready
Impact detected!
Rollover detected!
ACCIDENT CONFIRMED - ACTIVATING BEACON
Beacon transmitted - Type: 0x04
Beacon cancelled by manual override
```

### Receiver Unit

```
Off-Highway Guardian - Receiver Unit Ready
Monitoring...
Packet received - Vehicle: 01000001 Type: COMBINED
ALERT ACTIVATED
Alert cleared
```

## Integration Examples

### Custom Alert Handler

```cpp
void onAccidentDetected(uint8_t alertType) {
    // Custom logic when accident detected
    switch(alertType) {
        case ALERT_TYPE_IMPACT:
            // Handle impact
            break;
        case ALERT_TYPE_ROLLOVER:
            // Handle rollover
            break;
        case ALERT_TYPE_SPIN:
            // Handle spin
            break;
    }
}
```

### External Logging

```cpp
void logEvent(EmergencyPacket* packet) {
    // Log to SD card, EEPROM, or external system
    File logFile = SD.open("events.log", FILE_APPEND);
    logFile.write((uint8_t*)packet, sizeof(EmergencyPacket));
    logFile.close();
}
```

### GPS Integration (Future)

```cpp
void addGPSData(EmergencyPacket* packet) {
    // Extend packet with GPS coordinates when available
    // Requires protocol version update
}
```

## Performance Metrics

| Metric | Target | Typical |
|--------|--------|---------|
| Detection Latency | < 500ms | 250-350ms |
| Beacon Activation | < 1s | 800-900ms |
| RF Transmission | Every 5s | 5.0s ±10ms |
| IMU Sample Rate | 50 Hz | 50 Hz |
| Power Consumption (Vehicle) | < 500mA | 150-400mA |
| Power Consumption (Receiver) | < 500mA | 220-410mA |

---

**Version:** 1.0  
**Last Updated:** January 2026  
**Protocol Version:** 1
