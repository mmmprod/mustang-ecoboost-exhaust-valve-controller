<div align="center">

# 🐎 Mustang EcoBoost Exhaust Valve Controller

### *When 600+ HP needs to breathe*

<br>

[![Version](https://img.shields.io/badge/version-4.7-0366d6?style=for-the-badge&logo=v&logoColor=white)](https://github.com)
[![Status](https://img.shields.io/badge/status-PRODUCTION_READY-28a745?style=for-the-badge&logo=checkmarx&logoColor=white)](https://github.com)
[![ISO](https://img.shields.io/badge/ISO_7637--2-COMPLIANT-ff6b00?style=for-the-badge&logo=iso&logoColor=white)](https://github.com)

<br>

[![Mustang](https://img.shields.io/badge/Ford-Mustang_EcoBoost-00529b?style=flat-square&logo=ford&logoColor=white)](https://github.com)
[![Tuning](https://img.shields.io/badge/Stage_4+-E85_Flex_Fuel-e85b00?style=flat-square&logo=fires&logoColor=white)](https://github.com)
[![Power](https://img.shields.io/badge/600+-Horsepower-dc3545?style=flat-square&logo=lightning&logoColor=white)](https://github.com)
[![Build](https://img.shields.io/badge/build-THT_Stripboard-6f42c1?style=flat-square&logo=arduino&logoColor=white)](https://github.com)

<br>

```
    ___  ___          _                      
    |  \/  |         | |                     
    | .  . |_   _ ___| |_ __ _ _ __   __ _   
    | |\/| | | | / __| __/ _` | '_ \ / _` |  
    | |  | | |_| \__ \ || (_| | | | | (_| |  
    \_|  |_/\__,_|___/\__\__,_|_| |_|\__, |  
                                      __/ |  
     EXHAUST VALVE CONTROLLER v4.7   |___/   
```

<br>

**Automatic boost-activated exhaust bypass system**

*Fail-Safe • Automotive-Grade • Plug & Play*

<br>

[📖 Documentation](#-documentation) •
[🔧 Installation](#-installation) •
[⚡ Quick Start](#-quick-start) •
[🛡️ Safety](#%EF%B8%8F-safety)

<br>

---

</div>

## 🆕 What's New in v4.7

<table>
<tr>
<td>🔧</td>
<td><b>BTS5090 + DIP Adapter</b></td>
<td>Smart high-side driver now mountable on stripboard via TSSOP-8→DIP-8 adapter</td>
</tr>
<tr>
<td>💡</td>
<td><b>Brighter Power LED</b></td>
<td>R20 reduced to 470Ω — doubled visibility (6.4mA vs 3mA)</td>
</tr>
<tr>
<td>📟</td>
<td><b>Breakout Box V1.5</b></td>
<td>Watchdog, NVS calibration, I2C timeout, THT-only protection clamps</td>
</tr>
<tr>
<td>❄️</td>
<td><b>Cold Crank Advisory</b></td>
<td>MCP1702 upgrade path documented for extreme cold starts</td>
</tr>
</table>

---

## 📊 Quick Stats

<table>
<tr>
<td align="center">
<img src="https://img.shields.io/badge/2.9--3.6-PSI-dc3545?style=for-the-badge" alt="Boost"/><br>
<sub><b>Boost Threshold</b></sub>
</td>
<td align="center">
<img src="https://img.shields.io/badge/100V-400ms-ffc107?style=for-the-badge" alt="Load Dump"/><br>
<sub><b>Load Dump Protection</b></sub>
</td>
<td align="center">
<img src="https://img.shields.io/badge/24.4V-Clamp-28a745?style=for-the-badge" alt="Clamp"/><br>
<sub><b>TVS Clamping</b></sub>
</td>
<td align="center">
<img src="https://img.shields.io/badge/--40°C_to_+85°C-Operating-17a2b8?style=for-the-badge" alt="Temp"/><br>
<sub><b>Temperature Range</b></sub>
</td>
</tr>
</table>

---

## 🎯 Features

<table>
<tr>
<td width="50%">

### 🚗 Automatic Mode
Opens valve when SMC ISE30A pressure switch detects boost above configurable threshold

### 🕹️ Manual Override  
Toggle valve state anytime with OEM-style illuminated Toyota push button

### 🛡️ Fail-Safe Design
Pneumatic actuator is fail-closed — valve closes on power loss, protecting your turbo

</td>
<td width="50%">

### ⚡ Automotive-Grade
Full ISO 7637-2 transient protection including load dump (+100V) and cold crank (6V)

### 🔍 Quick Diagnostic
5-second visual check: LED + TEST button validates entire signal chain

### 🔧 DIY Friendly
100% through-hole build on stripboard — SMD driver uses plug-in DIP adapter

</td>
</tr>
</table>

---

## 🏗️ System Architecture

```
┌─────────────────────────────────────────────────────────────────────────────┐
│                          VALVE CONTROLLER v4.7                              │
├─────────────────────────────────────────────────────────────────────────────┤
│                                                                             │
│   ┌─────────┐    ┌─────────┐    ┌─────────┐    ┌─────────┐    ┌─────────┐  │
│   │ +12V    │───▶│ P6KE18CA│───▶│ FERRITE │───▶│ FUSE 1A │───▶│ 1N5822  │  │
│   │ BATTERY │    │ TVS 18V │    │ 90Ω EMI │    │ PROTECT │    │ REVERSE │  │
│   └─────────┘    └─────────┘    └─────────┘    └─────────┘    └────┬────┘  │
│                                                                     │       │
│        ┌────────────────────────────────────────────────────────────┘       │
│        ▼                                                                    │
│   ┌─────────┐         ┌─────────────────────────────────────────────────┐  │
│   │ 78L05   │────────▶│                  +5V LOGIC                      │  │
│   │ 5V REG  │         └─────────────────────────────────────────────────┘  │
│   └─────────┘                     │                    │                    │
│                                   ▼                    ▼                    │
│   ┌─────────┐    ┌─────────┐    ┌─────────┐    ┌─────────┐    ┌─────────┐  │
│   │ BOOST   │───▶│ SMC     │───▶│ 74HC14  │───▶│   OR    │───▶│ BTS5090 │  │
│   │ INTAKE  │    │ ISE30A  │    │ BUFFER  │    │  GATE   │    │ +ADAPTER│  │
│   └─────────┘    └─────────┘    └─────────┘    └────┬────┘    └────┬────┘  │
│                                                     │              │       │
│   ┌─────────┐    ┌─────────┐                       │              ▼       │
│   │ TOYOTA  │───▶│ CD4013  │───────────────────────┘         ┌─────────┐  │
│   │ BUTTON  │    │ TOGGLE  │                                 │ MAC 35A │  │
│   └─────────┘    └─────────┘                                 │ SOLENOID│  │
│                                                              └────┬────┘  │
│   ┌─────────────────────────────────────────────────────────────┐ │       │
│   │ 🟢 LED POWER        🔘 TEST BUTTON        📟 J-BB BREAKOUT  │ │       │
│   └─────────────────────────────────────────────────────────────┘ ▼       │
│                                                              ACTUATOR      │
└─────────────────────────────────────────────────────────────────────────────┘
```

---

## 🔌 Hardware

### Protection Chain (BLOC A) — *v4.7 Certified*

| Stage | Component | Function | Specs | Package |
|:-----:|-----------|----------|-------|:-------:|
| **1** | `P6KE18CA` | TVS Clamping | V_C = 24.4V @ 10A, 600W | DO-15 |
| **2** | `Würth 742792093` | EMI Filter | 90Ω @ 100MHz | Axial |
| **3** | `Fuse 1A T` | Overcurrent | Fast-blow | 5×20mm |
| **4** | `1N5822` | Reverse Polarity | 40V 3A Schottky | DO-201AD |

> ⚠️ **CRITICAL**: TVS orientation — **Cathode (band) → +12V_IN** / **Anode → GND**

### Logic & Control

| Block | Component | Package | Function |
|:-----:|-----------|:-------:|----------|
| **B** | 78L05 | TO-92 | 5V Regulation |
| **C** | SMC ISE30A-01-N | — | Boost Pressure Sensing |
| **C** | 74HC14 | DIP-14 | Schmitt Buffer |
| **D** | Toyota 22×22mm | — | Illuminated Push Button |
| **E** | CD4013 | DIP-14 | Toggle Flip-Flop |
| **G** | BTS5090-1EJA | TSSOP-8 **+ DIP Adapter** | High-Side Smart Driver |

### 🆕 DIP Adapter (v4.7)

| SMD Component | Adapter | Source |
|---------------|---------|--------|
| BTS5090-1EJA (TSSOP-8) | TSSOP-8 → DIP-8 | Aries 08-350000-11 |

> 💡 **Why keep the BTS5090?** Integrated OC/OT/ESD protection + diagnostic pin — no THT equivalent exists with same features

### Pneumatic System

| Component | Reference | Function |
|-----------|-----------|----------|
| Pressure Switch | SMC ISE30A-01-N | Boost detection (NPN, 12-24V) |
| Pressure Regulator | SMC IR1000-01BG | Compressor → 1 bar output |
| Solenoid Valve | MAC 35A 12V | Pneumatic control (5.4W) |
| Actuator | Fail-closed | Exhaust valve actuation |

> ⚠️ **NO CHECK VALVE** on detection line — Direct vacuum path required

---

## 🛡️ Safety

### Transient Protection Matrix

| Event | Input | Protection | Output | Status |
|-------|:-----:|------------|:------:|:------:|
| Normal Operation | 12-14.8V | Pass-through | 12-14.8V | ✅ |
| Alternator Max | 14.8V | V_WRM margin | 14.8V | ✅ |
| **Jump Start** | **24V** | **Fuse blows** | **Protected** | ⚡ |
| **Load Dump** | **100V/400ms** | **TVS clamp** | **24.4V** | 🛡️ |
| Cold Crank | 6V | 78L05 dropout | Degraded | ⚠️ |

### Component Margins

```
SMC ISE30A    ████████████░░░░░░░░  28V max    → 24.4V actual  (12.9% margin)
78L05         ████████████████░░░░  30V max    → 24.4V actual  (18.7% margin)  
1N5822        ████████████████████  40V max    → 24.4V actual  (39% margin)
```

### ⚠️ Fail-Safe Behavior

```diff
- Power Loss → Solenoid OFF → Valve CLOSES → Turbo Backpressure
! IMMEDIATELY LIFT THROTTLE IF SYSTEM FAILS
```

---

## 📟 Breakout Box V1.5

*ESP32-C6-LCD diagnostic interface with persistent calibration*

### 🆕 V1.5 Features

| Feature | Description |
|---------|-------------|
| 🐕 **Watchdog** | 5s hardware watchdog with auto-reset |
| 💾 **NVS Calibration** | ADC calibration persists across reboots |
| ⏱️ **I2C Timeout** | 100ms timeout prevents INA219 freeze |
| 🔌 **THT Clamps** | 2× 1N5817 replace SMD BAT54S |

### Connector Pinout

<table>
<tr>
<th>Row A — Measurements</th>
<th>Row B — Stimulation</th>
</tr>
<tr>
<td>

| Pin | Signal | Description |
|:---:|--------|-------------|
| 1 | `+12V_PROT` | Protected 12V rail |
| 2 | `+5V_LOG` | Logic supply |
| 3 | `AUTO_REQ` | Pressure switch out |
| 4 | `OUT_DRIVER` | BTS5090 output |
| 5 | `CMD` | OR gate output |
| 6 | `MANUAL_REQ` | Button toggle state |

</td>
<td>

| Pin | Signal | Description |
|:---:|--------|-------------|
| 7 | `GND` | Ground reference |
| 8 | `STIM_1` | Force AUTO high |
| 9 | `STIM_2` | Toggle MANUAL |
| 10 | `I_SENSE+` | Current sense + |
| 11 | `I_SENSE-` | Current sense - |
| 12 | `GND` | Ground reference |

</td>
</tr>
</table>

### Serial Commands

```
L - Live mode (default)     a/A - STIM1 on/off
S - Stress test (20×10Hz)   M   - STIM2 pulse (toggle)
C - Calibration mode        R   - Reset min/max
I - System info             H   - Help
```

---

## 🔧 Installation

### Prerequisites

- ✅ 2016 Ford Mustang EcoBoost with boost reference port
- ✅ 12V ACC-switched power source  
- ✅ Air compressor system (90-120 PSI)
- ✅ Exhaust bypass valve with pneumatic actuator

### Wiring Checklist

```
[ ] +12V_BATT → Battery via 5A fuse (≤15cm from terminal)
[ ] +12V_ACC  → Ignition-switched source
[ ] GND_STAR  → Single point chassis ground
[ ] BOOST     → 6mm tubing from intake manifold
[ ] PNEUMATIC → Compressor → Regulator (1 bar) → MAC 35A → Actuator
```

### Pressure Settings

| Parameter | Value | Notes |
|-----------|:-----:|-------|
| **P_ON** | 2.9-3.6 PSI | Valve opens |
| **P_OFF** | 2.0-2.9 PSI | Valve closes |
| **Hysteresis** | ≥0.7 PSI | Prevents chatter |

---

## 📜 Version History

| Version | Date | Changes |
|:-------:|:----:|---------|
| **v4.7** | Dec 2025 | 🔧 BTS5090 DIP adapter, LED 470Ω brighter, cold crank advisory, Breakout V1.5 |
| v4.6 | Dec 2025 | 🔴 TVS SM5S22A→P6KE18CA, orientation fix, removed conflicting protections |
| v4.5 | Dec 2025 | Added SM5S22A TVS for ISO 7637-2 Pulse 1 |
| v4.4 | Dec 2025 | Added diagnostic LED + TEST button |
| v4.3 | Dec 2025 | Added Breakout Box connector J-BB |
| v4.2 | Dec 2025 | Removed check valve, added SMC IR1000-01BG |

---

## 📁 Documentation

| File | Description |
|------|-------------|
| [`VALVE_v4_7.md`](./VALVE_v4_7.md) | 📄 Complete circuit documentation |
| [`BOM_VALVE_v4_7.md`](./BOM_VALVE_v4_7.md) | 📋 Bill of Materials |
| [`BreakoutBox_Circuit_V1_5.md`](./BreakoutBox_Circuit_V1_5.md) | 🔌 Diagnostic tool schematic |
| [`BreakoutBox_V1_5.ino`](./BreakoutBox_V1_5.ino) | 💻 ESP32-C6 firmware |
| [`PROTOCOLE_PREMORTEM_V4_2_2.txt`](./PROTOCOLE_PREMORTEM_V4_2_2.txt) | ✅ Design validation protocol |

---

<div align="center">

## ⚠️ Disclaimer

**This system controls exhaust flow on a high-power turbocharged vehicle.**

Improper installation or failure can cause engine damage or safety hazards.

*Professional installation recommended.*

<br>

---

<br>

**Made with 🔥 for the Mustang community**

*Mehdi — December 2025*

<br>

[![GitHub](https://img.shields.io/badge/Personal_Project-Not_for_Commercial_Use-lightgrey?style=flat-square)](https://github.com)

</div>
