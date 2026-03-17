// Off-Highway Guardian - Frontend Script

// ---- Audio Context for Continuous Alarm Sound ----
const AudioContext = window.AudioContext || window.webkitAudioContext;
let audioContext;
let alarmInterval;

function initAudio() {
    if (!audioContext) {
        audioContext = new AudioContext();
    }
}

function playBuzzerBeep() {
    initAudio();
    const oscillator = audioContext.createOscillator();
    const gainNode = audioContext.createGain();
    
    oscillator.connect(gainNode);
    gainNode.connect(audioContext.destination);
    
    oscillator.frequency.value = 2000; // 2kHz beep
    oscillator.type = 'square'; // Square wave for buzzer sound
    
    gainNode.gain.setValueAtTime(0.3, audioContext.currentTime);
    gainNode.gain.exponentialRampToValueAtTime(0.01, audioContext.currentTime + 0.2);
    
    oscillator.start(audioContext.currentTime);
    oscillator.stop(audioContext.currentTime + 0.2);
}

function startContinuousAlarm() {
    // Stop any existing alarm
    stopContinuousAlarm();
    
    // Play first beep immediately
    playBuzzerBeep();
    
    // Continue beeping every 500ms (continuous alarm)
    alarmInterval = setInterval(() => {
        playBuzzerBeep();
    }, 500);
}

function stopContinuousAlarm() {
    if (alarmInterval) {
        clearInterval(alarmInterval);
        alarmInterval = null;
    }
}

// ---- Navigation ----
const navToggle = document.getElementById('navToggle');
const navMenu = document.querySelector('.nav-menu');

navToggle.addEventListener('click', () => {
    navMenu.classList.toggle('open');
});

const sections = document.querySelectorAll('section[id]');
const navLinks = document.querySelectorAll('.nav-link');

window.addEventListener('scroll', () => {
    let current = '';
    sections.forEach(section => {
        if (window.scrollY >= section.offsetTop - 80) {
            current = section.getAttribute('id');
        }
    });
    navLinks.forEach(link => {
        link.classList.remove('active');
        if (link.getAttribute('href') === '#' + current) link.classList.add('active');
    });
});

document.querySelectorAll('a[href^="#"]').forEach(anchor => {
    anchor.addEventListener('click', function (e) {
        e.preventDefault();
        const target = document.querySelector(this.getAttribute('href'));
        if (target) {
            target.scrollIntoView({ behavior: 'smooth' });
            navMenu.classList.remove('open');
        }
    });
});

// ---- Demo Simulation ----
const state = {
    accidentDetected: false,
    beaconInterval: null,
    rssiInterval: null,
    rssi: -80,
};

const ledSystem   = document.querySelector('#ledSystem .led');
const ledAccident = document.querySelector('#ledAccident .led');
const ledBeacon   = document.querySelector('#ledBeacon .led');
const accelValue  = document.getElementById('accelValue');
const tiltValue   = document.getElementById('tiltValue');
const gyroValue   = document.getElementById('gyroValue');
const lcdLine1    = document.getElementById('lcdLine1');
const lcdLine2    = document.getElementById('lcdLine2');
const rssiValueEl = document.getElementById('rssiValue');
const rssiLeds    = document.querySelectorAll('.rssi-led');
const logContent  = document.getElementById('logContent');

ledSystem.classList.add('active');

function addLog(message, type) {
    type = type || 'info';
    const colors = { info: '#9ca3af', success: '#10b981', warning: '#f59e0b', danger: '#ef4444' };
    const now = new Date();
    const ts = [now.getHours(), now.getMinutes(), now.getSeconds()]
        .map(n => String(n).padStart(2, '0')).join(':');
    const entry = document.createElement('div');
    entry.className = 'log-entry';
    entry.innerHTML = '<span style="color:#6b7280">[' + ts + ']</span> <span style="color:' + colors[type] + '">' + message + '</span>';
    logContent.appendChild(entry);
    logContent.scrollTop = logContent.scrollHeight;
}

function setRSSILeds(level) {
    rssiLeds.forEach((led, i) => led.classList.toggle('active', i < level));
}

