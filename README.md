# 🐎 Mustang EcoBoost Exhaust Valve Controller

### *When 600+ HP needs to breathe*

---

<p align="center">
  <img src="https://img.shields.io/badge/version-4.9-0366d6?style=for-the-badge&logo=v&logoColor=white" alt="Version">
  <img src="https://img.shields.io/badge/status-PRODUCTION__READY-28a745?style=for-the-badge&logo=checkmarx&logoColor=white" alt="Status">
  <img src="https://img.shields.io/badge/ISO_7637--2-COMPLIANT-ff6b00?style=for-the-badge&logo=iso&logoColor=white" alt="ISO">
  <img src="https://img.shields.io/badge/AEC--Q100-CERTIFIED-dc3545?style=for-the-badge&logo=lightning&logoColor=white" alt="AEC-Q100">
</p>

<p align="center">
  <img src="https://img.shields.io/badge/Ford-Mustang_EcoBoost-00529b?style=flat-square&logo=ford&logoColor=white" alt="Mustang">
  <img src="https://img.shields.io/badge/Stage_4+-E85_Flex_Fuel-e85b00?style=flat-square&logo=fires&logoColor=white" alt="Tuning">
  <img src="https://img.shields.io/badge/600+-Horsepower-dc3545?style=flat-square&logo=lightning&logoColor=white" alt="Power">
  <img src="https://img.shields.io/badge/build-THT_Stripboard-6f42c1?style=flat-square&logo=arduino&logoColor=white" alt="Build">
</p>

---

