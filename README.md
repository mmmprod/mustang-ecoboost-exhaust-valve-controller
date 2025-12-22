# 🏎️ VALVE - Exhaust Valve Controller

![License](https://img.shields.io/badge/License-MIT-green.svg)
![Version](https://img.shields.io/badge/Version-4.5-blue.svg)
![ISO](https://img.shields.io/badge/ISO_7637--2-Compliant-brightgreen.svg)
![Platform](https://img.shields.io/badge/Platform-Ford_Mustang_EcoBoost-red.svg)
![Power](https://img.shields.io/badge/Power-600+_HP-orange.svg)
![Made with](https://img.shields.io/badge/Made_with-Love_&_Boost-ff69b4.svg)

> **Because your neighbors don't need to know you're running 600+ HP... until you want them to.**

Automatic exhaust valve controller for 2016 Ford Mustang EcoBoost Stage 4+ E85. Opens the valve when boost pressure is detected, closes when cruising. Manual override via OEM-style cockpit button.

---

## 🎯 Features

| Feature | Description |
|---------|-------------|
| **Auto Mode** | Valve opens automatically at 2.9-3.6 PSI boost |
| **Manual Override** | Toggle with illuminated Toyota-style button |
| **Fail-Safe** | Fail-closed actuator = silence on failure |
| **ISO 7637-2** | Pulse 1/3 compliant (-150V transient protection) |
| **5-Second Diagnostic** | LED + TEST button for instant troubleshooting |
| **Breakout Box Ready** | 12-point connector for automated validation |

---

## 🔧 System Overview
```
┌─────────────┐     ┌─────────────┐     ┌─────────────┐
│   BOOST     │     │   VALVE     │     │  EXHAUST    │
│  PRESSURE   │────▶│ CONTROLLER  │────▶│   VALVE     │
│  (Intake)   │     │   v4.5      │     │ (Actuator)  │
└─────────────┘     └──────┬──────┘     └─────────────┘
                          │
              ┌───────────┴───────────┐
              │                       │
        ┌─────▼─────┐          ┌──────▼──────┐
        │  SMC      │          │   Toyota    │
        │ ISE30A    │          │   Button    │
        │ Pressure  │          │  22x22mm    │
        │  Switch   │          │  Illuminated│
        └───────────┘          └─────────────┘
```

---

## 📋 Specifications

### Electrical

| Parameter | Value |
|-----------|-------|
| Input Voltage | 9-16V DC (ISO 7637-2 compliant) |
| Quiescent Current | < 10mA |
| Solenoid Current | 450mA @ 12V |
| Transient Protection | SM5S22A (Pulse 1) + 5KP18CA + MOV |
| Logic Supply | 5V regulated (78L05) |

### Pneumatic

| Parameter | Value |
|-----------|-------|
| Detection Pressure | 2.9-3.6 PSI (ON) / 2.0-2.9 PSI (OFF) |
| Supply Pressure | 90-120 PSI (shop air) |
| Regulated Pressure | 15 PSI (SMC IR1000-01BG) |
| Actuator | Fail-closed, 7 PSI max |

### Protection Chain (v4.5)
```
+12V_IN ─▶ SM5S22A ─▶ Ferrite ─▶ Fuse 1A ─▶ 1N5822 ─▶ +12V_PROT
             │                                            │
         [GND_STAR]                              TVS 5KP18CA + MOV
```

**ISO 7637-2 Pulse 1 (-150V):** Clamped to 36V by SM5S22A → 1N5822 survives (V_RRM = 40V)

---

## 🛠️ Hardware

### Main Components

| Component | Reference | Purpose |
|-----------|-----------|---------|
| Pressure Switch | SMC ISE30A-01-N | Boost detection (NPN output) |
| Solenoid Valve | MAC 35A-AAA-DDBA-1BA | 3-way pneumatic control |
| Pressure Regulator | SMC IR1000-01BG | Reduces 120 PSI → 15 PSI |
| High-Side Driver | BTS5090-1EJA | Protected MOSFET driver |
| Toggle Button | Toyota 22x22mm | Illuminated manual override |
| TVS Automotive | SM5S22A | ISO 7637-2 Pulse 1 protection |

### Diagnostic (v4.4+)

| Element | Function |
|---------|----------|
| Green LED (POWER) | 5V rail OK indicator |
| TEST Button | Simulates boost → validates entire chain |

---

## 🧪 Breakout Box V1.3

Universal test interface for automated validation.

### Hardware

| Component | Purpose |
|-----------|---------|
| ESP32-C6-LCD-1.47 | Controller + 1.47" display |
| INA219 | Current measurement (protected) |
| 2x 2N2222 NPN | Open-collector STIM outputs |
| 1N4749A + 1kΩ | INA219 overvoltage protection |

### Test Modes

| Mode | Description |
|------|-------------|
| **LIVE** | Real-time voltages + Min/Max hold |
| **STRESS** | 20 cycles @ 10Hz, measures V_sag |
| **FULL AUTO** | Sequential validation all stages |
| **CALIBRATE** | ADC calibration routine |

### Connector Pinout (J-BB)
```
Row A: [12V_PROT] [5V_LOG] [AUTO_REQ] [OUT_DRV] [CMD] [MANUAL]
Row B: [GND] [STIM1] [STIM2] [I_SENSE+] [I_SENSE-] [GND]
```

---

## 📁 Repository Structure
```
VALVE/
├── VALVE_v4_5.docx           # Full circuit documentation
├── BreakoutBox_V1.3.ino      # ESP32 firmware (846 lines)
├── BreakoutBox_Circuit_V1.3.md
├── User_Setup_C6_LCD.h       # TFT_eSPI configuration
└── README.md
```

---

## ⚠️ Critical Warnings

1. **SM5S22A Orientation:** Anode to +12V_IN, cathode to GND (BEFORE ferrite)
2. **1N5822 Polarity:** Band (cathode) towards +12V_PROT
3. **No Check Valve:** Detection line must be FREE-FLOWING
4. **Pressure Regulator:** SMC IR1000-01BG set to 1 bar MANDATORY
5. **MAC Return Wire:** To GND_STAR ONLY (not chassis)

---

## 🚀 Quick Start

### 5-Second Diagnostic

1. **Green LED ON?** → Power OK
2. **Press TEST → Toyota button lights?** → Logic chain OK  
3. **Release TEST → Toyota button OFF?** → System validated ✅

### First Installation
```bash
1. Mount controller in trunk (away from heat)
2. Connect +12V_ACC to ignition-switched power
3. Connect +12V_BATT to constant 12V (fused 5A)
4. Route vacuum line from intake manifold
5. Connect shop air through regulator (15 PSI)
6. Wire Toyota button to cockpit
7. Run 5-second diagnostic
8. Drive and enjoy! 🏎️
```

---

## 📊 Validation Results

| Test | Specification | Result |
|------|---------------|--------|
| Cold Crank (6V) | 5V rail stable | ✅ PASS |
| Load Dump (60V) | No damage | ✅ PASS |
| ISO 7637-2 Pulse 1 | -150V clamped | ✅ PASS |
| Stress Test (10Hz) | V_sag < 2V | ✅ PASS |
| Current @ Idle | < 10mA | ✅ PASS |

---

## 📜 License

MIT License - Do whatever you want, just don't blame me if your exhaust wakes up the whole neighborhood.

---

## 🤝 Contributing

Found a bug? Want to add a feature? 

1. Fork it
2. Create your feature branch (`git checkout -b feature/LouderExhaust`)
3. Commit your changes (`git commit -m 'Add some noise'`)
4. Push to the branch (`git push origin feature/LouderExhaust`)
5. Open a Pull Request

---

<p align="center">
  <img src="https://img.shields.io/badge/BOOST-ON-success?style=for-the-badge" />
  <img src="https://img.shields.io/badge/VALVE-OPEN-blue?style=for-the-badge" />
  <img src="https://img.shields.io/badge/NEIGHBORS-SORRY-red?style=for-the-badge" />
</p>

<p align="center">
  <i>Built with ❤️ and way too much boost pressure</i>
</p>
