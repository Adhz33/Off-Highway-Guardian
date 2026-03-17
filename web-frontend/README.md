# Off-Highway Guardian - Web Frontend

Interactive web dashboard for the Off-Highway Guardian project.

## Features

### 🏠 Home Section
- Hero banner with project overview
- Live statistics (detection time, RF range, cost, offline capability)
- Animated system diagram showing vehicle-to-receiver communication

### ⚡ Features Section
- 6 feature cards with hover animations
- IMU-based detection, multi-condition analysis, RF beacon system
- RSSI homing, autonomous operation, cost-effectiveness

### 🔧 System Architecture
- Side-by-side comparison of Vehicle Unit vs Receiver Unit
- Complete component lists with specifications
- Cost breakdown per unit

### 📚 Documentation
- 6 interactive documentation cards
- Modal viewer for detailed content
- Quick Start, Installation, API Reference, Testing, BOM, FAQ

### 🎮 Live Demo
- Interactive accident simulation
- Real-time LED indicators (System Active, Accident Detected, Beacon Active)
- Sensor readings display (acceleration, tilt, rotation)
- LCD display simulation with RSSI bar
- System log with timestamps
- 4 simulation scenarios:
  - Impact (3g+ acceleration)
  - Rollover (45°+ tilt)
  - Spin (180°/s+ rotation)
  - Combined (multiple conditions)

### 📊 Roadmap
- Visual progress bars for all 4 project phases
- Current status tracking

### 👥 Team Section
- Team member cards
- Project supervisors
- Institution information

## Quick Start

### Option 1: Direct File Open
Simply open `index.html` in any modern web browser. No server required.

### Option 2: Local Server (Recommended)

**Python 3:**
```bash
cd off-highway-guardian/web-frontend
python3 -m http.server 3000
```

**Node.js:**
```bash
cd off-highway-guardian/web-frontend
npx http-server -p 3000
```

**PHP:**
```bash
cd off-highway-guardian/web-frontend
php -S localhost:3000
```

Then open: **http://localhost:3000**

## File Structure

```
web-frontend/
├── index.html          # Main HTML structure (495 lines)
├── styles.css          # Complete styling (867 lines)
├── script.js           # Interactive functionality (458 lines)
└── README.md           # This file
```

## Technologies Used

- **HTML5** - Semantic markup
- **CSS3** - Modern styling with CSS Grid, Flexbox, animations
- **Vanilla JavaScript** - No frameworks, pure ES6+
- **Font Awesome 6** - Icons (CDN)
- **Responsive Design** - Mobile-first approach

## Browser Compatibility

- ✅ Chrome 90+
- ✅ Firefox 88+
- ✅ Safari 14+
- ✅ Edge 90+

## Features Breakdown

### Navigation
- Sticky header with smooth scroll
- Active section highlighting
- Mobile hamburger menu
- Responsive design

### Animations
- Scroll-triggered entrance animations
- Hover effects on cards
- LED blinking simulation
- RSSI fluctuation animation
- Progress bar animations

### Interactive Demo
The live demo simulates the complete accident detection workflow:

1. **Impact Simulation**
   - Triggers 4.2g acceleration
   - Checks other conditions
   - Confirms after 2-of-3 threshold met

2. **Rollover Simulation**
   - Triggers 52° tilt angle
   - Sustained for >200ms
   - Activates beacon

3. **Spin Simulation**
   - Triggers 210°/s rotation
   - Combined with impact check
   - Multi-condition confirmation

4. **Combined Simulation**
   - All 3 conditions simultaneously
   - 5.1g + 67° + 320°/s
   - Immediate beacon activation

### System Log
- Real-time timestamped entries
- Color-coded by severity (info, success, warning, danger)
- Auto-scroll to latest entry
- Simulates actual firmware serial output

### RSSI Homing
- 5-LED bar indicator
- Dynamic signal strength display
- Simulated fluctuation (-110 to -40 dBm)
- LCD display with Unicode bar characters

## Customization

### Colors
Edit CSS variables in `styles.css`:
```css
:root {
    --primary-color: #2563eb;
    --secondary-color: #10b981;
    --danger-color: #ef4444;
    --warning-color: #f59e0b;
}
```

### Vehicle ID
Edit in `script.js`:
```javascript
lcdLine1.textContent = 'V:01000001 ' + alertType;
```

### Thresholds
Update documentation content in `script.js` `docContent` object.

## Performance

- **Load Time:** < 1 second (no external dependencies except Font Awesome)
- **File Size:** ~100KB total (HTML + CSS + JS)
- **Animations:** 60fps smooth transitions
- **Mobile Optimized:** Responsive breakpoints at 768px

## Accessibility

- Semantic HTML5 elements
- ARIA labels where needed
- Keyboard navigation support
- High contrast ratios
- Readable font sizes

## Future Enhancements

- [ ] Dark mode toggle
- [ ] Real-time data from actual hardware (WebSocket)
- [ ] Export system logs
- [ ] Multi-language support
- [ ] Print-friendly documentation view
- [ ] Offline PWA support

## Development

### Adding New Documentation
Edit `docContent` object in `script.js`:
```javascript
const docContent = {
    'your-doc-id': {
        content: `<h2>Your Title</h2><p>Your content...</p>`
    }
};
```

Add corresponding card in `index.html`:
```html
<div class="doc-card" data-doc="your-doc-id">
    <div class="doc-icon"><i class="fas fa-icon"></i></div>
    <h3>Your Title</h3>
    <p>Description</p>
    <button class="btn-doc">View Guide</button>
</div>
```

### Adding New Demo Scenarios
Add button in HTML and event listener in `script.js`:
```javascript
document.getElementById('yourButton').addEventListener('click', () => {
    if (state.accidentDetected) return;
    addLog('Your scenario triggered', 'warning');
    // Update sensor values
    // Call confirmAccident()
});
```

## Credits

**Developed by:** Group 5, CSA Semester 6  
**Institution:** Sahrdaya College of Engineering & Technology  
**Project Guide:** Dr. Raju G  
**Project Coordinator:** Anly Antony M

## License

MIT License - See parent directory LICENSE file

---

**Version:** 1.0  
**Last Updated:** January 2026  
**Status:** Production Ready ✅
