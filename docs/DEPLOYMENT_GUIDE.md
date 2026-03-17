# Deployment Guide

## Pre-Deployment Checklist

### Hardware Verification
- [ ] All components tested individually
- [ ] Vehicle unit passes all Phase II tests
- [ ] Receiver unit passes all Phase II tests
- [ ] RF range verified in target environment
- [ ] Power supply stable across voltage range
- [ ] Enclosures waterproof and secure
- [ ] All connections soldered (no breadboard)
- [ ] Firmware uploaded and configured

### Documentation
- [ ] Vehicle IDs documented and assigned
- [ ] Installation manual provided to operators
- [ ] Emergency response procedures defined
- [ ] Maintenance schedule established
- [ ] Contact information for technical support

## Fleet Deployment Strategy

### Phase 1: Pilot Deployment (2-3 vehicles)
1. Select vehicles for pilot program
2. Install vehicle units
3. Provide 2 receiver units (1 check post, 1 portable)
4. Train operators and patrol officers
5. Monitor for 2 weeks
6. Collect feedback and adjust thresholds

### Phase 2: Gradual Rollout (remaining fleet)
1. Install 3-5 vehicles per week
2. Ensure adequate receiver coverage
3. Continue training program
4. Monitor false positive rates
5. Document any issues

### Phase 3: Full Operation
1. All vehicles equipped
2. Regular maintenance schedule active
3. Incident response procedures tested
4. Performance metrics tracked

## Installation Procedure

### Vehicle Unit Installation (30-45 minutes per vehicle)

1. **Pre-Installation**
   - Read vehicle manual for electrical system
   - Identify 12V switched power source (ignition-controlled)
   - Plan wire routing to avoid heat/moving parts
   - Prepare tools: screwdriver, wire strippers, multimeter

2. **IMU Mounting**
   - Select rigid mounting location on chassis
   - Avoid flexible panels or plastic trim
   - Orient with Z-axis vertical when vehicle level
   - Use vibration-damping washers
   - Secure with M4 bolts

3. **Power Connection**
   - Locate 12V switched power (fuse box)
   - Install 5A inline fuse
   - Connect to buck converter input
   - Verify 5V output with multimeter
   - Connect to vehicle unit

4. **Control Box Mounting**
   - Mount in driver-visible location
   - Ensure LED visibility
   - Position override button within reach
   - Secure enclosure

5. **Antenna Installation**
   - Mount on roof or roll cage
   - Vertical orientation
   - Route cable through rubber grommet
   - Connect to RF module

6. **Testing**
   - Power on vehicle
   - Verify green "System Active" LED
   - Check Serial Monitor for "MPU6050 connected"
   - Test manual override button
   - Simulate tilt to trigger alert
   - Verify beacon transmission with receiver

7. **Documentation**
   - Record vehicle ID
   - Note installation date
   - Photograph installation
   - Update fleet inventory

### Receiver Unit Setup (15 minutes per unit)

1. **Check Post Installation**
   - Mount on desk or wall bracket
   - Connect to 5V power adapter
   - Position LCD for operator visibility
   - Test with vehicle beacon

2. **Portable Unit Setup**
   - Charge power bank fully
   - Attach belt clip or lanyard
   - Provide protective case
   - Train officer on RSSI homing

## Operator Training Program

### Vehicle Operators (15 minutes)
- System overview and purpose
- LED indicator meanings
- Manual override procedure (5-second hold)
- What to do if alert triggers
- Reporting false alarms

### Patrol Officers (30 minutes)
- Receiver unit operation
- Alert interpretation
- RSSI homing technique
- Response procedures
- Communication protocols

### Check Post Personnel (20 minutes)
- Monitoring procedures
- Alert logging
- Dispatch coordination
- System troubleshooting basics

## Maintenance Schedule

### Daily
- Visual inspection of LEDs (system active)
- Check receiver unit battery level

### Weekly
- Test manual override on each vehicle
- Verify receiver units functional
- Check for loose connections

### Monthly
- Clean enclosures and connectors
- Verify antenna connections
- Test RF range
- Review false positive logs
- Update firmware if needed