function updateRSSI(dbm) {
    rssiValueEl.textContent = dbm + ' dBm';
    const level = Math.max(0, Math.min(5, Math.round(((dbm + 120) / 90) * 5)));
    setRSSILeds(level);
}

function rssiBar(dbm) {
    const count = Math.max(0, Math.min(5, Math.round(((dbm + 110) / 70) * 5)));
    return '\u25A0'.repeat(count) + '\u25A1'.repeat(5 - count);
}

function confirmAccident(alertType, accel, tilt, gyro) {
    if (state.accidentDetected) return;
    state.accidentDetected = true;

    accelValue.textContent = accel;
    tiltValue.textContent  = tilt;
    gyroValue.textContent  = gyro;

    ledAccident.classList.add('active');
    ledBeacon.classList.add('active');

    state.rssi = -80;
    lcdLine1.textContent = 'V:01000001 ' + alertType;
    lcdLine2.textContent = 'RSSI:-80 ' + rssiBar(-80);
    updateRSSI(-80);

    // Start continuous alarm sound
    startContinuousAlarm();

    state.rssiInterval = setInterval(() => {
        state.rssi += Math.floor(Math.random() * 11) - 5;
        state.rssi = Math.max(-110, Math.min(-40, state.rssi));
        updateRSSI(state.rssi);
        lcdLine2.textContent = 'RSSI:' + state.rssi + ' ' + rssiBar(state.rssi);
    }, 1200);

    let pkt = 1;
    addLog('ACCIDENT CONFIRMED — Activating RF beacon', 'danger');
    addLog('Beacon #' + pkt++ + ' transmitted — Vehicle: 01000001 | Type: ' + alertType, 'warning');
    addLog('Receiver alert activated — Continuous alarm sounding', 'warning');

    state.beaconInterval = setInterval(() => {
        addLog('Beacon #' + pkt++ + ' transmitted — Vehicle: 01000001 | Type: ' + alertType, 'warning');
    }, 5000);
}

function resetDemo() {
    clearInterval(state.beaconInterval);
    clearInterval(state.rssiInterval);
    stopContinuousAlarm(); // Stop the alarm sound
    state.accidentDetected = false;

    ledAccident.classList.remove('active');
    ledBeacon.classList.remove('active');

    accelValue.textContent = '1.0g';
    tiltValue.textContent  = '0\u00b0';
    gyroValue.textContent  = '0\u00b0/s';

    lcdLine1.textContent = 'Monitoring...';
    lcdLine2.textContent = '';

    setRSSILeds(0);
    rssiValueEl.textContent = '-80 dBm';

    addLog('System reset — Monitoring resumed', 'success');
}

document.getElementById('simulateImpact').addEventListener('click', () => {
    if (state.accidentDetected) return;
    addLog('Impact event detected — Acceleration: 4.2g', 'warning');
    accelValue.textContent = '4.2g';
    setTimeout(() => {
        addLog('Rollover check: 12\u00b0 — below threshold', 'info');
        addLog('Spin check: 22\u00b0/s — below threshold', 'info');
        setTimeout(() => {
            addLog('Second condition: impact sustained — CONFIRMED', 'danger');
            confirmAccident('IMPACT', '4.2g', '12\u00b0', '22\u00b0/s');
        }, 600);
    }, 300);
});

document.getElementById('simulateRollover').addEventListener('click', () => {
    if (state.accidentDetected) return;
    addLog('Tilt angle detected — 52\u00b0', 'warning');
    tiltValue.textContent = '52\u00b0';
    setTimeout(() => {
        addLog('Impact check: 1.8g — below threshold', 'info');
        addLog('Rollover sustained > 200ms — CONFIRMED', 'danger');
        confirmAccident('ROLLOVER', '1.8g', '52\u00b0', '45\u00b0/s');
    }, 400);
});

document.getElementById('simulateSpin').addEventListener('click', () => {
    if (state.accidentDetected) return;
    addLog('High angular velocity detected — 210\u00b0/s', 'warning');
    gyroValue.textContent = '210\u00b0/s';
    setTimeout(() => {
        addLog('Impact check: 2.1g — below threshold', 'info');
        addLog('Spin + impact conditions met — CONFIRMED', 'danger');
        confirmAccident('SPIN', '2.1g', '18\u00b0', '210\u00b0/s');
    }, 400);
});

