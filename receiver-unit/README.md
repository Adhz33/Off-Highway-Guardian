# Receiver Unit - Installation Guide

## Hardware Setup

### Components Required
- ESP32 DevKit or Arduino Nano/Uno
- HC-12 433MHz RF Module (matching vehicle unit)
- 16x2 LCD with I2C backpack (PCF8574)
- 5x LEDs for RSSI bar indicator
- 1x Buzzer (active or passive)
- Resistors: 5x 220Ω (for LEDs)
- Power supply: USB power bank or 5V adapter

### Wiring Diagram

#### HC-12 RF Module to ESP32
```
HC-12       ->  ESP32
VCC         ->  5V
GND         ->  GND
TX          ->  GPIO 16 (RX2)
RX          ->  GPIO 17 (TX2)
```

#### 16x2 LCD (I2C) to ESP32
```
LCD I2C     ->  ESP32
VCC         ->  5V
GND         ->  GND
SDA         ->  GPIO 21
SCL         ->  GPIO 22
```

#### Status LEDs
```
LED                 ->  ESP32       ->  Resistor
Signal (Blue)       ->  GPIO 2      ->  220Ω -> GND
Alert (Red)         ->  GPIO 4      ->  220Ω -> GND
```

#### RSSI Bar LEDs (Signal Strength Indicator)
```
RSSI LED    ->  ESP32       ->  Resistor
LED 1       ->  GPIO 12     ->  220Ω -> GND
LED 2       ->  GPIO 13     ->  220Ω -> GND
LED 3       ->  GPIO 14     ->  220Ω -> GND
LED 4       ->  GPIO 27     ->  220Ω -> GND
LED 5       ->  GPIO 26     ->  220Ω -> GND
```

#### Buzzer
```
Buzzer      ->  ESP32
Positive    ->  GPIO 5
Negative    ->  GND
```

## Software Setup

### Arduino IDE Configuration

1. Install Arduino IDE
2. Add ESP32 board support (same as vehicle unit)
3. Install required libraries:
   - Wire (built-in)
   - LiquidCrystal_I2C (Library Manager -> Search "LiquidCrystal I2C" by Frank de Brabander)

4. Select board:
   - Tools -> Board -> ESP32 Dev Module
   - Tools -> Upload Speed -> 115200
   - Tools -> Port -> (Select your COM port)

### LCD I2C Address Configuration

The LCD I2C address may be 0x27 or 0x3F. To find your address:

1. Upload I2C scanner sketch:
```cpp
#include <Wire.h>

void setup() {
  Serial.begin(115200);
  Wire.begin();
  Serial.println("I2C Scanner");
}

void loop() {
  for(byte i = 8; i < 120; i++) {
    Wire.beginTransmission(i);
    if(Wire.endTransmission() == 0) {
      Serial.print("Found address: 0x");
      Serial.println(i, HEX);
    }
  }
  delay(5000);
}
```

2. Update `receiver-unit.ino`:
```cpp
LiquidCrystal_I2C lcd(0x27, 16, 2); // Change 0x27 to your address
```

### Uploading Firmware

1. Connect ESP32 via USB
2. Open `receiver-unit.ino` in Arduino IDE
3. Click Upload button
4. Monitor Serial output (115200 baud)

## Operation

### Normal Operation
1. Power on the receiver unit
2. LCD displays "Monitoring..."
3. Unit continuously listens for emergency beacons

### When Alert Received
1. Buzzer sounds 3 beeps
2. Red alert LED turns on
3. LCD displays:
   - Line 1: Vehicle ID and alert type
   - Line 2: RSSI value and signal strength bars
4. RSSI bar LEDs indicate signal strength (1-5 LEDs)

### RF Homing (Locating Vehicle)
1. Note the RSSI value on LCD
2. Walk in different directions
3. RSSI increases as you get closer to vehicle
4. More LEDs light up = stronger signal = closer to vehicle
5. Follow direction of increasing RSSI to locate accident site

### Alert Timeout
- Alert automatically clears after 30 seconds of no signal
- This prevents stale alerts if vehicle beacon is manually cancelled

## Deployment Scenarios

### Check Post Installation
1. Mount receiver at check post desk
2. Connect to mains power (5V adapter)
3. Position for operator visibility
4. Keep log book for recording alerts

### Patrol Officer Portable Unit
1. Use USB power bank for portability
2. Mount in protective case
3. Attach to belt or vest
4. Use RSSI homing to locate vehicles in distress

## Range Testing

### Open Terrain Test
1. Place vehicle unit at known location
2. Trigger test alert (tilt IMU)
3. Walk away with receiver unit
4. Note maximum distance for reliable reception
5. Target: 500m minimum in open terrain

### Forest Canopy Test
1. Repeat test in actual forest environment
2. Note reduced range due to foliage
3. Document dead zones
4. Plan check post locations accordingly

## Troubleshooting

### No LCD Display
- Check I2C address (0x27 or 0x3F)
- Verify I2C wiring (SDA/SCL)
- Adjust LCD contrast potentiometer on I2C backpack

### Not Receiving Packets
- Verify RF module power (5V, 100mA+)
- Check TX/RX pins not swapped
- Ensure vehicle and receiver use same RF frequency
- Test RF modules with AT commands

### Buzzer Not Working
- Check polarity (active buzzers have polarity)
- Verify GPIO pin connection
- Test with simple digitalWrite HIGH/LOW

### RSSI Always Same Value
- Current implementation uses simulated RSSI
- For real RSSI, implement AT+RX command for HC-12
- Or use LoRa module with built-in RSSI function

## Advanced: Real RSSI Implementation

For HC-12 modules, implement actual RSSI reading:

```cpp
int readRSSI() {
    Serial2.print("AT+RX");
    delay(100);
    
    if (Serial2.available()) {
        String response = Serial2.readString();
        // Parse RSSI from response
        // Format: "OK+RX:-XXX"
        int rssiStart = response.indexOf(":-") + 2;
        String rssiStr = response.substring(rssiStart);
        return -rssiStr.toInt();
    }
    
    return -100; // Default if no response
}
```

## Multi-Vehicle Monitoring

The receiver can monitor multiple vehicles simultaneously:
- Each vehicle has unique ID in beacon
- LCD displays most recent alert
- Serial monitor logs all received packets
- Future enhancement: Queue multiple alerts

## Safety Notes

⚠️ Prototype system - not for production use
⚠️ Test range in actual deployment environment
⚠️ Carry backup communication devices
⚠️ Train all operators on RF homing procedure
⚠️ Document all alert responses for review
