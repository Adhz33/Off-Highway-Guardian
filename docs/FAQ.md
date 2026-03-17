# Frequently Asked Questions (FAQ)

## General Questions

### What is Off-Highway Guardian?

Off-Highway Guardian is an autonomous accident detection and emergency alert system designed for vehicles operating in GPS-denied environments like dense forests and remote off-road terrain. It uses an IMU sensor to detect accidents and an RF beacon to broadcast emergency alerts to nearby rescue units.

### Why not use GPS or cellular networks?

GPS signals are blocked or heavily degraded under dense forest canopy, and cellular networks are typically unavailable in remote forest areas. Our system operates completely offline and infrastructure-independent, making it ideal for these challenging environments.

### How much does it cost?

Approximately ₹3,694 for a complete system (1 vehicle unit + 1 receiver unit). For fleet deployment, the per-vehicle cost drops to around ₹2,169 when receivers are shared across multiple vehicles.

### Is this a commercial product?

No, this is an academic prototype developed for CSD 334 project at Sahrdaya College of Engineering & Technology. It is not certified for commercial use or safety-critical applications.

## Technical Questions

### What accidents can the system detect?

The system detects three types of accidents:
1. **Impact** - Sudden acceleration changes (>3g)
2. **Rollover** - Vehicle tilt beyond normal angles (>45°)
3. **Spin** - Loss of control with rapid rotation (>180°/s)

### How does it prevent false positives?

The system uses multi-condition confirmation: at least 2 of 3 conditions (impact, rollover, spin) must be met within a 300ms window. This significantly reduces false alarms from normal rough terrain driving.

### What is the RF range?

Target range is 500m minimum in open terrain. Actual range depends on:
- Terrain and obstacles
- Forest canopy density
- Antenna quality and placement
- RF module power output

In dense forest, expect 250-350m range (30-50% reduction).

### Can it work with multiple vehicles?

Yes! Each vehicle has a unique ID, and receiver units can monitor multiple vehicles simultaneously. The LCD displays the most recent alert, while all alerts are logged to the serial monitor.

### What happens if the driver is okay?

The driver can cancel the beacon by holding the manual override button for 5 seconds. This prevents unnecessary rescue dispatches for false alarms.

### How long does the beacon transmit?

The beacon transmits continuously (every 5 seconds) until:
- Manually cancelled by the driver (5-second button hold)
- Power is lost
- Optional auto-shutoff timeout (configurable, default: disabled)

## Hardware Questions

### Can I use Arduino instead of ESP32?

Yes! The code is compatible with Arduino Nano/Uno. However, ESP32 is recommended because:
- More GPIO pins available
- Better processing power
- Built-in dual UART for RF module
- Future expansion capabilities (WiFi, Bluetooth)

### What RF module should I use?

Recommended options:
1. **HC-12** (433MHz) - Budget-friendly, 1km range, easy to use
2. **LoRa SX1278** - Premium option, 3-5km range, better penetration
3. Any 433MHz or 868MHz ISM band module

### Why 433MHz frequency?

433MHz is a license-exempt ISM band in India (per WPC regulations). It also provides better obstacle penetration than higher frequencies, making it ideal for forest environments.

### Can I power it from USB?

Vehicle unit requires 12V vehicle power (converted to 5V). Receiver unit can run on USB power bank for portability.

### How long does the receiver battery last?

With a 10,000mAh power bank, expect 40-45 hours of continuous operation.

### Do I need to solder everything?

For prototyping, breadboards work fine. For field deployment, soldering is strongly recommended for reliability and vibration resistance.

## Installation Questions

### Where should I mount the IMU?

Mount rigidly to the vehicle chassis (not on flexible panels). Orient with Z-axis vertical when vehicle is level. Avoid locations near:
- Engine vibration sources
- Heat sources (exhaust, radiator)
- Airbag deployment zones

### How do I find the LCD I2C address?

Run an I2C scanner sketch (provided in receiver README). Common addresses are 0x27 or 0x3F.

### Can I install it myself?

Yes, if you have basic electronics knowledge. The installation guide provides step-by-step instructions. Allow 30-45 minutes per vehicle unit.

### Does it need professional calibration?

No, the included calibration tool guides you through the process. Simply drive normally for 30 seconds while the tool records baseline data.

## Operation Questions

### How do I know the system is working?

Vehicle unit: Green "System Active" LED should be ON when vehicle is powered.
Receiver unit: LCD displays "Monitoring..." when operational.

### What do the LEDs mean?

**Vehicle Unit:**
- Green: System active and monitoring
- Yellow: Accident detected
- Red: Beacon transmitting

**Receiver Unit:**
- Blue: Signal received (blinks)
- Red: Active alert
- 5 LEDs: RSSI signal strength bar

### How do rescue teams locate the vehicle?