document.getElementById('simulateCombined').addEventListener('click', () => {
    if (state.accidentDetected) return;
    addLog('Multiple conditions triggered simultaneously', 'warning');
    accelValue.textContent = '5.1g';
    tiltValue.textContent  = '67\u00b0';
    gyroValue.textContent  = '320\u00b0/s';
    setTimeout(() => {
        addLog('Impact: 5.1g — THRESHOLD EXCEEDED', 'danger');
        addLog('Rollover: 67\u00b0 — THRESHOLD EXCEEDED', 'danger');
        addLog('Spin: 320\u00b0/s — THRESHOLD EXCEEDED', 'danger');
        addLog('3/3 conditions confirmed within 300ms window', 'danger');
        confirmAccident('COMBINED', '5.1g', '67\u00b0', '320\u00b0/s');
    }, 300);
});

document.getElementById('resetDemo').addEventListener('click', resetDemo);

// ---- Documentation Modal ----
const modal     = document.getElementById('docModal');
const modalBody = document.getElementById('modalBody');
const modalClose = document.querySelector('.modal-close');

const docContent = {
    'quick-start': {
        content: `<h2>Quick Start Guide</h2>
<p>Get your Off-Highway Guardian system running in 30 minutes.</p>
<h3>Step 1 — Hardware Assembly</h3>
<pre>MPU-6050 to ESP32
  VCC -> 3.3V | GND -> GND
  SDA -> GPIO 21 | SCL -> GPIO 22

HC-12 RF to ESP32
  VCC -> 5V | GND -> GND
  TX  -> GPIO 16 | RX -> GPIO 17

LEDs (220 ohm resistors)
  Green -> GPIO 2 | Yellow -> GPIO 4 | Red -> GPIO 5

Override Button
  One side -> GPIO 15 | Other side -> GND</pre>
<h3>Step 2 — Upload Firmware</h3>
<ol>
  <li>Install Arduino IDE and ESP32 board support</li>
  <li>Open <code>vehicle-unit/vehicle-unit.ino</code></li>
  <li>Set unique Vehicle ID in <code>config.h</code></li>
  <li>Select board: ESP32 Dev Module, then click Upload</li>
</ol>
<h3>Step 3 — First Test</h3>
<ul>
  <li>Green LED ON = System active</li>
  <li>Tilt unit 45°+ = Yellow + Red LEDs activate</li>
  <li>Receiver beeps 3 times = Alert received</li>
  <li>Hold button 5s = Beacon cancelled</li>
</ul>`
    },
    'installation': {
        content: `<h2>Installation Manual</h2>
<h3>Vehicle Unit — Full Wiring</h3>
<pre>MPU-6050 (GY-521)
  VCC -> 3.3V (ESP32) | GND -> GND
  SDA -> GPIO 21 | SCL -> GPIO 22
  AD0 -> GND (I2C address 0x68)

HC-12 RF Module
  VCC -> 5V (buck converter) | GND -> GND
  TX  -> GPIO 16 (RX2) | RX -> GPIO 17 (TX2)

Power Supply
  12V+ -> [FUSE 5A] -> [TVS Diode] -> LM2596 IN+
  LM2596 OUT+ -> 5V rail
  Add 100uF capacitors on both sides of converter</pre>
<h3>Receiver Unit — Full Wiring</h3>
<pre>16x2 LCD (I2C)
  VCC -> 5V | GND -> GND
  SDA -> GPIO 21 | SCL -> GPIO 22

RSSI Bar LEDs (220 ohm each)
  LED1 -> GPIO 12 | LED2 -> GPIO 13
  LED3 -> GPIO 14 | LED4 -> GPIO 27 | LED5 -> GPIO 26

Buzzer
  (+) -> GPIO 5 | (-) -> GND</pre>
<h3>Vehicle Mounting</h3>
<ul>
  <li>Mount IMU rigidly to chassis — no flexible panels</li>
  <li>Orient Z-axis vertical when vehicle is level</li>
  <li>Use vibration-damping washers on all bolts</li>
  <li>Mount antenna vertically on roof or roll cage</li>
  <li>Route power through 5A inline fuse</li>
</ul>`
    },
    'api': {
        content: `<h2>API Reference</h2>
<h3>Emergency Packet Structure (11 bytes)</h3>
<pre>typedef struct __attribute__((packed)) {
    uint8_t  version;       // Protocol version (1)
    uint8_t  vehicleId[4];  // Unique vehicle ID
    uint8_t  alertType;     // Accident type code
    uint32_t timestamp;     // Milliseconds since boot
    uint8_t  checksum;      // XOR checksum
} EmergencyPacket;</pre>
<h3>Alert Types</h3>
<table class="modal-table">
  <tr><th>Value</th><th>Constant</th><th>Description</th></tr>
  <tr><td>0x01</td><td>ALERT_TYPE_IMPACT</td><td>Sudden impact (&gt;3g)</td></tr>
  <tr><td>0x02</td><td>ALERT_TYPE_ROLLOVER</td><td>Vehicle rollover (&gt;45°)</td></tr>
  <tr><td>0x03</td><td>ALERT_TYPE_SPIN</td><td>Loss of control (&gt;180°/s)</td></tr>
  <tr><td>0x04</td><td>ALERT_TYPE_COMBINED</td><td>Multiple conditions</td></tr>
</table>
<h3>Key Functions</h3>
<pre>void detectAccident(float ax, float ay, float az,
                    float gx, float gy, float gz);
void transmitBeacon();
void cancelBeacon();
bool mpu.testConnection();
void mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
uint8_t calculateChecksum(const EmergencyPacket* p);
bool    verifyChecksum(const EmergencyPacket* p);</pre>
<h3>Configuration Constants</h3>
<table class="modal-table">
  <tr><th>Constant</th><th>Default</th><th>Description</th></tr>
  <tr><td>IMPACT_THRESHOLD_G</td><td>3.0</td><td>Impact threshold (g)</td></tr>
  <tr><td>ROLLOVER_ANGLE_DEG</td><td>45.0</td><td>Rollover angle (°)</td></tr>
  <tr><td>SPIN_THRESHOLD_DPS</td><td>180.0</td><td>Spin rate (°/s)</td></tr>
  <tr><td>CONFIRMATION_WINDOW_MS</td><td>300</td><td>Multi-condition window (ms)</td></tr>
  <tr><td>BEACON_INTERVAL_MS</td><td>5000</td><td>Beacon interval (ms)</td></tr>
  <tr><td>OVERRIDE_HOLD_TIME_MS</td><td>5000</td><td>Override hold time (ms)</td></tr>
</table>`
    },
    'testing': {
        content: `<h2>Testing Guide — Phase II</h2>
<h3>Test 1: Impact Detection</h3>
<p>Drop unit from 30cm onto cushioned surface. Expect detection within 500ms.</p>
<table class="modal-table">
  <tr><th>Test #</th><th>Drop Height</th><th>Detection</th><th>Latency</th><th>Pass</th></tr>
  <tr><td>1</td><td>30cm</td><td>Yes</td><td>320ms</td><td>✅</td></tr>
  <tr><td>2</td><td>30cm</td><td>Yes</td><td>280ms</td><td>✅</td></tr>
</table>
<h3>Test 2: Rollover Detection</h3>
<table class="modal-table">
  <tr><th>Angle</th><th>Axis</th><th>Detection</th><th>Pass</th></tr>
  <tr><td>30°</td><td>Pitch</td><td>No</td><td>✅</td></tr>
  <tr><td>45°</td><td>Pitch</td><td>Yes (250ms)</td><td>✅</td></tr>
  <tr><td>45°</td><td>Roll</td><td>Yes (240ms)</td><td>✅</td></tr>
</table>
<h3>Test 3: Multi-Condition Confirmation</h3>
<table class="modal-table">
  <tr><th>Conditions Applied</th><th>Beacon Activated</th><th>Pass</th></tr>
  <tr><td>Impact only</td><td>No</td><td>✅</td></tr>
  <tr><td>Tilt only</td><td>No</td><td>✅</td></tr>
  <tr><td>Impact + Tilt</td><td>Yes</td><td>✅</td></tr>
  <tr><td>Impact + Spin</td><td>Yes</td><td>✅</td></tr>
</table>
<h3>Test 4: RF Range</h3>
<table class="modal-table">
  <tr><th>Distance</th><th>Success Rate</th><th>RSSI</th><th>Pass</th></tr>
  <tr><td>100m</td><td>100%</td><td>-58 dBm</td><td>✅</td></tr>
  <tr><td>300m</td><td>100%</td><td>-85 dBm</td><td>✅</td></tr>
  <tr><td>500m</td><td>100%</td><td>-98 dBm</td><td>✅</td></tr>
</table>
<h3>Test 5: False Positive Rate</h3>
<p>30-minute rough terrain drive. Target: &lt;5% false positives.</p>
<p><strong>Result: 0 false positives in 30 minutes ✅</strong></p>`
    },
    'hardware': {
        content: `<h2>Bill of Materials</h2>
<h3>Vehicle Unit — Rs.1,515 per unit</h3>
<table class="modal-table">
  <tr><th>Component</th><th>Specification</th><th>Qty</th><th>Cost (INR)</th></tr>
  <tr><td>ESP32 DevKit</td><td>ESP32-WROOM-32</td><td>1</td><td>Rs.450</td></tr>
  <tr><td>MPU-6050</td><td>GY-521 6-DOF IMU</td><td>1</td><td>Rs.150</td></tr>
  <tr><td>HC-12 RF Module</td><td>433MHz, 1000m range</td><td>1</td><td>Rs.350</td></tr>
  <tr><td>Buck Converter</td><td>LM2596 12V to 5V</td><td>1</td><td>Rs.80</td></tr>
  <tr><td>LEDs</td><td>5mm, assorted</td><td>3</td><td>Rs.15</td></tr>
  <tr><td>Push Button</td><td>Tactile switch</td><td>1</td><td>Rs.10</td></tr>
  <tr><td>Resistors</td><td>220 ohm, 10k ohm</td><td>5</td><td>Rs.10</td></tr>
  <tr><td>Enclosure</td><td>Waterproof ABS</td><td>1</td><td>Rs.200</td></tr>
  <tr><td>Antenna</td><td>433MHz spring</td><td>1</td><td>Rs.50</td></tr>
  <tr><td>Misc</td><td>PCB, wires, connectors</td><td>—</td><td>Rs.200</td></tr>
</table>
<h3>Receiver Unit — Rs.2,179 per unit</h3>
<table class="modal-table">
  <tr><th>Component</th><th>Specification</th><th>Qty</th><th>Cost (INR)</th></tr>
  <tr><td>ESP32 DevKit</td><td>ESP32-WROOM-32</td><td>1</td><td>Rs.450</td></tr>
  <tr><td>HC-12 RF Module</td><td>433MHz, 1000m range</td><td>1</td><td>Rs.350</td></tr>
  <tr><td>LCD Display</td><td>16x2 with I2C backpack</td><td>1</td><td>Rs.200</td></tr>
  <tr><td>LEDs</td><td>5mm, assorted</td><td>7</td><td>Rs.35</td></tr>
  <tr><td>Buzzer</td><td>Active 5V</td><td>1</td><td>Rs.30</td></tr>
  <tr><td>Power Bank</td><td>10000mAh USB</td><td>1</td><td>Rs.600</td></tr>
  <tr><td>Misc</td><td>PCB, wires, enclosure</td><td>—</td><td>Rs.514</td></tr>
</table>
<h3>Fleet Cost (10 vehicles + 3 receivers)</h3>
<table class="modal-table">
  <tr><th>Item</th><th>Qty</th><th>Unit Cost</th><th>Total</th></tr>
  <tr><td>Vehicle Units</td><td>10</td><td>Rs.1,515</td><td>Rs.15,150</td></tr>
  <tr><td>Receiver Units</td><td>3</td><td>Rs.2,179</td><td>Rs.6,537</td></tr>
  <tr><td colspan="3"><strong>Total Fleet Cost</strong></td><td><strong>Rs.21,687</strong></td></tr>
</table>`
    },
    'faq': {
        content: `<h2>Frequently Asked Questions</h2>
<h3>What accidents can the system detect?</h3>
<p>Three types: <strong>Impact</strong> (&gt;3g), <strong>Rollover</strong> (&gt;45° tilt), and <strong>Spin</strong> (&gt;180°/s). Any 2 of 3 within 300ms confirms an accident.</p>
<h3>Why not use GPS or cellular?</h3>
<p>GPS is blocked under dense forest canopy and cellular networks are absent in remote areas. This system is fully offline and infrastructure-independent.</p>
<h3>How does RSSI homing work?</h3>
<p>The receiver displays signal strength (dBm). As rescue teams walk toward the vehicle, RSSI increases. Follow the direction of increasing signal to locate the accident site.</p>
<h3>What is the RF range?</h3>
<p>500m+ in open terrain. In dense forest expect 250-350m (30-50% reduction). LoRa modules can extend this to 3-5km.</p>
<h3>How do I prevent false alarms?</h3>
<p>The multi-condition confirmation (2-of-3 within 300ms) handles most cases. If still getting false positives, increase <code>IMPACT_THRESHOLD_G</code> in config.h and ensure the IMU is rigidly mounted.</p>
<h3>Can I use Arduino instead of ESP32?</h3>
<p>Yes, Arduino Nano/Uno is compatible. ESP32 is recommended for more GPIO pins and better processing power.</p>
<h3>How long does the receiver battery last?</h3>
<p>~40-45 hours on a 10,000mAh power bank.</p>
<h3>How do I cancel a false alarm?</h3>
<p>Hold the override button on the vehicle unit for 5 seconds. The beacon deactivates and LEDs turn off.</p>
<h3>Can it monitor multiple vehicles?</h3>
<p>Yes. Each vehicle has a unique ID. The receiver displays the most recent alert and logs all packets to serial.</p>`
    }
};

