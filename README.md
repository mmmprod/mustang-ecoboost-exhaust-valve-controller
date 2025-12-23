# 🐎 Mustang EcoBoost Exhaust Valve Controller

### *When 600+ HP needs to breathe*

&nbsp;

[![Version](https://img.shields.io/badge/VALVE-v4.9-0366d6?style=for-the-badge&logo=v&logoColor=white)](https://github.com)
[![Breakout](https://img.shields.io/badge/BREAKOUT-v1.10-9c27b0?style=for-the-badge&logo=espressif&logoColor=white)](https://github.com)
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

*Fail-Safe • Automotive-Grade • Touch Interface*

&nbsp;

[📖 Documentation](#-documentation) •
[🔧 Installation](#-installation) •
[📟 Breakout Box](#-breakout-box-v110) •
[🛡️ Safety](#%EF%B8%8F-safety)

&nbsp;

---

## 📊 Quick Stats

|  |  |  |  |
| --- | --- | --- | --- |
| ![Boost](https://img.shields.io/badge/2.9--3.6-PSI-dc3545?style=for-the-badge) | ![Load Dump](https://img.shields.io/badge/100V-400ms-ffc107?style=for-the-badge) | ![Clamp](https://img.shields.io/badge/24.4V-Clamp-28a745?style=for-the-badge) | ![Temp](https://img.shields.io/badge/--40°C_to_+85°C-Operating-17a2b8?style=for-the-badge) |
| **Boost Threshold** | **Load Dump Protection** | **TVS Clamping** | **Temperature Range** |

---

## 🆕 What's New

### VALVE v4.9 — *Load Dump Survivor*

| Change | Before | After | Impact |
|--------|--------|-------|--------|
| 🔴 **5V Regulator** | 78L05 (30V max) | **NCV2931AZ** (60V trans) | Survives TVS clamp |
| ⚡ **Cold Crank** | 2V dropout | **330mV dropout** | Works at 5.3V input |
| 🛡️ **Transient** | Destruction at 25V | **60V certified** | ISO 7637-2 compliant |

### BreakoutBox v1.10 — *Touch & Go*

| Feature | Description |
|---------|-------------|
| 👆 **Capacitive Touch** | ESP32-C6 + 1.9" IPS touchscreen |
| 🔒 **Race Condition Fix** | Atomic interrupt handling |
| 🩺 **Sensor Diagnosis** | Detects "sensor fault" vs "valve stuck" |
| ⏱️ **1s Debounce** | No more accidental double-tap |

---

## 🎯 Features

|  |  |
| --- | --- |
| 🚗 **Automatic Mode**<br>Opens valve when SMC ISE30A detects boost above threshold | ⚡ **Automotive-Grade**<br>Full ISO 7637-2 protection: load dump, cold crank, EMI |
| 🕹️ **Manual Override**<br>Toggle valve anytime with illuminated Toyota button | 📱 **Touch Diagnostics**<br>One-tap test validates entire signal chain |
| 🛡️ **Fail-Safe Design**<br>Pneumatic actuator closes on power loss | 🔧 **DIY Friendly**<br>100% through-hole on standard stripboard |

---

## 🏗️ System Architecture

```
┌─────────────────────────────────────────────────────────────────────────────┐
│                          VALVE CONTROLLER v4.9                              │
├─────────────────────────────────────────────────────────────────────────────┤
│                                                                             │
│   ┌─────────┐    ┌─────────┐    ┌─────────┐    ┌─────────┐    ┌─────────┐  │
│   │ +12V    │───▶│ P6KE18CA│───▶│ FERRITE │───▶│ FUSE 1A │───▶│ 1N5822  │  │
│   │ BATTERY │    │ TVS 18V │    │ 90Ω EMI │    │ PROTECT │    │ REVERSE │  │
│   └─────────┘    └─────────┘    └─────────┘    └─────────┘    └────┬────┘  │
│                                                                     │       │
│        ┌────────────────────────────────────────────────────────────┘       │
│        ▼                                                                    │
│   ┌──────────┐        ┌─────────────────────────────────────────────────┐  │
│   │ NCV2931  │───────▶│                  +5V LOGIC                      │  │
│   │ 60V SAFE │        └─────────────────────────────────────────────────┘  │
│   └──────────┘                    │                    │                    │
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
│   │ 🟢 LED        🔘 TEST        📟 BREAKOUT BOX        👆 TOUCH │ │       │
│   └─────────────────────────────────────────────────────────────┘ ▼       │
│                                                              ACTUATOR      │
└─────────────────────────────────────────────────────────────────────────────┘
```

---

## 📟 Breakout Box V1.10

*Touch-enabled diagnostic interface — tap to test*

<table>
<tr>
<td width="50%">

### 🖥️ Hardware

| Spec | Value |
|------|-------|
| **MCU** | ESP32-C6FH8 |
| **Clock** | 160 MHz |
| **Display** | 1.9" IPS 170×320 |
| **Touch** | CST816 Capacitive |
| **Current** | INA219 ±3.2A |
| **Interface** | WiFi 6 / BLE 5 |

</td>
<td width="50%">

### ⚡ Features

| Feature | Status |
|---------|--------|
| 👆 Touch to Test | ✅ |
| 📊 Live Readings | ✅ |
| 🔴🟢 GO/NO-GO | ✅ |
| 🩺 Fault Diagnosis | ✅ |
| 📋 One-liner Report | ✅ |
| 🔒 Race Condition Fix | ✅ |

</td>
</tr>
</table>

### GPIO Map (Waveshare ESP32-C6 Touch LCD 1.9")

```
LCD RESERVED:     GPIO 4, 5, 6, 7, 14, 15  ← DO NOT USE
Touch I2C:        GPIO 8 (SCL), 18 (SDA)   ← Shared with INA219
Touch Control:    GPIO 22 (INT)
ADC Available:    GPIO 0, 1, 2, 3
Digital Free:     GPIO 19, 20, 21, 23      ← Used for BreakoutBox I/O
```

> ⚠️ **SD Card slot disabled** — GPIO 19,20 reused for stimulation outputs

---

## 🔌 Hardware

### Protection Chain (BLOC A)

| Stage | Component | Function | Rating |
|:-----:|-----------|----------|--------|
| **1** | P6KE18CA | TVS Clamping | 24.4V @ 10A |
| **2** | Würth 742792093 | EMI Filter | 90Ω @ 100MHz |
| **3** | Fuse 1A T | Overcurrent | Fast-blow |
| **4** | 1N5822 | Reverse Polarity | 40V 3A |

### Regulation (BLOC B) — v4.9

| | Old (v4.6) | New (v4.9) |
|---|------------|------------|
| **Part** | 78L05 | **NCV2931AZ-5.0G** |
| **Vin Max** | 30V | **60V transient** |
| **Dropout** | 2V | **330mV** |
| **Cold Crank** | ❌ Fails at 6V | ✅ Works at 5.3V |

> 🔴 **Why the change?** TVS clamps at 25.2V. 78L05 dies at 30V. Math doesn't work.

### Logic & Pneumatic

| Block | Component | Function |
|-------|-----------|----------|
| **C** | SMC ISE30A-01-N | Boost sensing (NPN) |
| **C** | 74HC14 | Schmitt buffer |
| **D** | Toyota 22×22mm | Manual button |
| **E** | CD4013 | Toggle flip-flop |
| **G** | BTS5090-1EJA | Smart high-side driver |
| **P** | MAC 35A 12V | Solenoid valve |
| **P** | SMC IR1000-01BG | Pressure regulator |

---

## 🛡️ Safety

### Transient Protection Matrix

| Event | Input | Protection | Output | |
|-------|-------|------------|--------|---|
| Normal | 12-14.8V | — | 12-14.8V | ✅ |
| **Jump Start** | 24V | Fuse | Protected | ⚡ |
| **Load Dump** | 100V/400ms | TVS | 24.4V | 🛡️ |
| **Cold Crank** | 6V | NCV2931 | 5V stable | ✅ |

### Margin Analysis (v4.9)

```
NCV2931       ████████████████████████████░░  60V   → 25.2V  (58% margin) ✅
BTS5090       ████████████████████████░░░░░░  42V   → 25.2V  (40% margin) ✅
1N5822        █████████████████████████░░░░░  40V   → 25.2V  (37% margin) ✅
SMC ISE30A    ███████████████████████████░░░  28V   → 25.2V  (10% margin) ⚠️
```

### Fail-Safe Behavior

```
┌─────────────┐     ┌─────────────┐     ┌─────────────┐
│ POWER LOSS  │ ──▶ │ SOLENOID    │ ──▶ │ VALVE       │
│             │     │ DE-ENERGIZES│     │ CLOSES      │
└─────────────┘     └─────────────┘     └─────────────┘
                         ⚠️ LIFT THROTTLE IMMEDIATELY
```

---

## 📁 Documentation

| File | Description |
|------|-------------|
| [`docs/VALVE_v4_9.md`](docs/) | Complete circuit documentation |
| [`docs/BOM_VALVE_v4_9.md`](docs/) | Bill of Materials |
| [`hardware/BreakoutBox_Circuit_V1_7.md`](hardware/) | Diagnostic schematic |
| [`firmware/BreakoutBox_V1_10.ino`](firmware/) | Touch interface firmware |
| [`firmware/User_Setup_C6_Touch_LCD.h`](firmware/) | TFT_eSPI configuration |

---

## 🔧 Installation

### Prerequisites

```
✅ 2016 Ford Mustang EcoBoost with boost reference port
✅ 12V ACC-switched power source  
✅ Air compressor system (90-120 PSI)
✅ Exhaust bypass valve with pneumatic actuator
```

### Wiring

```
+12V_BATT ──── Battery via 5A fuse (≤15cm from terminal)
+12V_ACC  ──── Ignition-switched source
GND_STAR  ──── Single point chassis ground
BOOST     ──── 6mm tubing from intake manifold
PNEUMATIC ──── Compressor → Regulator → MAC 35A → Actuator
```

### Pressure Settings

| Parameter | Value |
|-----------|-------|
| **P_ON** | 2.9–3.6 PSI |
| **P_OFF** | 2.0–2.9 PSI |
| **Hysteresis** | ≥0.7 PSI |

---

## 📜 Version History

<details>
<summary><b>VALVE Versions</b></summary>

| Version | Date | Changes |
|---------|------|---------|
| **v4.9** | Dec 2025 | 🔴 78L05→NCV2931 (60V transient safe) |
| v4.8 | Dec 2025 | MCP1702 attempt (failed: 14.5V max) |
| v4.6 | Dec 2025 | TVS fix: SM5S22A→P6KE18CA |
| v4.5 | Dec 2025 | Added TVS for ISO 7637-2 |
| v4.4 | Dec 2025 | Diagnostic LED + TEST button |

</details>

<details>
<summary><b>BreakoutBox Versions</b></summary>

| Version | Date | Changes |
|---------|------|---------|
| **V1.10** | Dec 2025 | Race condition fix, sensor diagnosis, 1s debounce |
| V1.9 | Dec 2025 | ESP32-C6 Touch LCD 1.9" support |
| V1.7 | Dec 2025 | INA219 NaN protection, ADC validation |

</details>

---

## ⚠️ Disclaimer

**This system controls exhaust flow on a high-power turbocharged vehicle.**

Improper installation or failure can cause engine damage or safety hazards.  
*Professional installation recommended.*

&nbsp;

---

&nbsp;

<div align="center">

**Made with 🔥 for the Mustang community**

*Mehdi — December 2025*

&nbsp;

[![License](https://img.shields.io/badge/License-MIT-blue?style=flat-square)](LICENSE)
[![Personal](https://img.shields.io/badge/Personal_Project-Not_Commercial-lightgrey?style=flat-square)](https://github.com)

</div>
