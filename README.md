# Off-Highway Guardian

Autonomous Accident Detection & Emergency Alert System for GPS-Denied Off-Highway Environments

## Project Overview

Off-Highway Guardian is an embedded safety system for forest and off-road vehicles that detects accidents and issues emergency alerts without relying on GPS, cellular, or satellite infrastructure.

## System Components

1. **Vehicle Unit** - IMU-based accident detection with RF beacon transmitter
2. **Rescue Receiver Unit** - RF receiver with RSSI homing capability

## Hardware Requirements

### Vehicle Unit
- Microcontroller: ESP32 DevKit / Arduino Nano / STM32
- IMU Sensor: MPU-6050 (6-DOF)
- RF Module: HC-12 (433MHz) or LoRa SX1278
- Power: 12V to 5V/3.3V buck converter
- Status LEDs and manual override button

### Rescue Receiver Unit
- Microcontroller: ESP32 DevKit / Arduino Nano
- RF Module: HC-12 (433MHz) or LoRa SX1278
- Display: 16x2 LCD or OLED
- RSSI indicator (LED bar or analog meter)

## Project Structure

```
off-highway-guardian/
├── vehicle-unit/          # Vehicle-mounted detection & beacon firmware
├── receiver-unit/         # Rescue receiver firmware
├── common/                # Shared libraries and protocols
├── docs/                  # Documentation and schematics
└── tests/                 # Test scripts and calibration tools
```

## Quick Start

See individual README files in `vehicle-unit/` and `receiver-unit/` directories for setup and flashing instructions.

## Development Team

Group 5 - CSA Semester 6, Sahrdaya College of Engineering & Technology