Using RSSI (Received Signal Strength Indicator) homing:
1. Note RSSI value on receiver LCD
2. Walk in different directions
3. RSSI increases as you get closer
4. Follow increasing signal strength to accident site

### What if multiple vehicles have accidents?

The receiver displays the most recent alert. All alerts are logged to the serial monitor for review. Future versions will implement alert queuing.

### Can the system be hacked or spoofed?

Basic security is provided via vehicle ID and checksum validation. Optional AES-128 encryption can be enabled in config.h for enhanced security (requires additional implementation).

## Troubleshooting Questions

### Why am I getting false alarms?

Common causes:
1. Thresholds too sensitive - increase IMPACT_THRESHOLD_G
2. Loose IMU mounting - ensure rigid attachment
3. Extreme terrain - recalibrate for your specific conditions

### The receiver isn't receiving packets. Why?

Check:
1. RF module powered (needs 5V, not 3.3V)
2. TX/RX pins not swapped
3. Both units using same RF frequency
4. Antenna connected
5. Test at close range first (< 10m)

### LCD is blank. What's wrong?

1. Check I2C address (try 0x3F if 0x27 doesn't work)
2. Verify I2C wiring (SDA/SCL)
3. Adjust contrast potentiometer on I2C backpack
4. Verify 5V power supply

### System keeps resetting. Help!

1. Check power supply stability (add 100µF capacitor)
2. Verify voltage regulator output
3. Check for short circuits
4. Ensure adequate current capacity (500mA minimum)

### RSSI always shows the same value. Why?

Current implementation uses simulated RSSI for testing. To use real RSSI, set `RSSI_SIMULATED = false` in receiver config.h and implement RF module RSSI query (see API_REFERENCE.md).

## Calibration Questions

### How often should I calibrate?

- Initial installation: Required
- After threshold changes: Required
- After moving to different terrain: Recommended
- Routine maintenance: Every 3 months

### What if calibration recommends very high thresholds?

This indicates your normal driving is very rough. Options:
1. Accept higher thresholds (may miss some accidents)
2. Improve vehicle suspension
3. Drive more smoothly during calibration
4. Use multi-condition confirmation (already enabled)

### Can I use the same thresholds for all vehicles?

Not recommended. Different vehicles have different characteristics. Calibrate each vehicle individually for best results.

## Deployment Questions

### How many receiver units do I need?

Depends on your deployment:
- Small area (< 1km²): 1-2 receivers
- Medium area (1-5km²): 3-5 receivers
- Large area (> 5km²): Plan check posts every 500m

### Can I extend the range?

Yes, several options:
1. Use LoRa modules (3-5km range)
2. Implement relay stations
3. Use higher-gain antennas
4. Increase RF module power (within legal limits)

### What about regulatory compliance?

For academic/prototype use: No certification required
For commercial deployment: WPC type approval needed (~₹50,000 one-time cost)

### How do I train operators?

Training programs are provided in DEPLOYMENT_GUIDE.md:
- Vehicle operators: 15 minutes
- Patrol officers: 30 minutes
- Check post personnel: 20 minutes

## Maintenance Questions

### What maintenance is required?

**Daily:** Visual inspection of LEDs
**Weekly:** Test manual override, verify receiver battery
**Monthly:** Clean enclosures, verify connections, test RF range
**Quarterly:** Full system test, recalibrate if needed

### How long will the components last?

Expected lifespan:
- Electronics: 3-5 years
- LEDs: 50,000+ hours
- Enclosures: 5+ years
- Batteries (receiver): 300-500 charge cycles

### Can I replace individual components?

Yes! The modular design allows individual component replacement without replacing the entire system.

## Future Enhancement Questions

### Will there be a mobile app?

Planned for future phases. Current focus is on core functionality and reliability.

### Can it integrate with existing forest department systems?

Yes, the serial output can be interfaced with external systems. Integration protocols can be developed based on specific requirements.

### What about GPS integration?

GPS can be added when available (e.g., in clearings). The system will automatically use GPS data if present, falling back to RF-only in GPS-denied areas.

### Will you add voice communication?

Possible future enhancement. Current system focuses on automated detection and alerting.

## Support Questions

### Where can I get help?

1. Check documentation in `docs/` folder
2. Review code comments in source files
3. Contact project team (see README.md)
4. Consult project guide: Dr. Raju G

### Can I modify the code?

Yes! The project is open-source (MIT License). Modifications are encouraged. Please share improvements with the community.

### How do I report bugs?

Document the issue with:
- Hardware configuration
- Firmware version
- Steps to reproduce
- Expected vs actual behavior
- Serial monitor output

Contact project team with this information.

### Can I use this for my own project?

Yes, under MIT License terms. Attribution to original team is appreciated but not required.

---

**Still have questions?** Check the comprehensive documentation in the `docs/` folder or contact the project team.

**Document Version:** 1.0  
**Last Updated:** January 2026
