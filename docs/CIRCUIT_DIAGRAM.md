# Circuit Diagrams & Schematics

## Vehicle Unit - Complete Circuit

```
                                    VEHICLE UNIT
                                    
                    +12V (Vehicle Battery)
                         |
                         |
                    [FUSE 5A]
                         |
                         |
                    +----+----+
                    |         |
                [TVS Diode]  |
                    |         |
                   GND    [LM2596 Buck Converter]
                              |
                         +----+----+
                         |         |
                        5V        GND
                         |         |
              +----------+----------+----------+
              |          |          |          |
              |          |          |          |
         [ESP32]    [HC-12 RF]  [MPU-6050]  [LEDs]
              |          |          |          |
             GND        GND        GND        GND


Detailed Connections:

ESP32 DevKit:
  VIN  -------- 5V (from buck converter)
  GND  -------- GND (common ground)
  GPIO 21 (SDA) -------- MPU-6050 SDA
  GPIO 22 (SCL) -------- MPU-6050 SCL
  GPIO 16 (RX2) -------- HC-12 TX
  GPIO 17 (TX2) -------- HC-12 RX
  GPIO 2  -------- LED1 (System Active - Green) ----[220Ω]---- GND
  GPIO 4  -------- LED2 (Accident Detected - Yellow) ----[220Ω]---- GND
  GPIO 5  -------- LED3 (Beacon Active - Red) ----[220Ω]---- GND
  GPIO 15 -------- Push Button -------- GND (internal pull-up enabled)

MPU-6050 (GY-521):
  VCC  -------- 3.3V (from ESP32 3.3V pin)
  GND  -------- GND
  SDA  -------- ESP32 GPIO 21
  SCL  -------- ESP32 GPIO 22
  XDA  -------- (not connected)
  XCL  -------- (not connected)
  AD0  -------- GND (I2C address 0x68)
  INT  -------- (not connected, optional)

HC-12 RF Module:
  VCC  -------- 5V (from buck converter)
  GND  -------- GND
  TX   -------- ESP32 GPIO 16 (RX2)
  RX   -------- ESP32 GPIO 17 (TX2)
  SET  -------- (not connected, default mode)

Power Supply:
  Vehicle 12V+ ----[FUSE 5A]----[TVS Diode]---- LM2596 IN+
  Vehicle GND  -------------------- LM2596 IN-
  LM2596 OUT+ -------- 5V rail
  LM2596 OUT- -------- GND rail
  
  Add capacitors:
  - 100µF electrolytic across LM2596 input (12V side)
  - 100µF electrolytic across LM2596 output (5V side)
  - 0.1µF ceramic across ESP32 VIN and GND
```

---

## Receiver Unit - Complete Circuit

```
                                    RECEIVER UNIT
                                    
                         +5V (USB Power Bank)
                              |
                              |
                    +---------+---------+
                    |         |         |
                    |         |         |
               [ESP32]   [HC-12 RF]  [LCD I2C]
                    |         |         |
                   GND       GND       GND


Detailed Connections:

ESP32 DevKit:
  VIN  -------- 5V (from USB power bank)
  GND  -------- GND (common ground)
  GPIO 21 (SDA) -------- LCD I2C SDA
  GPIO 22 (SCL) -------- LCD I2C SCL
  GPIO 16 (RX2) -------- HC-12 TX
  GPIO 17 (TX2) -------- HC-12 RX
  GPIO 2  -------- LED (Signal - Blue) ----[220Ω]---- GND
  GPIO 4  -------- LED (Alert - Red) ----[220Ω]---- GND
  GPIO 5  -------- Buzzer (+) -------- GND (-)
  GPIO 12 -------- RSSI LED 1 ----[220Ω]---- GND
  GPIO 13 -------- RSSI LED 2 ----[220Ω]---- GND
  GPIO 14 -------- RSSI LED 3 ----[220Ω]---- GND
  GPIO 27 -------- RSSI LED 4 ----[220Ω]---- GND
  GPIO 26 -------- RSSI LED 5 ----[220Ω]---- GND

16x2 LCD with I2C Backpack:
  VCC  -------- 5V
  GND  -------- GND
  SDA  -------- ESP32 GPIO 21
  SCL  -------- ESP32 GPIO 22

HC-12 RF Module:
  VCC  -------- 5V
  GND  -------- GND
  TX   -------- ESP32 GPIO 16 (RX2)
  RX   -------- ESP32 GPIO 17 (TX2)
  SET  -------- (not connected)

Active Buzzer:
  (+)  -------- ESP32 GPIO 5
  (-)  -------- GND

RSSI Bar Indicator (5 LEDs):
  LED 1-5 Anodes -------- ESP32 GPIOs (12,13,14,27,26)
  LED 1-5 Cathodes ----[220Ω]---- GND
  
  Arrange LEDs in horizontal bar for visual signal strength
```

---

## PCB Layout Recommendations

### Vehicle Unit PCB (Single Layer)

