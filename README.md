# 🐎 Mustang EcoBoost Exhaust Valve Controller

### *When 600+ HP needs to breathe*

&nbsp;

[![Version](https://img.shields.io/badge/version-4.8-0366d6?style=for-the-badge&logo=v&logoColor=white)](https://github.com)
[![Status](https://img.shields.io/badge/status-PRODUCTION_READY-28a745?style=for-the-badge&logo=checkmarx&logoColor=white)](https://github.com)
[![ISO](https://img.shields.io/badge/ISO_7637--2-COMPLIANT-ff6b00?style=for-the-badge&logo=iso&logoColor=white)](https://github.com)

&nbsp;

[![Mustang](https://img.shields.io/badge/Ford-Mustang_EcoBoost-00529b?style=flat-square&logo=ford&logoColor=white)](https://github.com)
[![Tuning](https://img.shields.io/badge/Stage_4+-E85_Flex_Fuel-e85b00?style=flat-square&logo=fires&logoColor=white)](https://github.com)
[![Power](https://img.shields.io/badge/600+-Horsepower-dc3545?style=flat-square&logo=lightning&logoColor=white)](https://github.com)
[![Build](https://img.shields.io/badge/build-THT_Stripboard-6f42c1?style=flat-square&logo=arduino&logoColor=white)](https://github.com)

&nbsp;

```
    ___  ___          _                      
    |  \/  |         | |                     
    | .  . |_   _ ___| |_ __ _ _ __   __ _   
    | |\/| | | | / __| __/ _` | '_ \ / _` |  
    | |  | | |_| \__ \ || (_| | | | | (_| |  
    \_|  |_/\__,_|___/\__\__,_|_| |_|\__, |  
                                      __/ |  
     EXHAUST VALVE CONTROLLER v4.8   |___/
```

&nbsp;

**Automatic boost-activated exhaust bypass system**

*Fail-Safe • Automotive-Grade • Plug & Play*

&nbsp;

[📖 Documentation](#-documentation) •
[🔧 Installation](#-installation) •
[⚡ Quick Start](#-quick-start) •
[🛡️ Safety](#%EF%B8%8F-safety)

&nbsp;

---

## 📊 Quick Stats

|  |  |  |  |
| --- | --- | --- | --- |
| ![Boost](https://img.shields.io/badge/2.9--3.6-PSI-dc3545?style=for-the-badge) <br> **Boost Threshold** | ![Load Dump](https://img.shields.io/badge/100V-400ms-ffc107?style=for-the-badge) <br> **Load Dump Protection** | ![Clamp](https://img.shields.io/badge/25.2V-Clamp-28a745?style=for-the-badge) <br> **TVS Clamping** | ![Temp](https://img.shields.io/badge/--40°C_to_+85°C-Operating-17a2b8?style=for-the-badge) <br> **Temperature Range** |

---

## 🎯 Features

|  |  |
| --- | --- |
| 🚗 **Automatic Mode** <br> Opens valve when SMC ISE30A pressure switch detects boost above configurable threshold | ⚡ **Automotive-Grade** <br> Full ISO 7637-2 transient protection including load dump (+100V) and cold crank (6V) |
| 🕹️ **Manual Override** <br> Toggle valve state anytime with OEM-style illuminated Toyota push button | 🔍 **Quick Diagnostic** <br> 5-second visual check: LED + TEST button validates entire signal chain |
| 🛡️ **Fail-Safe Design** <br> Pneumatic actuator is fail-closed — valve closes on power loss, protecting your turbo | 🔧 **DIY Friendly** <br> 100% through-hole components on standard stripboard — no SMD soldering required* |

> *\*BTS5090 requires TSSOP-8 to DIP-8 adapter (Aries 08-350000-11)*

---

## 🏗️ System Architecture

```
┌─────────────────────────────────────────────────────────────────────────────┐
│                          VALVE CONTROLLER v4.8                              │
├─────────────────────────────────────────────────────────────────────────────┤
│                                                                             │
│   ┌─────────┐    ┌─────────┐    ┌─────────┐    ┌─────────┐    ┌─────────┐  │
│   │ +12V    │───▶│1.5KE18CA│───▶│ FERRITE │───▶│ FUSE 1A │───▶│ 1N5822  │  │
│   │ BATTERY │    │ TVS 18V │    │ 90Ω EMI │    │ PROTECT │    │ REVERSE │  │
│   └─────────┘    └─────────┘    └─────────┘    └─────────┘    └────┬────┘  │
│                     1500W                                          │       │
│        ┌────────────────────────────────────────────────────────────┘       │
│        ▼                                                                    │
│   ┌─────────┐         ┌─────────────────────────────────────────────────┐  │
│   │ MCP1702 │────────▶│                  +5V LOGIC                      │  │
│   │ LDO 5V  │         └─────────────────────────────────────────────────┘  │
│   └─────────┘                     │                    │                    │
│    625mV dropout                  ▼                    ▼                    │
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
│   └─────────────────────────────────────────────────────────────┘ ▼       │
│                                                              ACTUATOR      │
└─────────────────────────────────────────────────────────────────────────────┘
```

---

## 🔌 Hardware

### Protection Chain (BLOC A) — *v4.8 Upgraded*

| Stage | Component | Function | Specs | Package |
| --- | --- | --- | --- | --- |
| **1** | `1.5KE18CA` | TVS Clamping | V\_C = 25.2V @ 60A, **1500W** | DO-15 |
| **2** | `Würth 742792093` | EMI Filter | 90Ω @ 100MHz | Axial |
| **3** | `Fuse 1A T` | Overcurrent | Slow-blow | 5×20mm |
| **4** | `1N5822` | Reverse Polarity | 40V 3A Schottky | DO-201AD |

> ⚠️ **CRITICAL**: TVS orientation — **Cathode (band) → +12V\_IN** / **Anode → GND**

### Logic & Control

| Block | Component | Package | Function |
| --- | --- | --- | --- |
| **B** | MCP1702-5002E/TO | TO-92 | 5V LDO (625mV dropout) |
| **C** | SMC ISE30A-01-N | — | Boost Pressure Sensing |
| **C** | 74HC14 | DIP-14 | Schmitt Buffer |
| **D** | Toyota 22×22mm | — | Illuminated Push Button |
| **E** | CD4013 | DIP-14 | Toggle Flip-Flop |
| **G** | BTS5090-1EJA | PG-TDSO-8 + Adapter | High-Side Smart Driver |

> ⚠️ **MCP1702 Pinout ≠ 78L05!** — Pin 1=GND, Pin 2=Vout, Pin 3=Vin

### Pneumatic System

| Component | Reference | Function |
| --- | --- | --- |
| Pressure Switch | SMC ISE30A-01-N | Boost detection (NPN, 12-24V) |
| Pressure Regulator | SMC IR1000-01BG | Compressor → 1 bar output |
| Solenoid Valve | MAC 35A 12V | Pneumatic control (5.4W) |
| Actuator | Fail-closed | Exhaust valve actuation |
| PRV *(optional)* | 3 bar brass valve | Overpressure protection |

> ⚠️ **NO CHECK VALVE** on detection line — Direct vacuum path required

---

## 🛡️ Safety

### Transient Protection Matrix

| Event | Input | Protection | Output | Status |
| --- | --- | --- | --- | --- |
| Normal Operation | 12-14.8V | Pass-through | 12-14.8V | ✅ |
| Alternator Max | 14.8V | V\_WRM margin | 14.8V | ✅ |
| **Jump Start** | **24V** | **Fuse blows** | **Protected** | ⚡ |
| **Load Dump** | **100V/400ms** | **TVS clamp 1500W** | **25.2V** | 🛡️ |
| **Cold Crank** | **6V** | **MCP1702 regulates** | **5V stable** | ✅ |

### Component Margins

```
SMC ISE30A    ████████████░░░░░░░░  28V max    → 25.2V actual  (10% margin)
MCP1702       █████████████████░░░  13.2V max  → regulates at 6V input  ✓
1N5822        ████████████████████  40V max    → 25.2V actual  (37% margin)
1.5KE18CA     ████████████████████  1500W peak → load dump covered  ✓
```

### ⚠️ Fail-Safe Behavior

```
╔═══════════════════════════════════════════════════════════════════╗
║  Power Loss → Solenoid OFF → Valve CLOSES → Turbo Backpressure   ║
║                                                                   ║
║  ⚠️  IMMEDIATELY LIFT THROTTLE IF SYSTEM FAILS                   ║
╚═══════════════════════════════════════════════════════════════════╝
```

---

## 📟 Breakout Box Interface (V1.5)

*Diagnostic interface for automated validation testing*

| Row A — Measurements | Row B — Stimulation |
| --- | --- |
| <table><tr><th>Pin</th><th>Signal</th><th>Description</th></tr><tr><td>1</td><td>`+12V_PROT`</td><td>Protected 12V rail</td></tr><tr><td>2</td><td>`+5V_LOG`</td><td>Logic supply</td></tr><tr><td>3</td><td>`AUTO_REQ`</td><td>Pressure switch out</td></tr><tr><td>4</td><td>`OUT_DRIVER`</td><td>BTS5090 output</td></tr><tr><td>5</td><td>`CMD`</td><td>OR gate output</td></tr><tr><td>6</td><td>`MANUAL_REQ`</td><td>Button toggle state</td></tr></table> | <table><tr><th>Pin</th><th>Signal</th><th>Description</th></tr><tr><td>7</td><td>`GND`</td><td>Ground reference</td></tr><tr><td>8</td><td>`STIM_1`</td><td>After 1k resistor</td></tr><tr><td>9</td><td>`STIM_2`</td><td>Before 10k resistor</td></tr><tr><td>10</td><td>`I_SENSE+`</td><td>Current sense +</td></tr><tr><td>11</td><td>`I_SENSE-`</td><td>Current sense -</td></tr><tr><td>12</td><td>`GND`</td><td>Ground reference</td></tr></table> |

**ESP32-C6-LCD** based — Features: NVS calibration, watchdog, I2C timeout, INA219 current sensing

---

## 🔧 Installation

### Prerequisites

* ✅ 2016 Ford Mustang EcoBoost with boost reference port
* ✅ 12V ACC-switched power source
* ✅ Air compressor system (90-120 PSI)
* ✅ Exhaust bypass valve with pneumatic actuator

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
| --- | --- | --- |
| **P\_ON** | 2.9-3.6 PSI | Valve opens |
| **P\_OFF** | 2.0-2.9 PSI | Valve closes |
| **Hysteresis** | ≥0.7 PSI | Prevents chatter |

---

## 📜 Version History

| Version | Date | Changes |
| --- | --- | --- |
| **v4.8** | Dec 2025 | 🔴 **MCP1702** replaces 78L05 (cold crank OK), **1.5KE18CA** 1500W TVS, PRV optional |
| v4.7 | Dec 2025 | BTS5090 adapter, LED brightness fix, cold crank note |
| v4.6 | Dec 2025 | TVS SM5S22A→P6KE18CA, orientation fix, removed conflicting protections |
| v4.5 | Dec 2025 | Added SM5S22A TVS for ISO 7637-2 Pulse 1 |
| v4.4 | Dec 2025 | Added diagnostic LED + TEST button |
| v4.3 | Dec 2025 | Added Breakout Box connector J-BB |
| v4.2 | Dec 2025 | Removed check valve, added SMC IR1000-01BG |

---

## 📁 Documentation

| File | Description |
| --- | --- |
| [`docs/VALVE_v4_8.md`](docs/VALVE_v4_8.md) | 📄 Complete circuit documentation |
| [`docs/BOM_VALVE_v4_8.md`](docs/BOM_VALVE_v4_8.md) | 📋 Bill of Materials |
| [`hardware/BreakoutBox_Circuit_V1_5.md`](hardware/BreakoutBox_Circuit_V1_5.md) | 🔌 Diagnostic tool schematic |
| [`firmware/BreakoutBox_V1_5.ino`](firmware/BreakoutBox_V1_5.ino) | 💾 ESP32-C6 firmware |
| [`docs/PROTOCOLE_PREMORTEM_V4_2_2.txt`](docs/PROTOCOLE_PREMORTEM_V4_2_2.txt) | ✅ Design validation protocol |

---

## ⚡ Quick Start

```bash
# 1. Order components from BOM_VALVE_v4_8.md
# 2. Solder BTS5090 onto TSSOP-8→DIP-8 adapter FIRST
# 3. Verify 1.5KE18CA orientation with multimeter (mode diode)
# 4. Check MCP1702 pinout (≠ 78L05!)
# 5. Build on stripboard following VALVE_v4_8.md
# 6. Test: Power on → Green LED → Press TEST → Toyota button ON
```

---

## ⚠️ Disclaimer

**This system controls exhaust flow on a high-power turbocharged vehicle.**

Improper installation or failure can cause engine damage or safety hazards.

*Professional installation recommended.*

&nbsp;

---

&nbsp;

**Made with 🔥 for the Mustang community**

*Mehdi — December 2025*

&nbsp;

[![GitHub](https://img.shields.io/badge/Personal_Project-Not_for_Commercial_Use-lightgrey?style=flat-square)](https://github.com)
