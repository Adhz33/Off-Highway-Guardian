# Testing & Validation Guide

## Phase II Testing - Prototype Validation

### Test 1: IMU Impact Detection

**Objective:** Verify impact detection at 3g threshold

**Setup:**
1. Mount vehicle unit on cushioned surface
2. Connect to Serial Monitor (115200 baud)
3. Prepare controlled drop test

**Procedure:**
1. Drop unit from 30cm height onto cushioned surface
2. Observe Serial Monitor for "Impact detected!" message
3. Verify beacon activation within 1 second
4. Check red LED illuminates
5. Repeat 10 times

**Pass Criteria:**
- 100% detection rate for drops
- Latency < 500ms from impact to detection
- No false positives during handling

**Results Log:**
```
Test #  | Drop Height | Detection | Latency | Beacon Active | Pass/Fail
--------|-------------|-----------|---------|---------------|----------
1       | 30cm        | Yes       | 320ms   | Yes           | Pass
2       | 30cm        | Yes       | 280ms   | Yes           | Pass
...
```

---

### Test 2: Rollover Detection

**Objective:** Verify rollover detection at 45° tilt

**Setup:**
1. Mount vehicle unit on adjustable tilt platform
2. Use protractor or digital inclinometer
3. Connect to Serial Monitor

**Procedure:**
1. Tilt platform to 30° - should NOT trigger
2. Tilt platform to 45° - should trigger
3. Tilt platform to 60° - should trigger
4. Return to level - beacon should remain active
5. Test all axes (pitch and roll)

**Pass Criteria:**
- No detection below 40°
- 100% detection at 45°+
- Sustained tilt > 200ms required

**Results Log:**
```
Angle | Axis  | Detection | Time to Detect | Pass/Fail
------|-------|-----------|----------------|----------
30°   | Pitch | No        | -              | Pass
45°   | Pitch | Yes       | 250ms          | Pass
60°   | Pitch | Yes       | 220ms          | Pass
45°   | Roll  | Yes       | 240ms          | Pass
```

---

### Test 3: Spin Detection

**Objective:** Verify angular velocity detection

**Setup:**
1. Mount vehicle unit on rotating platform
2. Use tachometer or video analysis for RPM measurement
3. Connect to Serial Monitor

**Procedure:**
1. Rotate slowly (< 180°/s) - should NOT trigger
2. Rotate rapidly (> 180°/s) - should trigger
3. Test all three axes
4. Verify multi-condition confirmation

**Pass Criteria:**
- No detection below threshold
- 100% detection above 180°/s
- Proper axis identification

---

### Test 4: Multi-Condition Confirmation

**Objective:** Verify false positive reduction via 2-of-3 condition logic

**Setup:**
1. Vehicle unit on test bench
2. Serial Monitor active
3. Controlled stimulus application

**Procedure:**
1. Apply only impact (shake) - should NOT trigger beacon
2. Apply only tilt (45°) - should NOT trigger beacon
3. Apply impact + tilt simultaneously - SHOULD trigger beacon
4. Apply impact + spin - SHOULD trigger beacon
5. Apply tilt + spin - SHOULD trigger beacon

**Pass Criteria:**
- Single condition does NOT activate beacon
- Any 2 conditions within 300ms window DOES activate beacon

**Results:**
```
Conditions Applied    | Beacon Activated | Pass/Fail
---------------------|------------------|----------
Impact only          | No               | Pass
Tilt only            | No               | Pass
Impact + Tilt        | Yes              | Pass
Impact + Spin        | Yes              | Pass
Tilt + Spin          | Yes              | Pass
```

---

### Test 5: RF Transmission Range

**Objective:** Verify 500m minimum range in open terrain

**Setup:**
1. Vehicle unit with beacon active
2. Receiver unit with operator
3. Open field or parking lot
4. Measuring tape or GPS for distance

**Procedure:**
1. Activate beacon on vehicle unit
2. Walk away with receiver in 50m increments
3. Record successful packet reception at each distance
4. Note RSSI values
5. Identify maximum reliable range

**Pass Criteria:**
- 100% packet reception at 500m
- RSSI readable for homing at 500m

**Results Log:**
```
Distance | Packets Sent | Packets Received | Success Rate | RSSI  | Pass/Fail
---------|--------------|------------------|--------------|-------|----------
50m      | 10           | 10               | 100%         | -45   | Pass
100m     | 10           | 10               | 100%         | -58   | Pass
200m     | 10           | 10               | 100%         | -72   | Pass
300m     | 10           | 10               | 100%         | -85   | Pass
500m     | 10           | 10               | 100%         | -98   | Pass
```

---

### Test 6: Manual Override Function

**Objective:** Verify false alarm cancellation

**Setup:**
1. Vehicle unit with active beacon
2. Override button accessible
3. Serial Monitor and LED observation

**Procedure:**
1. Trigger accident detection (tilt unit)
2. Verify beacon activates
3. Press and hold override button for 5 seconds
4. Verify beacon deactivates
5. Check LEDs turn off
6. Test accidental press (< 5 seconds) - should NOT cancel

**Pass Criteria:**
- 5-second hold successfully cancels beacon
- Presses < 5 seconds do NOT cancel
- Visual confirmation via LEDs

---

### Test 7: Power Supply Stability

**Objective:** Verify operation across 10V-14V range

**Setup:**
1. Variable DC power supply
2. Ammeter for current measurement
3. Vehicle unit connected
4. Serial Monitor for stability check