```
Top View (Component Side):

+------------------------------------------+
|  [12V IN] [GND]                          |
|                                          |
|  [Buck Converter Module]                 |
|                                          |
|  +5V Rail ========================       |
|                                          |
|  [ESP32 DevKit]        [HC-12 Module]    |
|  (Socket Mount)        (Socket Mount)    |
|                                          |
|  [MPU-6050]                              |
|  (Socket Mount)                          |
|                                          |
|  [LED1] [LED2] [LED3]  [Button]          |
|                                          |
|  GND Rail ========================       |
|                                          |
+------------------------------------------+

Dimensions: 10cm x 7cm
Mounting: 4x M3 holes in corners
```

### Receiver Unit PCB (Single Layer)

```
Top View (Component Side):

+------------------------------------------+
|  [USB IN]                                |
|                                          |
|  [ESP32 DevKit]        [HC-12 Module]    |
|  (Socket Mount)        (Socket Mount)    |
|                                          |
|  [LCD 16x2 with I2C]                     |
|  (Header Mount)                          |
|                                          |
|  [LED1] [LED2]  [Buzzer]                 |
|                                          |
|  [RSSI Bar: LED3 LED4 LED5 LED6 LED7]    |
|                                          |
+------------------------------------------+

Dimensions: 12cm x 8cm
Mounting: 4x M3 holes in corners
```

---

## Enclosure Design

### Vehicle Unit Enclosure

```
Material: ABS plastic, IP65 rated
Dimensions: 12cm x 9cm x 5cm

Features:
- Cable gland for 12V power input
- Transparent window for LED visibility
- Rubber gasket for waterproofing
- Mounting tabs for vehicle installation
- Ventilation slots (covered with mesh)

Mounting:
- 4x M4 bolts to vehicle chassis
- Vibration-damping rubber washers
```

### Receiver Unit Enclosure

```
Material: ABS plastic with belt clip
Dimensions: 14cm x 10cm x 4cm

Features:
- LCD cutout on front face
- LED bar visible on top edge
- Buzzer grille on side
- USB port access for charging
- Belt clip or lanyard attachment
- Rubberized edges for drop protection
```

---

## Antenna Placement

### Vehicle Unit
- Mount antenna vertically on vehicle roof or roll cage
- Use magnetic mount or permanent bracket
- Keep antenna away from metal surfaces (>5cm clearance)
- Route coax cable through rubber grommet

### Receiver Unit
- Integrated spring antenna on PCB
- Vertical orientation for best reception
- Keep away from body when using (hold at arm's length)

---

## Power Consumption Analysis

### Vehicle Unit

| Component | Voltage | Current (Typical) | Current (Peak) |
|-----------|---------|-------------------|----------------|
| ESP32 | 5V | 80mA | 240mA (WiFi off) |
| MPU-6050 | 3.3V | 3.5mA | 3.5mA |
| HC-12 (RX) | 5V | 22mA | 22mA |
| HC-12 (TX) | 5V | - | 100mA |
| LEDs (3x) | 5V | 30mA | 30mA |
| **Total** | **12V** | **~150mA** | **~400mA** |

**Power at 12V:** ~1.8W typical, ~4.8W peak  
**Daily consumption (24h):** ~43Wh (3.6Ah at 12V)

### Receiver Unit

| Component | Voltage | Current (Typical) | Current (Peak) |
|-----------|---------|-------------------|----------------|
| ESP32 | 5V | 80mA | 240mA |
| HC-12 (RX) | 5V | 22mA | 22mA |
| LCD Backlight | 5V | 50mA | 50mA |
| LEDs (7x) | 5V | 70mA | 70mA |
| Buzzer | 5V | - | 30mA (active) |
| **Total** | **5V** | **~220mA** | **~410mA** |

**10,000mAh power bank runtime:** ~45 hours continuous

---

## Troubleshooting - Hardware Issues

### Issue: ESP32 not powering on
- Check buck converter output voltage (should be 5V)
- Verify fuse not blown
- Check for short circuits on power rails

### Issue: MPU-6050 not detected
- Verify I2C connections (SDA/SCL not swapped)
- Check 3.3V supply to MPU-6050
- Try different I2C address (AD0 pin high/low)

### Issue: HC-12 not transmitting
- Verify 5V supply (HC-12 requires 5V, not 3.3V)
- Check TX/RX not swapped
- Ensure antenna connected
- Test with AT commands

### Issue: LCD not displaying
- Check I2C address (0x27 or 0x3F)
- Verify I2C connections
- Adjust contrast potentiometer on I2C backpack
- Test with I2C scanner sketch

---

## Safety & Regulatory Notes

⚠️ **Electrical Safety:**
- Use proper fuse rating (5A recommended)
- Include TVS diode for voltage spike protection
- Ensure proper wire gauge for current capacity
- Double-check polarity before powering on

⚠️ **RF Safety:**
- 433MHz ISM band: 10mW max power in India
- Keep antenna away from body during transmission
- Follow WPC regulations for RF emissions

⚠️ **Automotive Installation:**
- Connect to switched 12V (ignition-controlled)
- Use proper automotive-grade connectors
- Secure all wiring to prevent chafing
- Keep away from airbag deployment zones