document.querySelectorAll('.doc-card').forEach(card => {
    card.addEventListener('click', () => {
        const doc = docContent[card.dataset.doc];
        if (!doc) return;
        modalBody.innerHTML = doc.content;
        modal.style.display = 'block';
        document.body.style.overflow = 'hidden';
    });
});

modalClose.addEventListener('click', closeModal);
modal.addEventListener('click', e => { if (e.target === modal) closeModal(); });
document.addEventListener('keydown', e => { if (e.key === 'Escape') closeModal(); });

function closeModal() {
    modal.style.display = 'none';
    document.body.style.overflow = '';
}

// ---- Roadmap progress bars ----
const roadmapSection = document.getElementById('roadmap');
if (roadmapSection) {
    const container = roadmapSection.querySelector('.roadmap-bars');
    const items = [
        { phase: 'Phase I',   label: 'Planning & Design',    pct: 100, color: '#10b981' },
        { phase: 'Phase II',  label: 'Prototype Build',       pct: 85,  color: '#2563eb' },
        { phase: 'Phase III', label: 'RF & Field Testing',    pct: 20,  color: '#f59e0b' },
        { phase: 'Phase IV',  label: 'Refinement & Demo',     pct: 0,   color: '#6b7280' },
    ];
    items.forEach(item => {
        const div = document.createElement('div');
        div.className = 'roadmap-item';
        div.innerHTML =
            '<div class="roadmap-label"><span>' + item.phase + '</span><span>' + item.label + '</span><span>' + item.pct + '%</span></div>' +
            '<div class="roadmap-track"><div class="roadmap-fill" style="width:' + item.pct + '%;background:' + item.color + '"></div></div>';
        container.appendChild(div);
    });
}

// ---- Scroll entrance animations ----
const animCards = document.querySelectorAll('.feature-card, .doc-card, .team-card');
const cardObserver = new IntersectionObserver(entries => {
    entries.forEach((entry, i) => {
        if (entry.isIntersecting) {
            setTimeout(() => {
                entry.target.style.opacity = '1';
                entry.target.style.transform = 'translateY(0)';
            }, i * 80);
            cardObserver.unobserve(entry.target);
        }
    });
}, { threshold: 0.1 });

animCards.forEach(card => {
    card.style.opacity = '0';
    card.style.transform = 'translateY(30px)';
    card.style.transition = 'opacity 0.5s, transform 0.5s';
    cardObserver.observe(card);
});
