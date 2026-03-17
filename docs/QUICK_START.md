# Quick Start Guide

Get your Off-Highway Guardian system up and running in 30 minutes!

## What You Need

### Vehicle Unit
- ESP32 DevKit or Arduino Nano
- MPU-6050 IMU module
- HC-12 433MHz RF module
- 12V to 5V buck converter
- 3 LEDs, 1 button, resistors
- Breadboard and jumper wires

### Receiver Unit
- ESP32 DevKit or Arduino Nano
- HC-12 433MHz RF module
- 16x2 LCD with I2C backpack
- 5 LEDs, 1 buzzer, resistors
- USB power bank
- Breadboard and jumper wires

## Step 1: Hardware Assembly (15 minutes)

### Vehicle Unit Wiring

```
MPU-6050 -> ESP32
  VCC -> 3.3V
  GND -> GND
  SDA -> GPIO 21
  SCL -> GPIO 22

HC-12 -> ESP32
  VCC -> 5V
  GND -> GND
  TX -> GPIO 16
  RX -> GPIO 17

LEDs -> ESP32 (with 220Ω resistors)
  Green -> GPIO 2 -> GND
  Yellow -> GPIO 4 -> GND
  Red -> GPIO 5 -> GND

Button -> ESP32
  One side -> GPIO 15
  Other side -> GND
```

### Receiver Unit Wiring

```
HC-12 -> ESP32
  VCC -> 5V
  GND -> GND
  TX -> GPIO 16
  RX -> GPIO 17

LCD (I2C) -> ESP32
  VCC -> 5V
  GND -> GND
  SDA -> GPIO 21
  SCL -> GPIO 22

Buzzer -> ESP32
  + -> GPIO 5
  - -> GND

RSSI LEDs -> ESP32 (with 220Ω resistors)
  LED1 -> GPIO 12 -> GND
  LED2 -> GPIO 13 -> GND
  LED3 -> GPIO 14 -> GND
  LED4 -> GPIO 27 -> GND
  LED5 -> GPIO 26 -> GND
```

## Step 2: Software Setup (10 minutes)

### Install Arduino IDE

1. Download from https://www.arduino.cc/en/software
2. Install ESP32 board support:
   - File -> Preferences
   - Add URL: `https://dl.espressif.com/dl/package_esp32_index.json`
   - Tools -> Board -> Boards Manager
   - Search "ESP32" and install

### Install Libraries

1. Open Library Manager (Tools -> Manage Libraries)
2. Search and install:
   - "LiquidCrystal I2C" by Frank de Brabander

## Step 3: Upload Firmware (5 minutes)

### Vehicle Unit

1. Open `vehicle-unit/vehicle-unit.ino`
2. Edit `config.h` to set unique vehicle ID:
   ```cpp
   const uint8_t VEHICLE_ID[4] = {0x01, 0x00, 0x00, 0x01};
   ```
3. Select board: Tools -> Board -> ESP32 Dev Module
4. Select port: Tools -> Port -> (your COM port)
5. Click Upload
6. Open Serial Monitor (115200 baud)
7. Verify "MPU6050 connected" message

### Receiver Unit

1. Open `receiver-unit/receiver-unit.ino`
2. Check LCD I2C address in `config.h` (usually 0x27 or 0x3F)
3. Select board and port
4. Click Upload
5. LCD should display "Monitoring..."

## Step 4: First Test (5 minutes)

### Test 1: System Power-On
- Vehicle unit: Green LED should be ON
- Receiver unit: LCD should show "Monitoring..."

### Test 2: Accident Detection
1. Tilt vehicle unit to 45° or more
2. Yellow and red LEDs should turn ON
3. Receiver should beep 3 times
4. LCD should show vehicle ID and "ROLLOVER"
5. RSSI bar LEDs should light up

### Test 3: Manual Override
1. Hold button on vehicle unit for 5 seconds
2. Yellow and red LEDs should turn OFF
3. Receiver alert should clear after 30 seconds

### Test 4: RF Range
1. Activate beacon on vehicle unit (tilt it)
2. Walk away with receiver unit
3. Note maximum distance for reliable reception
4. Target: 500m in open area

## Troubleshooting

### Vehicle Unit

**Green LED not on:**
- Check power supply (5V to ESP32)
- Verify MPU-6050 wiring

**No beacon transmission:**
- Check HC-12 power (needs 5V)
- Verify TX/RX pins not swapped

### Receiver Unit

**LCD blank:**
- Check I2C address (try 0x3F if 0x27 doesn't work)
- Adjust contrast pot on I2C backpack

**Not receiving packets:**
- Verify HC-12 powered
- Check TX/RX pins
- Test at close range first (< 10m)

## Next Steps

1. **Calibration**: Run `tests/calibration_tool.ino` to optimize thresholds
2. **Range Testing**: Use `tests/rf_range_test.ino` to measure actual range
3. **Field Testing**: Test in actual vehicle on rough terrain
4. **Documentation**: Read full manuals in `docs/` folder

## Safety Reminder

⚠️ This is a prototype system for academic purposes
⚠️ Do not rely solely on this for safety-critical applications
⚠️ Always carry backup communication devices
⚠️ Test thoroughly before field deployment

## Support

- Check `docs/` folder for detailed documentation
- Review `README.md` for project overview
- See `TESTING_GUIDE.md` for comprehensive testing procedures

---

**Congratulations!** Your Off-Highway Guardian system is now operational. Proceed to calibration and field testing for optimal performance.
