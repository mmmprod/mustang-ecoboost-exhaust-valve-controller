# 🐎 Mustang EcoBoost Exhaust Valve Controller

### *When 600+ HP needs to breathe*

&nbsp;

[![Version](https://img.shields.io/badge/version-4.9-0366d6?style=for-the-badge&logo=v&logoColor=white)](https://github.com)
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
     EXHAUST VALVE CONTROLLER v4.9   |___/
```

&nbsp;

**Automatic boost-activated exhaust bypass system**

*Fail-Safe • Automotive-Grade • Plug & Play*

&nbsp;

[📖 Documentation](#-documentation) •
[🔧 Installation](#-installation) •
[⚡ Quick Start](#-quick-start) •
[🛡️ Safety](#%EF%B8%8F-safety) •
[📟 Breakout Box](#-breakout-box-diagnostic-tool)

&nbsp;

---

## 📊 Quick Stats

|  |  |  |  |
| --- | --- | --- | --- |
| ![Boost](https://img.shields.io/badge/2.9--3.6-PSI-dc3545?style=for-the-badge) <br/> **Boost Threshold** | ![Load Dump](https://img.shields.io/badge/100V-400ms-ffc107?style=for-the-badge) <br/> **Load Dump Protection** | ![Clamp](https://img.shields.io/badge/25.2V-Clamp-28a745?style=for-the-badge) <br/> **TVS Clamping** | ![Temp](https://img.shields.io/badge/--40°C_to_+85°C-Operating-17a2b8?style=for-the-badge) <br/> **Temperature Range** |

---

## 🎯 Features

|  |  |
| --- | --- |
| 🚗 **Automatic Mode** <br/> Opens valve when SMC ISE30A pressure switch detects boost above configurable threshold <br/><br/> 🕹️ **Manual Override** <br/> Toggle valve state anytime with OEM-style illuminated Toyota push button <br/><br/> 🛡️ **Fail-Safe Design** <br/> Pneumatic actuator is fail-closed — valve closes on power loss, protecting your turbo | ⚡ **Automotive-Grade** <br/> Full ISO 7637-2 transient protection including load dump (+100V) and cold crank (6V) <br/><br/> 🔍 **Quick Diagnostic** <br/> 5-second visual check: LED + TEST button validates entire signal chain <br/><br/> 🔧 **DIY Friendly** <br/> 100% through-hole components on standard stripboard — no SMD soldering required |

---

## 🏗️ System Architecture

```
┌─────────────────────────────────────────────────────────────────────────────┐
│                          VALVE CONTROLLER v4.9                              │
├─────────────────────────────────────────────────────────────────────────────┤
│                                                                             │
│   ┌─────────┐    ┌─────────┐    ┌─────────┐    ┌─────────┐    ┌─────────┐  │
│   │ +12V    │───▶│1.5KE18CA│───▶│ FERRITE │───▶│ FUSE 1A │───▶│ 1N5822  │  │
│   │ BATTERY │    │ TVS 18V │    │ 90Ω EMI │    │ PROTECT │    │ REVERSE │  │
│   └─────────┘    └─────────┘    └─────────┘    └─────────┘    └────┬────┘  │
│                                                                     │       │
│        ┌────────────────────────────────────────────────────────────┘       │
│        ▼                                                                    │
│   ┌─────────┐         ┌─────────────────────────────────────────────────┐  │
│   │NCV2931  │────────▶│                  +5V LOGIC                      │  │
│   │ 5V REG  │         └─────────────────────────────────────────────────┘  │
│   └─────────┘                     │                    │                    │
│   (60V transient)                 ▼                    ▼                    │
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

### Protection Chain (BLOC A) — *v4.9 Certified*

| Stage | Component | Function | Specs | Package |
| --- | --- | --- | --- | --- |
| **1** | `1.5KE18CA` | TVS Clamping | V_C = 25.2V @ 60A, 1500W | DO-201AE |
| **2** | `Würth 742792093` | EMI Filter | 90Ω @ 100MHz | Axial |
| **3** | `Fuse 1A T` | Overcurrent | Fast-blow | 5×20mm |
| **4** | `1N5822` | Reverse Polarity | 40V 3A Schottky | DO-201AD |
| **5** | `NCV2931AZ-5.0G` | 5V LDO | 60V transient | TO-92 |

> ⚠️ **v4.9 CRITICAL**: NCV2931 replaces 78L05 — supports 60V transients vs TVS clamp 25.2V

### Logic & Control

| Block | Component | Package | Function |
| --- | --- | --- | --- |
| **B** | NCV2931AZ-5.0G | TO-92 | 5V Regulation (60V transient) |
| **C** | SMC ISE30A-01-N | — | Boost Pressure Sensing |
| **C** | 74HC14 | DIP-14 | Schmitt Buffer |
| **D** | Toyota 22×22mm | — | Illuminated Push Button |
| **E** | CD4013 | DIP-14 | Toggle Flip-Flop |
| **G** | BTS5090-1EJA + Adapter | TSSOP-8→DIP | High-Side Smart Driver |

### Pneumatic System

| Component | Reference | Function |
| --- | --- | --- |
| Pressure Switch | SMC ISE30A-01-N | Boost detection (NPN, 12-24V) |
| Pressure Regulator | SMC IR1000-01BG | Compressor → 1 bar output |
| Solenoid Valve | MAC 35A 12V | Pneumatic control (5.4W) |
| Actuator | Fail-closed | Exhaust valve actuation |

> ⚠️ **NO CHECK VALVE** on detection line — Direct vacuum path required

---

## 📟 Breakout Box Diagnostic Tool

### *Touch Screen Interface for Technicians*

&nbsp;

[![BB Version](https://img.shields.io/badge/Firmware-V1.9-00d4aa?style=for-the-badge&logo=arduino&logoColor=white)](https://github.com)
[![BB Circuit](https://img.shields.io/badge/Circuit-V1.7-0099ff?style=for-the-badge&logo=altiumdesigner&logoColor=white)](https://github.com)
[![Touch](https://img.shields.io/badge/Interface-TOUCH_SCREEN-ff6b9d?style=for-the-badge&logo=smartphone&logoColor=white)](https://github.com)

&nbsp;

```
┌────────────────────────────────────────┐
│      BREAKOUT BOX V1.9 TACTILE         │
├────────────────────────────────────────┤
│                                        │
│   ┌──────────────────────────────┐    │
│   │                              │    │
│   │    ╔════════════════════╗    │    │
│   │    ║   TOUCH TO TEST    ║    │    │
│   │    ╚════════════════════╝    │    │
│   │                              │    │
│   │      ESP32-C6 Touch LCD     │    │
│   │         170 x 320           │    │
│   └──────────────────────────────┘    │
│                                        │
│   [ INA219 ]  Current: 0-3.2A         │
│   [ CST816 ]  Touch Controller        │
│                                        │
│   USB-C ●────────────● J-BB 12-pin    │
│                                        │
└────────────────────────────────────────┘
```

### Features

| Feature | Description |
| --- | --- |
| 👆 **Touch Interface** | No buttons — tap screen to test |
| 🟢🔴 **Full-Screen GO/NOGO** | Giant OK/FAIL visible from 3 meters |
| 📊 **One-Line AI Report** | `COPIER: Valve OK \| 12V=12.3V, 5V=5.0V, OUT=12.1V, I=350mA` |
| ⚡ **Auto Test Sequence** | Power → Stimulation → Measurement → Verdict |
| 🛡️ **All V1.7 Protections** | isfinite(), timeout 60s, watchdog 5s |

### Hardware

| Component | Reference | Notes |
| --- | --- | --- |
| MCU + Display | ESP32-C6 Touch LCD 1.9" (Waveshare) | 170×320, CST816 touch |
| Current Sensor | INA219 HW-831B | 0-26V, 0-3.2A, I2C 0x40 |
| Connector | 2×6 pin header | Interface J-BB on VALVE |

### GPIO Mapping V1.9

```
ESP32-C6 Touch LCD 1.9" — Pinout BreakoutBox V1.7

RESERVED (LCD/Touch):         AVAILABLE (BreakoutBox):
├─ GPIO4  LCD_DIN             ├─ GPIO0  → DIV_12V (ADC)
├─ GPIO5  LCD_CLK             ├─ GPIO1  → DIV_5V (ADC)
├─ GPIO6  LCD_DC              ├─ GPIO2  → DIV_OUT (ADC)
├─ GPIO7  LCD_CS              ├─ GPIO3  → AUTO_REQ (IN)
├─ GPIO8  I2C SCL ←───────────┼─ GPIO8  → INA219 SCL (shared)
├─ GPIO14 LCD_RST             ├─ GPIO18 → INA219 SDA (shared)
├─ GPIO15 LCD_BL              ├─ GPIO19 → CMD (IN)
├─ GPIO18 I2C SDA ←───────────┼─ GPIO20 → STIM1 (OUT)
├─ GPIO22 Touch INT           ├─ GPIO21 → STIM2 (OUT)
                              └─ GPIO23 → MANUAL_REQ (IN)

I2C Bus (GPIO8/18):
  ├─ 0x15 CST816 Touch Controller
  └─ 0x40 INA219 Current Sensor
```

### J-BB Connector

| Row A — Measurements | Row B — Stimulation |
| --- | --- |
| **Pin 1** `+12V_PROT` Protected 12V rail | **Pin 7** `GND` Ground reference |
| **Pin 2** `+5V_LOG` Logic supply | **Pin 8** `STIM_1` After 1k resistor |
| **Pin 3** `AUTO_REQ` Pressure switch out | **Pin 9** `STIM_2` Before 10k resistor |
| **Pin 4** `OUT_DRIVER` BTS5090 output | **Pin 10** `I_SENSE+` Current sense + |
| **Pin 5** `CMD` OR gate output | **Pin 11** `I_SENSE-` Current sense - |
| **Pin 6** `MANUAL_REQ` Button toggle state | **Pin 12** `GND` Ground reference |

---

## 🛡️ Safety

### Transient Protection Matrix — v4.9

| Event | Input | Protection | Output | Status |
| --- | --- | --- | --- | --- |
| Normal Operation | 12-14.8V | Pass-through | 12-14.8V | ✅ |
| Alternator Max | 14.8V | V_WRM margin | 14.8V | ✅ |
| **Jump Start** | **24V** | **TVS clamp** | **25.2V** | 🛡️ |
| **Load Dump** | **100V/400ms** | **TVS clamp** | **25.2V** | 🛡️ |
| Cold Crank | 6V | NCV2931 dropout | 4.8V OK | ✅ |

### Component Margins — v4.9

```
NCV2931 (NEW)  ████████████████████  60V trans  → 25.2V actual  (58% margin) ✅
SMC ISE30A    ████████████░░░░░░░░  28V max    → 25.2V actual  (10% margin)
1N5822        ████████████████████  40V max    → 25.2V actual  (37% margin)
```

> ⚠️ **v4.9 FIX**: 78L05 (14.5V max) → NCV2931 (60V transient) — **Critical for load dump survival**

### ⚠️ Fail-Safe Behavior

```
╔═══════════════════════════════════════════════════════════════╗
║  Power Loss → Solenoid OFF → Valve CLOSES → Turbo Protected  ║
║                                                               ║
║  ⚠️  IMMEDIATELY LIFT THROTTLE IF SYSTEM FAILS               ║
╚═══════════════════════════════════════════════════════════════╝
```

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

### Pressure Settings (SMC ISE30A)

| Parameter | Value | Notes |
| --- | --- | --- |
| **P_ON** | 2.9-3.6 PSI | Valve opens |
| **P_OFF** | 2.0-2.9 PSI | Valve closes |
| **Hysteresis** | ≥0.7 PSI | Prevents chatter |

---

## 📜 Version History

### VALVE Circuit

| Version | Date | Changes |
| --- | --- | --- |
| **v4.9** | Dec 2025 | 🔴 **CRITICAL**: 78L05→NCV2931 (60V transient), load dump compatible |
| v4.8 | Dec 2025 | MCP1702 cold crank fix (reverted — Vin 14.5V < TVS clamp) |
| v4.6 | Dec 2025 | TVS SM5S22A→P6KE18CA, orientation fix |
| v4.5 | Dec 2025 | Added SM5S22A TVS for ISO 7637-2 Pulse 1 |
| v4.4 | Dec 2025 | Added diagnostic LED + TEST button |
| v4.3 | Dec 2025 | Added Breakout Box connector J-BB |

### Breakout Box

| Version | Date | Changes |
| --- | --- | --- |
| **V1.9** | Dec 2025 | 🆕 **TOUCH SCREEN** interface, full GPIO remapping |
| **V1.7** | Dec 2025 | 🆕 Circuit update for ESP32-C6 Touch LCD 1.9" |
| V1.6 | Dec 2025 | GPIO protection verification, audit validation |
| V1.5 | Dec 2025 | NVS calibration, watchdog, I2C timeout |
| V1.4 | Dec 2025 | Initial release with INA219 current sensing |

---

## 📁 Documentation

### Main Circuit

| File | Description |
| --- | --- |
| [`VALVE_v4_9.md`](docs/VALVE_v4_9.md) | 📄 Complete circuit documentation |
| [`BOM_VALVE_v4_9.md`](docs/BOM_VALVE_v4_9.md) | 📋 Bill of Materials |
| [`PROTOCOLE_PREMORTEM_V4_2_3.txt`](docs/PROTOCOLE_PREMORTEM_V4_2_3.txt) | ✅ Design validation protocol |

### Breakout Box

| File | Description |
| --- | --- |
| [`BreakoutBox_V1_9.ino`](firmware/BreakoutBox_V1_9.ino) | 💻 Touch interface firmware |
| [`BreakoutBox_Circuit_V1_7.md`](hardware/BreakoutBox_Circuit_V1_7.md) | 🔌 Diagnostic tool schematic |

---

## ⚡ Quick Start — Breakout Box

### 1. Flash Firmware

```bash
# Arduino IDE Settings
Board: ESP32C6 Dev Module
Port: /dev/ttyUSBx (Linux) or COMx (Windows)
Upload Speed: 921600
```

### 2. Connect

```
VALVE J-BB (12-pin) ←──cable──→ BreakoutBox (12-pin)
                                     │
                                  USB-C (power)
```

### 3. Test

```
1. Touch "TEST" on screen
2. Wait 2 seconds
3. Read result:
   🟢 GREEN = VALVE OK
   🔴 RED   = VALVE FAIL + Cause
4. Serial: 'R' for AI-ready report
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
