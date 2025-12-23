<div align="center">

# 🐎 Mustang EcoBoost Exhaust Valve Controller

### *When 600+ HP needs to breathe*

<br/>

[![Version](https://img.shields.io/badge/version-4.10-0366d6?style=for-the-badge&logo=v&logoColor=white)](https://github.com)
[![Status](https://img.shields.io/badge/status-PRODUCTION_READY-28a745?style=for-the-badge&logo=checkmarx&logoColor=white)](https://github.com)
[![ISO](https://img.shields.io/badge/ISO_7637--2-COMPLIANT-ff6b00?style=for-the-badge&logo=iso&logoColor=white)](https://github.com)

<br/>

[![Mustang](https://img.shields.io/badge/Ford-Mustang_EcoBoost-00529b?style=flat-square&logo=ford&logoColor=white)](https://github.com)
[![Tuning](https://img.shields.io/badge/Stage_4+-E85_Flex_Fuel-e85b00?style=flat-square&logo=fires&logoColor=white)](https://github.com)
[![Power](https://img.shields.io/badge/600+-Horsepower-dc3545?style=flat-square&logo=lightning&logoColor=white)](https://github.com)
[![Build](https://img.shields.io/badge/build-THT_Stripboard-6f42c1?style=flat-square&logo=arduino&logoColor=white)](https://github.com)

<br/>

```
    ___  ___          _                      
    |  \/  |         | |                     
    | .  . |_   _ ___| |_ __ _ _ __   __ _   
    | |\/| | | | / __| __/ _` | '_ \ / _` |  
    | |  | | |_| \__ \ || (_| | | | | (_| |  
    \_|  |_/\__,_|___/\__\__,_|_| |_|\__, |  
                                      __/ |  
     EXHAUST VALVE CONTROLLER v4.10  |___/
```

<br/>

**Automatic boost-activated exhaust bypass with anti-flutter technology**

*Fail-Safe • Automotive-Grade • Cold-Crank Certified • Plug & Play*

<br/>

[📖 Documentation](#-documentation) •
[🔧 Installation](#-installation) •
[⚡ Quick Start](#-quick-start) •
[🛡️ Safety](#%EF%B8%8F-safety) •
[🔌 Breakout Box](#-breakout-box-v18)

<br/>

</div>

---

## 🆕 What's New in v4.10

<table>
<tr>
<td width="50%">

### ⏱️ Anti-Flutter Technology
**2-second closing delay** eliminates valve chatter during:
- Downshift auto-blip (200-500ms boost spikes)
- Gear changes (0.5-1s throttle lift)
- Traffic hesitation (oscillating throttle)
- **Instant opening** — zero backpressure delay

</td>
<td width="50%">

### 🧊 Cold-Crank Certified
**NCV2931 regulator** with 60V transient rating:
- Survives -40°C cold starts at 6V
- 625mV dropout vs 2V for 78L05
- Full automotive temperature range
- **No more brown-outs**

</td>
</tr>
</table>

---

## 📊 Quick Stats

<div align="center">

|  |  |  |  |
| :---: | :---: | :---: | :---: |
| ![Boost](https://img.shields.io/badge/2.9--3.6-PSI-dc3545?style=for-the-badge) | ![Load Dump](https://img.shields.io/badge/100V-400ms-ffc107?style=for-the-badge) | ![Clamp](https://img.shields.io/badge/25.2V-Clamp-28a745?style=for-the-badge) | ![Temp](https://img.shields.io/badge/--40°C_to_+85°C-Operating-17a2b8?style=for-the-badge) |
| **Boost Threshold** | **Load Dump Protection** | **TVS Clamping** | **Temperature Range** |

</div>

---

## 🎯 Features

<table>
<tr>
<td width="50%">

### 🚗 Automatic Mode
Opens valve when SMC ISE30A pressure switch detects boost above threshold (2.9 PSI default)

### 🕹️ Manual Override  
Toggle valve state anytime with OEM-style illuminated Toyota push button

### 🛡️ Fail-Safe Design
Pneumatic actuator is **fail-closed** — valve closes on power loss, protecting your turbo

</td>
<td width="50%">

### ⚡ Automotive-Grade
Full ISO 7637-2 transient protection including load dump (+100V) and cold crank (6V)

### ⏱️ Anti-Flutter Circuit
2-second closing delay smooths out boost oscillations — **instant opening**, delayed closing

### 🔧 DIY Friendly
100% through-hole components on standard stripboard — no SMD soldering required*

</td>
</tr>
</table>

<sub>*BTS5090 uses TSSOP-to-DIP adapter included in BOM</sub>

---

## 🏗️ System Architecture

```
┌─────────────────────────────────────────────────────────────────────────────────┐
│                          VALVE CONTROLLER v4.10                                 │
├─────────────────────────────────────────────────────────────────────────────────┤
│                                                                                 │
│   ┌─────────┐    ┌─────────┐    ┌─────────┐    ┌─────────┐    ┌─────────┐      │
│   │ +12V    │───▶│1.5KE18CA│───▶│ FERRITE │───▶│ FUSE 1A │───▶│ 1N5822  │      │
│   │ BATTERY │    │ 1500W   │    │ 90Ω EMI │    │ PROTECT │    │ REVERSE │      │
│   └─────────┘    └─────────┘    └─────────┘    └─────────┘    └────┬────┘      │
│                                                                     │           │
│        ┌────────────────────────────────────────────────────────────┘           │
│        ▼                                                                        │
│   ┌─────────┐         ┌──────────────────────────────────────────────────────┐  │
│   │NCV2931  │────────▶│                    +5V LOGIC                         │  │
│   │ 60V     │         └──────────────────────────────────────────────────────┘  │
│   └─────────┘                     │                    │                        │
│                                   ▼                    ▼                        │
│   ┌─────────┐    ┌─────────┐    ┌─────────┐    ┌─────────┐    ┌─────────┐      │
│   │ BOOST   │───▶│ SMC     │───▶│ 74HC14  │───▶│ TEMPO   │───▶│ BTS5090 │      │
│   │ INTAKE  │    │ ISE30A  │    │ BUFFER  │    │ 2s RC   │    │ DRIVER  │      │
│   └─────────┘    └─────────┘    └─────────┘    └────┬────┘    └────┬────┘      │
│                                                     │              │           │
│   ┌─────────┐    ┌─────────┐         ┌─────────────┴──────────────┤           │
│   │ TOYOTA  │───▶│ CD4013  │────────▶│        OR GATE             │           │
│   │ BUTTON  │    │ TOGGLE  │         └────────────────────────────┘           │
│   └─────────┘    └─────────┘                                      │           │
│                                                                   ▼           │
│   ┌───────────────────────────────────────────────────────────┐  ┌─────────┐  │
│   │ 🟢 LED    🔘 TEST    📟 J-BB (JST 2×6)    🔌 DB-15       │  │ MAC 35A │  │
│   └───────────────────────────────────────────────────────────┘  │SOLENOID │  │
│                                                                  └────┬────┘  │
│                                                                       ▼       │
│                                                                  ACTUATOR     │
└─────────────────────────────────────────────────────────────────────────────────┘
```

---

## 🔌 Hardware

### Protection Chain (BLOC A) — *v4.10 Certified*

| Stage | Component | Function | Specs | Package |
|:---:|---|---|---|:---:|
| **1** | `1.5KE18CA` | TVS Clamping | V_C = 25.2V @ 60A, **1500W** | DO-15 |
| **2** | `Würth 742792093` | EMI Filter | 90Ω @ 100MHz | Axial |
| **3** | `Fuse 1A T` | Overcurrent | Fast-blow | 5×20mm |
| **4** | `1N5822` | Reverse Polarity | 40V 3A Schottky | DO-201AD |

> ⚠️ **CRITICAL**: TVS orientation — **Cathode (band) → +12V_IN** / **Anode → GND**

### Power & Logic

| Block | Component | Package | Function | Key Spec |
|:---:|---|:---:|---|---|
| **B** | NCV2931AZ-5.0G | TO-92 | 5V Regulation | **60V transient** |
| **C** | SMC ISE30A-01-N | — | Boost Pressure Sensing | 2.9 PSI threshold |
| **C** | 74HC14 | DIP-14 | Schmitt Buffer | 6 inverters |
| **D** | Toyota 22×22mm | — | Illuminated Push Button | Blue LED |
| **E** | CD4013 | DIP-14 | Toggle Flip-Flop | Manual mode |
| **F** | RC Tempo | — | Anti-Flutter Delay | **2s closing** |
| **G** | BTS5090-1EJA | TSSOP-8* | High-Side Smart Driver | OC/OT/ESD |

<sub>*Mounted on TSSOP-to-DIP adapter (Aries 08-350000-11)</sub>

### Pneumatic System

| Component | Reference | Function |
|---|---|---|
| Pressure Switch | SMC ISE30A-01-N | Boost detection (NPN, 12-24V) |
| Pressure Regulator | SMC IR1000-01BG | Compressor → 1 bar output |
| Solenoid Valve | MAC 35A 12V | Pneumatic control (5.4W) |
| PRV | 3 bar | **MANDATORY** safety relief |
| Actuator | Fail-closed | Exhaust valve actuation |

> ⚠️ **NO CHECK VALVE** on detection line — Direct vacuum path required

---

## 🛡️ Safety

### Transient Protection Matrix

| Event | Input | Protection | Output | Status |
|---|:---:|---|:---:|:---:|
| Normal Operation | 12-14.8V | Pass-through | 12-14.8V | ✅ |
| Alternator Max | 14.8V | V_WRM margin | 14.8V | ✅ |
| **Cold Crank** | **6V** | **NCV2931 625mV dropout** | **5V stable** | 🧊 |
| **Jump Start** | **24V** | **Fuse blows** | **Protected** | ⚡ |
| **Load Dump** | **100V/400ms** | **TVS clamp** | **25.2V** | 🛡️ |

### Component Margins

```
NCV2931       ████████████████████████░░  60V trans  → 25.2V actual (58% margin)
SMC ISE30A    █████████████░░░░░░░░░░░░░  28V max    → 25.2V actual (10% margin)
1N5822        ████████████████████░░░░░░  40V max    → 25.2V actual (37% margin)
BTS5090       ████████████████████████░░  42V max    → 25.2V actual (40% margin)
```

### ⚠️ Fail-Safe Behavior

```diff
- Power Loss → Solenoid OFF → Valve CLOSES → Turbo Backpressure
! IMMEDIATELY LIFT THROTTLE IF SYSTEM FAILS
```

---

## 📟 Breakout Box V1.8

*ESP32-C6 Touch LCD diagnostic interface with universal DB-15 connector*

<table>
<tr>
<td>

### 🖥️ Hardware
- **ESP32-C6-Touch-LCD-1.47** (170×320 color)
- **INA219** current sensing (±3.2A / 0.8mA resolution)
- **DB-15** universal connector (screw terminals)
- **USB-C** panel mount power

</td>
<td>

### 📊 Capabilities
- Real-time voltage/current monitoring
- Digital I/O stimulation
- Touch interface for test selection
- Automated validation sequences

</td>
</tr>
</table>

### DB-15 Pinout (Universal Interface)

| Pin | Signal | Type | Pin | Signal | Type |
|:---:|---|:---:|:---:|---|:---:|
| 1 | +12V | PWR | 9 | DIN3 | GPIO |
| 2 | +5V | PWR | 10 | DOUT1 | GPIO |
| 3 | GND | PWR | 11 | DOUT2 | GPIO |
| 4 | AIN1 (0-20V) | ADC | 12 | I+ | SENSE |
| 5 | AIN2 (0-10V) | ADC | 13 | I- | SENSE |
| 6 | AIN3 (0-3.3V) | ADC | 14 | GND | PWR |
| 7 | DIN1 | GPIO | 15 | SPARE | — |
| 8 | DIN2 | GPIO | | | |

---

## 🔧 Installation

### Prerequisites

- ✅ 2015-2023 Ford Mustang EcoBoost with boost reference port
- ✅ 12V ACC-switched power source
- ✅ Air compressor system (90-120 PSI)
- ✅ Exhaust bypass valve with pneumatic actuator
- ✅ **3 bar PRV** (pressure relief valve) — MANDATORY

### Wiring Checklist

```
[ ] +12V_BATT → Battery via 5A fuse (≤15cm from terminal)
[ ] +12V_ACC  → Ignition-switched source (relay coil)
[ ] GND_STAR  → Single point chassis ground (≤30cm wire)
[ ] BOOST     → 6mm tubing from intake manifold
[ ] PNEUMATIC → Compressor → Regulator (1 bar) → PRV (3 bar) → MAC 35A → Actuator
```

### Pressure Settings (ISE30A)

| Parameter | Value | Display | Notes |
|---|:---:|:---:|---|
| **P_ON (P1)** | 20.0 kPa | 0.20 bar | Valve opens |
| **Hysteresis** | 5.0 kPa | HYST | Prevents chatter |
| **P_OFF** | 15.0 kPa | Auto | Valve closes |

> 💡 Highway cruise ~10 kPa (0.1 bar) = valve stays closed

---

## 📁 Documentation

| File | Description |
|---|---|
| [`hardware/VALVE_v4_10.md`](hardware/VALVE_v4_10.md) | 📄 Complete circuit documentation |
| [`hardware/BOM_VALVE_v4_10.md`](hardware/BOM_VALVE_v4_10.md) | 📋 Bill of Materials (~€45 total) |
| [`hardware/BreakoutBox_V1_8.md`](hardware/BreakoutBox_Circuit_V1_8.md) | 🔌 Diagnostic tool schematic |
| [`firmware/BreakoutBox_V1_6.ino`](firmware/BreakoutBox_V1_6.ino) | 💻 ESP32-C6 firmware |
| [`docs/PROTOCOLE_PREMORTEM_V4_2_4.txt`](docs/PROTOCOLE_PREMORTEM_V4_2_4.txt) | ✅ Design validation protocol |

---

## 📜 Version History

| Version | Date | Highlights |
|:---:|---|---|
| **v4.10** | Dec 2025 | ⏱️ **Anti-flutter tempo circuit** (2s closing delay) |
| v4.9 | Dec 2025 | 🧊 NCV2931 cold-crank certified regulator |
| v4.8 | Dec 2025 | 🔌 JST XH 2×6 breakout connector |
| v4.7 | Dec 2025 | 📟 BreakoutBox V1.8 with ESP32-C6 Touch LCD |
| v4.6 | Dec 2025 | 🛡️ P6KE18CA→1.5KE18CA TVS upgrade |

<details>
<summary>📋 Full changelog</summary>

- **v4.5** — Added TVS for ISO 7637-2 Pulse 1
- **v4.4** — Diagnostic LED + TEST button
- **v4.3** — Breakout Box connector J-BB
- **v4.2** — Removed check valve, added SMC IR1000-01BG
- **v4.1** — BTS5090 smart driver integration
- **v4.0** — Complete redesign for automotive compliance

</details>

---

## ⚠️ Disclaimer

<div align="center">

**This system controls exhaust flow on a high-power turbocharged vehicle.**

Improper installation or failure can cause engine damage or safety hazards.

*Professional installation recommended.*

<br/>

---

<br/>

**Made with 🔥 for the Mustang community**

*Mehdi — December 2025*

<br/>

[![License](https://img.shields.io/badge/License-MIT-green.svg?style=flat-square)](LICENSE)
[![Personal](https://img.shields.io/badge/Personal_Project-Not_for_Commercial_Use-lightgrey?style=flat-square)](https://github.com)

</div>