```
    ___  ___          _                      
    |  \/  |         | |                     
    | .  . |_   _ ___| |_ __ _ _ __   __ _   
    | |\/| | | | / __| __/ _` | '_ \ / _` |  
    | |  | | |_| \__ \ || (_| | | | | (_| |  
    \_|  |_/\__,_|___/\__\__,_|_| |_|\__, |  
                                      __/ |  
     EXHAUST VALVE CONTROLLER v4.9   |___/
```

<p align="center">
  <strong>Automatic boost-activated exhaust bypass system</strong><br>
  <em>Fail-Safe • Automotive-Grade • Load Dump Protected • DIY Friendly</em>
</p>

<p align="center">
  <a href="#-documentation">📖 Documentation</a> •
  <a href="#-installation">🔧 Installation</a> •
  <a href="#-quick-start">⚡ Quick Start</a> •
  <a href="#%EF%B8%8F-safety">🛡️ Safety</a> •
  <a href="#-breakout-box">📟 Breakout Box</a>
</p>

---

## 🚀 What's New in v4.9

> **CRITICAL UPDATE** — Load dump protection validated to automotive standards

| Change | Before | After | Impact |
|--------|--------|-------|--------|
| 🔴 **5V Regulator** | MCP1702 (14.5V max) | **NCV2931** (60V transient) | Survives load dump |
| 🟠 **PRV Safety** | Optional | **MANDATORY** | Mechanical fail-safe |
| 🟢 **Qualification** | Generic | **AEC-Q100** | Automotive certified |

```diff
- MCP1702 Vin_abs_max = 14.5V < TVS clamp 25.2V = 💥 DESTRUCTION
+ NCV2931 Vin_transient = 60V >> TVS clamp 25.2V = ✅ SAFE
```

⚠️ **PINOUT WARNING**: NCV2931 has **DIFFERENT pinout** than MCP1702!
```
MCP1702: 1=GND, 2=Vout, 3=Vin
NCV2931: 1=Vout, 2=GND, 3=Vin  ← PINS 1&2 SWAPPED!
```

---

## 📊 Specifications at a Glance

<table>
<tr>
<td align="center">
  <img src="https://img.shields.io/badge/2.9--3.6-PSI-dc3545?style=for-the-badge" alt="Boost"><br>
  <strong>Boost Threshold</strong>
</td>
<td align="center">
  <img src="https://img.shields.io/badge/100V-400ms-ffc107?style=for-the-badge" alt="Load Dump"><br>
  <strong>Load Dump Protection</strong>
</td>
<td align="center">
  <img src="https://img.shields.io/badge/25.2V-Clamp-28a745?style=for-the-badge" alt="Clamp"><br>
  <strong>TVS Clamping</strong>
</td>
<td align="center">
  <img src="https://img.shields.io/badge/--40°C_to_+125°C-Operating-17a2b8?style=for-the-badge" alt="Temp"><br>
  <strong>Temperature Range</strong>
</td>
</tr>
</table>

---

## 🎯 Features

<table>
<tr>
<td width="50%">

### 🚗 Automatic Mode
Opens valve when SMC ISE30A detects boost above configurable threshold

### 🕹️ Manual Override  
Toggle valve state anytime with OEM-style illuminated Toyota button

### 🛡️ Fail-Safe Design
Pneumatic actuator is **fail-closed** — valve closes on power loss

### 🔥 Load Dump Hardened
Survives +100V/400ms transients per ISO 7637-2 Pulse 1

</td>
<td width="50%">

### ⚡ Automotive-Grade
**AEC-Q100** qualified regulator, full ISO 7637-2 compliance

### 🔍 Quick Diagnostic
5-second visual check: LED + TEST button validates entire signal chain

### 📟 Breakout Box Ready
ESP32-C6 diagnostic interface with INA219 current sensing

### 🔧 DIY Friendly
100% through-hole components on standard stripboard

</td>
</tr>
</table>

---

## 🏗️ System Architecture

```
┌─────────────────────────────────────────────────────────────────────────────┐
│                          VALVE CONTROLLER v4.9                              │
├─────────────────────────────────────────────────────────────────────────────┤
│                                                                             │
│   ┌─────────┐    ┌─────────┐    ┌─────────┐    ┌─────────┐    ┌─────────┐  │
│   │ +12V    │───▶│1.5KE18CA│───▶│ FERRITE │───▶│ FUSE 1A │───▶│ 1N5822  │  │
│   │ BATTERY │    │ 1500W   │    │ 90Ω EMI │    │ PROTECT │    │ REVERSE │  │
│   └─────────┘    └─────────┘    └─────────┘    └─────────┘    └────┬────┘  │
│                                                                     │       │
│        ┌────────────────────────────────────────────────────────────┘       │
│        ▼                                                                    │
│   ┌─────────┐         ┌─────────────────────────────────────────────────┐  │
│   │NCV2931  │────────▶│                  +5V LOGIC                      │  │
│   │ 60V AEC │         └─────────────────────────────────────────────────┘  │
│   └─────────┘                     │                    │                    │
│                                   ▼                    ▼                    │
│   ┌─────────┐    ┌─────────┐    ┌─────────┐    ┌─────────┐    ┌─────────┐  │
│   │ BOOST   │───▶│ SMC     │───▶│ 74HC14  │───▶│   OR    │───▶│ BTS5090 │  │
│   │ INTAKE  │    │ ISE30A  │    │ BUFFER  │    │  GATE   │    │ DRIVER  │  │
│   └─────────┘    └─────────┘    └─────────┘    └────┬────┘    └────┬────┘  │
│                                                     │              │       │
│   ┌─────────┐    ┌─────────┐                       │              ▼       │
│   │ TOYOTA  │───▶│ CD4013  │───────────────────────┘         ┌─────────┐  │
│   │ BUTTON  │    │ TOGGLE  │                                 │ MAC 35A │  │
│   └─────────┘    └─────────┘                                 │ SOLENOID│  │
│                                                              └────┬────┘  │
│   ┌─────────────────────────────────────────────────────────────┐ │       │
│   │ 🟢 LED POWER        🔘 TEST BUTTON        📟 J-BB BREAKOUT  │ │       │
│   └─────────────────────────────────────────────────────────────┘ │       │
│                                                                   │       │
│   ┌───────────────────────────────────────────────────────────────┘       │
│   │                                                                       │
│   ▼     🆕 PRV 3 bar MANDATORY                                            │
│   ┌─────────┐    ┌─────────┐    ┌─────────┐                               │
│   │ SMC IR  │───▶│ PRV 3bar│───▶│ACTUATOR │                               │
│   │ 1 bar   │    │FAIL-SAFE│    │ EXHAUST │                               │
│   └─────────┘    └─────────┘    └─────────┘                               │
└─────────────────────────────────────────────────────────────────────────────┘
```

---

## 🔌 Hardware

### Protection Chain (BLOC A) — *v4.9 Certified*

| Stage | Component | Function | Specs | Package |
|:-----:|-----------|----------|-------|---------|
| **1** | `1.5KE18CA` | TVS Clamping | V_C = 25.2V @ 60A, **1500W** | DO-15 |
| **2** | `Würth 742792093` | EMI Filter | 90Ω @ 100MHz | Axial |
| **3** | `Fuse 1A T` | Overcurrent | Fast-blow | 5×20mm |
| **4** | `1N5822` | Reverse Polarity | 40V 3A Schottky | DO-201AD |

> ⚠️ **CRITICAL**: TVS orientation — **Cathode (band) → +12V_IN** / **Anode → GND**

### 5V Regulator (BLOC B) — *v4.9 UPGRADED*

| Parameter | MCP1702 ❌ | NCV2931 ✅ | Requirement |
|-----------|------------|------------|-------------|
| **Vin transient** | 14.5V | **60V** | > 25.2V (TVS clamp) |
| **Dropout** | 625mV | 600mV | < 1V @ 6V cold crank |
| **Qualification** | Commercial | **AEC-Q100** | Automotive |
| **Package** | TO-92 | TO-92 | THT compatible |

### Logic & Control

| Block | Component | Package | Function |
|:-----:|-----------|---------|----------|
| **B** | NCV2931AZ-5.0G | TO-92 | 5V Regulation (60V transient!) |
| **C** | SMC ISE30A-01-N | — | Boost Pressure Sensing |
| **C** | 74HC14 | DIP-14 | Schmitt Buffer |
| **D** | Toyota 22×22mm | — | Illuminated Push Button |
| **E** | CD4013 | DIP-14 | Toggle Flip-Flop |
| **G** | BTS5090-1EJA | TSSOP-8 + DIP adapter | High-Side Smart Driver |

### Pneumatic System

| Component | Reference | Function |
|-----------|-----------|----------|
| Pressure Switch | SMC ISE30A-01-N | Boost detection (NPN, 12-24V) |
| Pressure Regulator | SMC IR1000-01BG | Compressor → 1 bar output |
| **PRV** 🆕 | Brass 3 bar preset | **MANDATORY fail-safe** |
| Solenoid Valve | MAC 35A 12V | Pneumatic control (5.4W) |
| Actuator | Fail-closed | Exhaust valve actuation |

> ⚠️ **NO CHECK VALVE** on detection line — Direct vacuum path required

---

## 🛡️ Safety

### Transient Protection Matrix

| Event | Input | Protection | Output | Status |
|-------|-------|------------|--------|:------:|
| Normal Operation | 12-14.8V | Pass-through | 12-14.8V | ✅ |
| Alternator Max | 14.8V | V_WRM margin | 14.8V | ✅ |
| **Jump Start** | **24V** | **Fuse blows** | **Protected** | ⚡ |
| **Load Dump** | **100V/400ms** | **TVS clamp** | **25.2V** | 🛡️ |
| Cold Crank | 6V | NCV2931 dropout | 5V stable | ✅ |

### Component Margins v4.9

```
1.5KE18CA     ████████████████████  25.2V clamp  → ISO 7637-2 OK  (1500W)
NCV2931       ████████████████████  60V max      → 25.2V actual   (58% margin)  
1N5822        ████████████████████  40V max      → 25.2V actual   (37% margin)
SMC ISE30A    ████████████████░░░░  28V max      → 25.2V actual   (10% margin)
```

### ⚠️ Fail-Safe Behavior

```
┌─────────────────────────────────────────────────────────────────┐
│  FAILURE MODE                    │  RESULT           │ ACTION  │
├─────────────────────────────────────────────────────────────────┤
│  Power Loss                      │  Valve CLOSES     │ Safe    │
│  BTS5090 Short (MOSFET grilled)  │  Solenoid ON      │ PRV!    │
│  Regulator Overpressure          │  PRV vents @ 3bar │ Safe    │
└─────────────────────────────────────────────────────────────────┘
```

> 🔴 **CRITICAL**: Without PRV, BTS5090 failure = unlimited pressure on actuator!

```
! IMMEDIATELY LIFT THROTTLE IF SYSTEM FAILS
```

---

## 📟 Breakout Box v1.6

*ESP32-C6-LCD diagnostic interface with automated validation*

### Hardware

| Component | Reference | Function |
|-----------|-----------|----------|
| MCU | ESP32-C6-LCD-1.47 | Display + WiFi + GPIO |
| Current Sensor | INA219 | MAC 35A monitoring |
| Connector | 2×6 pin header | J-BB interface |

### Firmware v1.6 Features

| Feature | Description | Status |
|---------|-------------|:------:|
| **INA219 Validation** | Range check [-100, 1000] mA | ✅ |
| **I2C Recovery** | 3 consecutive errors → invalidate | ✅ |
| **NVS Protection** | `isfinite()` + range [0.8, 1.2] | ✅ |
| **ADC Fault Detection** | raw<10 → sensor HS, v>16.5V → saturated | ✅ |
| **Calibration Guard** | Division by zero protection | ✅ |
| **STIM Timeout** | Auto-OFF after 60s | ✅ |
| **Watchdog** | 5s hardware reset | ✅ |

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
| 8 | `STIM_1` | After 1k resistor |
| 9 | `STIM_2` | Before 10k resistor |
| 10 | `I_SENSE+` | Current sense + |
| 11 | `I_SENSE-` | Current sense - |
| 12 | `GND` | Ground reference |

</td>
</tr>
</table>

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
[ ] PNEUMATIC → Compressor → Regulator (1 bar) → PRV (3 bar) → MAC 35A → Actuator
```

### Pressure Settings

| Parameter | Value | Notes |
|-----------|-------|-------|
| **P_ON** | 2.9-3.6 PSI | Valve opens |
| **P_OFF** | 2.0-2.9 PSI | Valve closes |
| **Hysteresis** | ≥0.7 PSI | Prevents chatter |
| **PRV** | 3 bar | Fail-safe limit |

---

## ⚡ Quick Start

### 5-Second Diagnostic

```
1. 🟢 LED POWER on?      → If NO: Check fuse/relay/NCV2931
2. 🔘 Press TEST button  → Toyota LED ON?  → If NO: Check 74HC14/OR/BTS5090
3. 🔘 Release TEST       → Toyota LED OFF? → System OK ✓
```

### First Boot Checklist

- [ ] Verify +5V_LOG = 4.9-5.1V
- [ ] Cold crank test: 6V input → LED still ON
- [ ] Load dump simulation: 25V transient → no damage
- [ ] PRV installed and oriented downward
- [ ] No check valve on vacuum line

---

## 📜 Version History

| Version | Date | Changes |
|---------|------|---------|
| **v4.9** | Dec 2025 | 🔴 **CRITICAL**: NCV2931 (60V) replaces MCP1702 (14.5V), PRV mandatory |
| v4.8 | Dec 2025 | MCP1702 cold crank, 1.5KE18CA 1500W, PRV optional |
| v4.7 | Dec 2025 | BTS5090 + DIP adapter, LED POWER 470Ω |
| v4.6 | Dec 2025 | Breakout Box connector J-BB |
| v4.5 | Dec 2025 | SM5S22A TVS for ISO 7637-2 |
| v4.4 | Dec 2025 | Diagnostic LED + TEST button |

### Firmware Changelog

| Version | Date | Changes |
|---------|------|---------|
| **v1.6** | Dec 2025 | INA219 validation, NVS protection, ADC fault detection, STIM timeout |
| v1.5 | Dec 2025 | NVS calibration, Watchdog 5s, I2C timeout |
| v1.4 | Dec 2025 | Stress test mode, Min/Max hold |

---

## 📁 Documentation

| File | Description |
|------|-------------|
| [`docs/VALVE_v4_9.md`](docs/VALVE_v4_9.md) | 📄 Complete circuit documentation |
| [`docs/BOM_VALVE_v4_9.md`](docs/BOM_VALVE_v4_9.md) | 📋 Bill of Materials |
| [`hardware/BreakoutBox_Circuit_V1_5.md`](hardware/BreakoutBox_Circuit_V1_5.md) | 🔌 Diagnostic tool schematic |
| [`firmware/BreakoutBox_V1_6.ino`](firmware/BreakoutBox_V1_6.ino) | 💻 ESP32-C6 firmware |
| [`docs/PROTOCOLE_PREMORTEM_V4_2_3.txt`](docs/PROTOCOLE_PREMORTEM_V4_2_3.txt) | ✅ Design validation protocol |

---

## 🔬 Design Philosophy

This project follows the **PREMORTEM V4.2.3** validation protocol:

- **Zero tolerance** on critical component specs
- **Datasheet-first** design decisions
- **Net tracing** before any critique
- **Skills validation** for active components
- **Through-hole only** for DIY accessibility

```
"If it can fail, it must fail safely."
```

---

## ⚠️ Disclaimer

**This system controls exhaust flow on a high-power turbocharged vehicle.**

Improper installation or failure can cause:
- Engine damage from backpressure
- Turbo damage from heat soak
- Fire hazard from electrical faults

*Professional installation recommended. Use at your own risk.*

---

<p align="center">
  <strong>Made with 🔥 for the Mustang community</strong><br>
  <em>Mehdi — December 2025</em>
</p>

<p align="center">
  <img src="https://img.shields.io/badge/Personal_Project-Not_for_Commercial_Use-lightgrey?style=flat-square" alt="License">
  <img src="https://img.shields.io/badge/License-MIT-blue?style=flat-square" alt="MIT">
</p>

---

<p align="center">
  <sub>
    🏎️ Stage 4+ E85 • 600+ HP • ISO 7637-2 • AEC-Q100 • Fail-Safe Design
  </sub>
</p>