**Procedure:**
1. Set supply to 10V - verify normal operation
2. Set supply to 12V - verify normal operation
3. Set supply to 14V - verify normal operation
4. Measure current draw at each voltage
5. Check for resets or malfunctions

**Pass Criteria:**
- No resets or errors across voltage range
- Current draw < 500mA at 12V
- All functions operational

**Results:**
```
Voltage | Current | System Status | Pass/Fail
--------|---------|---------------|----------
10V     | 380mA   | Normal        | Pass
12V     | 420mA   | Normal        | Pass
14V     | 450mA   | Normal        | Pass
```

---

### Test 8: False Positive Rate - Rough Terrain

**Objective:** Verify < 5% false positive rate during normal driving

**Setup:**
1. Vehicle unit installed in test vehicle
2. Rough terrain course (unpaved road, speed bumps)
3. 30-minute test drive
4. Data logging enabled

**Procedure:**
1. Drive on rough terrain for 30 minutes
2. Include: potholes, speed bumps, sharp turns, sudden braking
3. Log all detection events
4. Count false alarms (no actual accident)
5. Calculate false positive rate

**Pass Criteria:**
- False positive rate < 5%
- No beacon activation during normal driving
- Thresholds properly calibrated

**Results:**
```
Duration: 30 minutes
Terrain: Unpaved forest road with potholes
Speed: 20-40 km/h

Total Detection Events: 2
False Positives: 0
False Positive Rate: 0%
Pass: Yes
```

---

## Phase III Testing - Field Validation

### Test 9: Forest Canopy RF Range

**Objective:** Measure RF range under dense forest canopy

**Setup:**
1. Forest location with dense canopy
2. Vehicle unit at fixed position
3. Receiver unit with operator
4. GPS for position tracking

**Procedure:**
1. Place vehicle unit in forest clearing
2. Activate beacon
3. Walk into forest in multiple directions
4. Record maximum reception distance
5. Map dead zones

**Expected Results:**
- Range reduction: 30-50% vs open terrain
- Typical range: 250-350m in forest
- Document areas requiring relay stations

---

### Test 10: RSSI Homing Accuracy

**Objective:** Verify rescue team can locate vehicle using RSSI

**Setup:**
1. Vehicle unit hidden in forest area
2. Receiver unit with rescue team
3. Stopwatch for time measurement
4. GPS for accuracy verification

**Procedure:**
1. Hide vehicle unit in forest (known location)
2. Rescue team starts 200m away
3. Use only RSSI indicator to navigate
4. Record time to locate vehicle
5. Measure final position accuracy

**Pass Criteria:**
- Vehicle located within 10 minutes
- Final position within 10m of actual location
- RSSI gradient clearly indicates direction

---

### Test 11: Multi-Vehicle Scenario

**Objective:** Verify receiver can handle multiple simultaneous alerts

**Setup:**
1. 3 vehicle units with different IDs
2. 1 receiver unit
3. Controlled test area

**Procedure:**
1. Activate beacon on Vehicle 1
2. Verify receiver displays Vehicle 1 alert
3. Activate beacon on Vehicle 2
4. Verify receiver updates to Vehicle 2
5. Check Serial Monitor logs both vehicles

**Expected Results:**
- Receiver displays most recent alert
- All alerts logged in Serial Monitor
- No packet collisions or data corruption

---

### Test 12: Environmental Stress Testing

**Objective:** Verify operation in extreme conditions

**Test 12A: Temperature**
- Cold: -10°C for 2 hours
- Hot: +60°C for 2 hours
- Verify normal operation after each

**Test 12B: Vibration**
- Mount on vibration table
- Simulate off-road driving (5-20 Hz)
- Duration: 1 hour
- Verify no false positives

**Test 12C: Moisture**
- Spray water on enclosure
- Verify IP rating
- Check for condensation inside

---

## Acceptance Test Checklist

Before final demonstration:

- [ ] IMU detects impact within 500ms
- [ ] IMU detects rollover at 45°+ tilt
- [ ] RF beacon activates within 1 second
- [ ] RF range ≥ 500m in open terrain
- [ ] Receiver correctly decodes all packets
- [ ] RSSI homing functional
- [ ] Manual override works (5-second hold)
- [ ] No false positives in 30-min rough terrain drive
- [ ] System stable on 10-14V supply
- [ ] All LEDs and indicators functional
- [ ] Documentation complete
- [ ] Code commented and clean

---

## Test Data Recording Template

```
Test Date: __________
Test Location: __________
Tester Name: __________
Hardware Version: __________
Firmware Version: __________

Test Results:
[Detailed observations]

Issues Found:
[List any problems]

Recommendations:
[Suggested improvements]

Overall Status: PASS / FAIL
```

---

## Calibration Procedure

After testing, if adjustments needed:

1. **Increase Impact Threshold** (if too many false positives):
   ```cpp
   #define IMPACT_THRESHOLD_G 4.0  // Increase from 3.0
   ```

2. **Adjust Rollover Angle** (if triggering on normal slopes):
   ```cpp
   #define ROLLOVER_ANGLE_DEG 50.0  // Increase from 45.0
   ```

3. **Modify Confirmation Window** (if missing real accidents):
   ```cpp
   #define CONFIRMATION_WINDOW_MS 500  // Increase from 300
   ```

4. **Re-test after each calibration change**

---

## Safety During Testing

⚠️ **Important Safety Guidelines:**

1. Never test in moving vehicle on public roads
2. Use controlled test environments only
3. Have emergency communication backup
4. Inform others of test location and duration
5. Wear safety equipment during field tests
6. Do not rely on prototype for actual safety
