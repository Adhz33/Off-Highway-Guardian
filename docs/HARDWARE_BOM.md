# Bill of Materials (BOM)

## Vehicle Unit - Per Unit Cost Estimate

| Component | Specification | Quantity | Unit Price (INR) | Total (INR) |
|-----------|--------------|----------|------------------|-------------|
| ESP32 DevKit | ESP32-WROOM-32 | 1 | 450 | 450 |
| MPU-6050 | GY-521 6-DOF IMU | 1 | 150 | 150 |
| HC-12 RF Module | 433MHz, 1000m range | 1 | 350 | 350 |
| Buck Converter | LM2596 12V to 5V | 1 | 80 | 80 |
| LEDs | 5mm, assorted colors | 3 | 5 | 15 |
| Push Button | Tactile switch | 1 | 10 | 10 |
| Resistors | 220Ω, 10kΩ | 5 | 2 | 10 |
| PCB/Breadboard | Prototyping board | 1 | 100 | 100 |
| Enclosure | Waterproof ABS box | 1 | 200 | 200 |
| Wires & Connectors | Jumper wires, terminals | - | 100 | 100 |
| Antenna | 433MHz spring antenna | 1 | 50 | 50 |
| **Total per Vehicle Unit** | | | | **₹1,515** |

## Receiver Unit - Per Unit Cost Estimate

| Component | Specification | Quantity | Unit Price (INR) | Total (INR) |
|-----------|--------------|----------|------------------|-------------|
| ESP32 DevKit | ESP32-WROOM-32 | 1 | 450 | 450 |
| HC-12 RF Module | 433MHz, 1000m range | 1 | 350 | 350 |
| LCD Display | 16x2 with I2C backpack | 1 | 200 | 200 |
| LEDs | 5mm, assorted colors | 7 | 5 | 35 |
| Buzzer | Active 5V buzzer | 1 | 30 | 30 |
| Resistors | 220Ω | 7 | 2 | 14 |
| PCB/Breadboard | Prototyping board | 1 | 100 | 100 |
| Enclosure | Portable ABS case | 1 | 250 | 250 |
| Power Bank | 10000mAh USB (portable) | 1 | 600 | 600 |
| Wires & Connectors | Jumper wires, terminals | - | 100 | 100 |
| Antenna | 433MHz spring antenna | 1 | 50 | 50 |
| **Total per Receiver Unit** | | | | **₹2,179** |

## Complete System Cost (1 Vehicle + 1 Receiver)

| Item | Cost (INR) |
|------|------------|
| Vehicle Unit | ₹1,515 |
| Receiver Unit | ₹2,179 |
| **Total System** | **₹3,694** |

## Scalability - Fleet Deployment

### Scenario: 10 Vehicles + 3 Receiver Units

| Item | Quantity | Unit Cost | Total Cost |
|------|----------|-----------|------------|
| Vehicle Units | 10 | ₹1,515 | ₹15,150 |
| Receiver Units (Check Posts) | 2 | ₹2,179 | ₹4,358 |
| Receiver Units (Portable) | 1 | ₹2,179 | ₹2,179 |
| **Total Fleet Cost** | | | **₹21,687** |

**Per Vehicle Cost in Fleet:** ₹2,169 (including shared receivers)

## Alternative Components (Cost Optimization)

### Budget Option - Arduino Nano Based

| Component Change | Original | Alternative | Savings |
|------------------|----------|-------------|---------|
| ESP32 -> Arduino Nano | ₹450 | ₹180 | ₹270 |
| HC-12 -> 433MHz Basic | ₹350 | ₹200 | ₹150 |
| **Total Savings per Unit** | | | **₹420** |

**Budget Vehicle Unit Cost:** ₹1,095  
**Budget Receiver Unit Cost:** ₹1,759

### Premium Option - LoRa Based (Extended Range)

| Component Change | Original | Alternative | Additional Cost |
|------------------|----------|-------------|-----------------|
| HC-12 -> LoRa SX1278 | ₹350 | ₹600 | ₹250 |
| Range improvement | 1km | 3-5km | - |
| **Additional Cost per Unit** | | | **₹250** |

**Premium Vehicle Unit Cost:** ₹1,765  
**Premium Receiver Unit Cost:** ₹2,429

## Development Tools (One-Time Cost)

| Tool | Purpose | Cost (INR) |
|------|---------|------------|
| USB-TTL Programmer | Firmware upload | 150 |
| Multimeter | Testing & debugging | 500 |
| Oscilloscope (optional) | Signal analysis | 3,000 |
| Soldering Kit | PCB assembly | 800 |
| **Total Development Tools** | | **₹4,450** |

## Notes

1. Prices are approximate (India market, 2026)
2. Bulk purchase discounts available for fleet deployment
3. PCB fabrication cost reduces with volume (₹100 -> ₹30 per board at 50+ units)
4. Enclosures can be 3D printed to reduce cost
5. All components available on Amazon, Robu.in, or local electronics markets

## Regulatory Compliance

- 433MHz ISM band: License-exempt in India (WPC regulations)
- No certification required for prototype/academic use
- For commercial deployment: WPC type approval required (~₹50,000 one-time)
