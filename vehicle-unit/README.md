# Vehicle Unit - Installation Guide

## Hardware Setup

### Components Required
- ESP32 DevKit or Arduino Nano/Uno
- MPU-6050 IMU Module (GY-521)
- HC-12 433MHz RF Module or LoRa SX1278
- 12V to 5V/3.3V Buck Converter (LM2596 or similar)
- 3x LEDs (Green, Yellow, Red)
- 1x Push Button (NO - Normally Open)
- Resistors: 3x 220Ω (for LEDs), 1x 10kΩ (pull-up for button)
- Breadboard or PCB for prototyping

### Wiring Diagram

#### MPU-6050 to ESP32/Arduino
```
MPU-6050    ->  ESP32/Arduino
VCC         ->  3.3V
GND         ->  GND
SCL         ->  GPIO 22 (ESP32) / A5 (Arduino)
SDA         ->  GPIO 21 (ESP32) / A4 (Arduino)
```

#### HC-12 RF Module to ESP32
```
HC-12       ->  ESP32
VCC         ->  5V
GND         ->  GND
TX          ->  GPIO 16 (RX2)
RX          ->  GPIO 17 (TX2)
```

#### Status LEDs
```
LED                     ->  ESP32       ->  Resistor
System Active (Green)   ->  GPIO 2      ->  220Ω -> GND
Accident Detected (Yel) ->  GPIO 4      ->  220Ω -> GND
Beacon Active (Red)     ->  GPIO 5      ->  220Ω -> GND
```

#### Manual Override Button
```
Button      ->  ESP32
One side    ->  GPIO 15
Other side  ->  GND
(Internal pull-up enabled in code)
```

#### Power Supply
```
Vehicle 12V -> Buck Converter -> 5V -> ESP32 VIN
                              -> 3.3V -> MPU-6050 VCC
```

## Software Setup

### Arduino IDE Configuration

1. Install Arduino IDE (1.8.x or 2.x)
2. Add ESP32 board support:
   - File -> Preferences
   - Additional Board URLs: `https://dl.espressif.com/dl/package_esp32_index.json`
   - Tools -> Board -> Boards Manager -> Search "ESP32" -> Install

3. Install required libraries:
   - Wire (built-in)
   - No external libraries needed (MPU6050.h is included)

4. Select board:
   - Tools -> Board -> ESP32 Dev Module
   - Tools -> Upload Speed -> 115200
   - Tools -> Port -> (Select your COM port)

### Configuration

Edit `vehicle-unit.ino` to configure:

```cpp
// Vehicle ID - MUST be unique per vehicle
const uint8_t VEHICLE_ID[4] = {0x01, 0x02, 0x03, 0x04};

// Adjust thresholds based on calibration
#define IMPACT_THRESHOLD_G 3.0      // Increase for rougher terrain
#define ROLLOVER_ANGLE_DEG 45.0     // Tilt angle threshold
#define SPIN_THRESHOLD_DPS 180.0    // Angular velocity threshold
```

### Uploading Firmware

1. Connect ESP32 via USB
2. Open `vehicle-unit.ino` in Arduino IDE
3. Click Upload button
4. Monitor Serial output (115200 baud) to verify operation

## Calibration

### Step 1: Baseline Testing
1. Power on the system with vehicle stationary
2. Verify "System Active" LED is ON
3. Check Serial Monitor shows "MPU6050 connected"

### Step 2: Impact Threshold Calibration
1. Simulate controlled impacts (drop test on cushioned surface)
2. Observe Serial Monitor for "Impact detected!" messages
3. Adjust `IMPACT_THRESHOLD_G` if too sensitive or not sensitive enough

### Step 3: Rollover Threshold Testing
1. Tilt the unit to various angles
2. Verify detection at 45° tilt
3. Adjust `ROLLOVER_ANGLE_DEG` if needed

### Step 4: False Positive Testing
1. Drive on rough terrain for 30 minutes
2. Count false alerts
3. Increase thresholds if false positive rate > 5%

### Step 5: Manual Override Test
1. Trigger an alert (tilt or shake the unit)
2. Hold override button for 5 seconds
3. Verify beacon cancels and LEDs turn off

## Installation in Vehicle

1. Mount IMU rigidly to vehicle chassis (avoid flexible mounting)
2. Orient IMU with Z-axis vertical when vehicle is level
3. Route power from vehicle 12V supply through fuse (5A recommended)
4. Mount override button within driver reach
5. Position LEDs for driver visibility
6. Secure RF antenna for maximum range (vertical orientation)

## Troubleshooting

### MPU6050 Not Detected
- Check I2C wiring (SDA/SCL)
- Verify 3.3V power supply
- Try different I2C address (0x68 or 0x69)

### No RF Transmission
- Check HC-12 power (requires 5V, 100mA+)
- Verify TX/RX pins not swapped
- Test HC-12 with AT commands

### False Positives
- Increase `IMPACT_THRESHOLD_G`
- Ensure IMU is rigidly mounted
- Check for loose connections causing vibration

### System Resets Randomly
- Add capacitor (100µF) across power supply
- Check voltage regulator output stability
- Implement watchdog timer (already in code)

## Safety Notes

⚠️ This is a prototype system for academic purposes
⚠️ Do not rely solely on this system for safety-critical applications
⚠️ Always carry backup communication devices
⚠️ Test thoroughly before field deployment
