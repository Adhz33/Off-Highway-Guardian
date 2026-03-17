# Contributing to Off-Highway Guardian

Thank you for your interest in improving Off-Highway Guardian! This document provides guidelines for contributing to the project.

## How to Contribute

### Reporting Issues

If you find a bug or have a suggestion:

1. Check existing issues to avoid duplicates
2. Create a new issue with:
   - Clear, descriptive title
   - Detailed description of the problem/suggestion
   - Steps to reproduce (for bugs)
   - Expected vs actual behavior
   - Hardware configuration
   - Firmware version
   - Serial monitor output (if applicable)

### Code Contributions

1. **Fork the repository**
2. **Create a feature branch**
   ```bash
   git checkout -b feature/your-feature-name
   ```
3. **Make your changes**
   - Follow existing code style
   - Add comments for complex logic
   - Update documentation if needed
4. **Test thoroughly**
   - Test on actual hardware
   - Verify no regressions
   - Document test results
5. **Commit with clear messages**
   ```bash
   git commit -m "Add feature: brief description"
   ```
6. **Push and create pull request**
   ```bash
   git push origin feature/your-feature-name
   ```

## Code Style Guidelines

### C/C++ Code

- Use 4 spaces for indentation (no tabs)
- Opening braces on same line
- Clear variable names (no single letters except loop counters)
- Comment complex algorithms
- Keep functions focused and small

Example:
```cpp
void detectAccident(float ax, float ay, float az) {
    // Calculate acceleration magnitude
    float magnitude = sqrt(ax*ax + ay*ay + az*az);
    
    // Check against threshold
    if (magnitude > IMPACT_THRESHOLD_G) {
        triggerAlert();
    }
}
```

### Documentation

- Use Markdown for all documentation
- Keep line length < 100 characters
- Use clear headings and structure
- Include code examples where helpful
- Update table of contents if adding sections

## Areas for Contribution

### High Priority

- [ ] Real RSSI implementation for HC-12 and LoRa modules
- [ ] Event logging to SD card or EEPROM
- [ ] Watchdog timer implementation
- [ ] Power consumption optimization
- [ ] Field testing and calibration data

### Medium Priority

- [ ] AES encryption for RF packets
- [ ] GPS integration (when available)
- [ ] Multi-vehicle alert queue on receiver
- [ ] Web-based configuration interface
- [ ] Battery voltage monitoring

### Low Priority

- [ ] Mobile app for fleet monitoring
- [ ] Cloud integration (when connectivity available)
- [ ] Voice communication capability
- [ ] Automatic firmware updates
- [ ] Advanced analytics and reporting

## Testing Requirements

All contributions must include:

1. **Hardware testing** on actual components
2. **Documentation** of test procedures and results
3. **No regressions** - existing features must still work
4. **Performance metrics** if applicable

## Documentation Updates

When adding features, update:

- [ ] README.md (if user-facing)
- [ ] API_REFERENCE.md (if API changes)
- [ ] Relevant installation guides
- [ ] Configuration examples
- [ ] FAQ if common questions arise

## Hardware Contributions

If contributing hardware designs:

- Provide complete schematics
- Include bill of materials (BOM)
- Document assembly instructions
- Test on actual hardware
- Provide photos of working prototype

## Calibration Data

If you've calibrated the system for specific vehicles/terrain:

- Share threshold values
- Document vehicle type and terrain
- Include false positive rate data
- Note any special considerations

## License

By contributing, you agree that your contributions will be licensed under the MIT License.

## Questions?

Contact the project team:
- Project Guide: Dr. Raju G
- Team: Group 5, CSA Semester 6, Sahrdaya College

## Recognition

Contributors will be acknowledged in:
- README.md contributors section
- Project documentation
- Future publications (if applicable)

---

Thank you for helping make Off-Highway Guardian better!