### Quarterly
- Full system test on each vehicle
- Recalibrate thresholds if needed
- Replace worn components
- Review incident logs
- Operator refresher training

## Troubleshooting in Field

### Vehicle Unit Issues

**System Active LED not on:**
- Check vehicle power (ignition on)
- Verify fuse not blown
- Check buck converter output

**False alarms during normal driving:**
- Increase IMPACT_THRESHOLD_G in firmware
- Check IMU mounting (must be rigid)
- Recalibrate using calibration tool

**Beacon not transmitting:**
- Check RF module power
- Verify antenna connection
- Test with receiver at close range

### Receiver Unit Issues

**Not receiving alerts:**
- Check power bank charge
- Verify RF module powered
- Test at closer range
- Check antenna

**LCD not displaying:**
- Adjust contrast potentiometer
- Check I2C connections
- Verify power supply

## Emergency Response Procedures

### When Alert Received at Check Post

1. **Immediate Actions (0-2 minutes)**
   - Note vehicle ID from LCD
   - Record time of alert
   - Sound alarm to alert rescue team
   - Check vehicle last known location (logbook)

2. **Dispatch (2-5 minutes)**
   - Assign rescue team with portable receiver
   - Provide vehicle ID and last location
   - Ensure communication equipment ready
   - Alert medical support if available

3. **En Route (5-30 minutes)**
   - Rescue team monitors RSSI on portable receiver
   - Follow increasing signal strength
   - Maintain radio contact with check post
   - Prepare rescue equipment

4. **On Scene (30+ minutes)**
   - Locate vehicle using RSSI homing
   - Assess situation
   - Provide first aid if needed
   - Report back to check post
   - Arrange evacuation if required

5. **Post-Incident**
   - Document incident details
   - Review system logs
   - Verify system functioned correctly
   - Update procedures if needed

### False Alarm Procedure

If driver is uninjured and alert was false:
1. Driver holds override button for 5 seconds
2. Beacon cancels
3. Driver reports false alarm to check post
4. Incident logged for threshold adjustment

## Performance Monitoring

### Metrics to Track

| Metric | Target | Review Frequency |
|--------|--------|------------------|
| False Positive Rate | < 5% | Weekly |
| Alert Response Time | < 15 min | Per incident |
| System Uptime | > 99% | Monthly |
| RF Range (actual) | > 500m | Quarterly |
| Battery Life (receiver) | > 40 hours | Monthly |

### Incident Log Template

```
Date: __________
Time: __________
Vehicle ID: __________
Alert Type: Impact / Rollover / Spin / Combined
Location: __________
Response Time: __________
Actual Accident: Yes / No (False Positive)
Injuries: Yes / No
System Performance: Satisfactory / Issues
Notes: __________
```

## Scaling Considerations

### Adding More Vehicles
- Ensure unique vehicle IDs
- Verify receiver coverage adequate
- Update fleet inventory
- Train new operators

### Adding More Receivers
- Configure same RF frequency
- Train additional personnel
- Update communication protocols
- Test coverage overlap

### Geographic Expansion
- Map RF coverage in new areas
- Identify dead zones
- Plan check post locations
- Consider relay stations if needed

## Regulatory Compliance

### India - WPC Regulations
- 433MHz ISM band: License-exempt
- Max power: 10mW
- No certification required for prototype
- For commercial: WPC type approval needed

### Documentation Required
- System description
- Technical specifications
- Test reports
- User manual
- Maintenance records

## Support & Maintenance Contacts

**Technical Support:**
- Team: Group 5, CSA Semester 6
- Email: [project email]
- Phone: [contact number]

**Emergency Escalation:**
- Project Guide: Dr. Raju G
- Department: CSE, Sahrdaya College

**Spare Parts:**
- Local electronics suppliers
- Online: Robu.in, Amazon India
- Lead time: 3-5 days

## Future Enhancements

### Phase IV+ Roadmap
1. Event logging to SD card
2. GPS integration (when available)
3. Multi-hop RF relay network
4. Solar charging for receiver units
5. Mobile app for fleet monitoring
6. Integration with forest dept. systems

---

**Document Version:** 1.0  
**Last Updated:** January 2026  
**Next Review:** After pilot deployment
