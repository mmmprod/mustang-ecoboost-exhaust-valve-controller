# 🏎️ Mustang EcoBoost Exhaust Valve Controller

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Version](https://img.shields.io/badge/version-4.4-blue.svg)](https://github.com/mmmprod/mustang-ecoboost-exhaust-valve-controller)
[![Platform](https://img.shields.io/badge/platform-Ford%20Mustang%20EcoBoost-red.svg)](#)
[![Power](https://img.shields.io/badge/power-600%2B%20HP-orange.svg)](#)
[![Made with](https://img.shields.io/badge/made%20with-love%20%26%20boost-ff69b4.svg)](#)

> **Automatic exhaust valve control based on boost pressure detection.**  
> *Because your neighbors don't need to know you're running 600+ HP... until you want them to.*

---

## 🎯 What Is This?

A fully analog, automotive-grade control system that **automatically opens your exhaust valve when boost kicks in**. No ECU tuning, no CAN bus hacking, no check engine lights. Just pure pneumatic simplicity.

```
📍 Boost detected (2.9 psi) → 💨 Valve opens → 🔊 BRAAAP
📍 Boost drops (2.0 psi)   → 🔇 Valve closes → 🤫 Quiet mode
```

**Manual override?** One-touch toggle button with LED feedback. Press once = valve open. Press again = back to auto.

---

## 🚀 Features

| Feature | Description |
|---------|-------------|
| 🎯 **Boost-Activated** | SMC ISE30A digital pressure switch with configurable thresholds |
| 🔄 **Fail-Safe Design** | Valve closes on power loss (spring-return actuator) |
| ⚡ **Automotive Grade** | ISO 7637-2 transient protection, -40°C to +85°C operation |
| 🛡️ **Triple Surge Protection** | TVS + MOV + Ferrite filtering |
| 🔘 **One-Touch Override** | Toyota-style 22mm LED button for manual control |
| 🔧 **5-Second Diagnostics** | LED + TEST button for instant system check |
| 📊 **Test Interface** | 12-pin connector for automated validation |

---

## 📦 What's Included

### Hardware Design

| File | Description |
|------|-------------|
| [`VALVE_v4_4.docx`](./VALVE_v4_4.docx) | Complete circuit documentation with BOM |

### Test Equipment (Optional)

| File | Description |
|------|-------------|
| [`BreakoutBox_V1.2.ino`](./BreakoutBox_V1.2.ino) | ESP32-C6 firmware for automated testing |
| [`BreakoutBox_Circuit_V1.2.md`](./BreakoutBox_Circuit_V1.2.md) | Breakout box schematic |
| [`User_Setup_C6_LCD.h`](./User_Setup_C6_LCD.h) | TFT_eSPI configuration for Waveshare LCD |

---

## 🔧 System Overview

```
                                    ┌─────────────────┐
                                    │   EXHAUST VALVE │
                                    │   (Pneumatic)   │
                                    └────────▲────────┘
                                             │
┌──────────────┐    ┌─────────────┐    ┌─────┴─────┐
│  INTAKE      │    │  CONTROL    │    │  MAC 35A  │
│  MANIFOLD    │───▶│  UNIT       │───▶│  SOLENOID │
│  (Boost)     │    │  (Trunk)    │    │           │
└──────────────┘    └─────────────┘    └───────────┘
       │                  │
       │            ┌─────┴─────┐
       │            │  BUTTON   │
       └───────────▶│  (Cabin)  │
     Pressure       └───────────┘
     Signal         Manual Override
```

---

## ⚡ Quick Specs

| Parameter | Value |
|-----------|-------|
| Operating Voltage | 9-16V DC (automotive 12V) |
| Quiescent Current | < 5mA |
| Boost Threshold ON | 2.9 - 3.6 psi (configurable) |
| Boost Threshold OFF | 2.0 - 2.9 psi (configurable) |
| Hysteresis | ≥ 0.7 psi |
| Response Time | < 50ms |
| Operating Temperature | -40°C to +85°C |
| Solenoid | MAC 35A 3/2 NC 12V |
| Pressure Switch | SMC ISE30A-01-N |

---

## 🛠️ Installation

### 1. Pressure Detection Line
```
Intake Manifold → 6mm tubing → Trunk → SMC ISE30A
```
⚠️ **NO CHECK VALVE** on this line (needs to see both pressure and vacuum)

### 2. Pneumatic Command Line
```
Air Compressor (90-120 PSI) → Regulator (15 PSI) → MAC Solenoid → Actuator
```
⚠️ **Regulator MANDATORY** - Actuator rated for 7 PSI max!

### 3. Electrical
```
+12V ACC → Relay → Control Unit → MAC Solenoid
                 → Button (cabin)
```

---

## 🔍 5-Second Diagnostics

Open your trunk. Look at the control box:

| Step | Action | Expected | If FAIL |
|------|--------|----------|---------|
| 1 | Check green LED | ON | No power: fuse, relay, wiring |
| 2 | Press TEST button | Button LED ON | Signal chain broken |
| 3 | Release TEST | Button LED OFF | System OK ✅ |

---

## 🧪 Breakout Box (Optional)

For builders who want automated testing:

![ESP32-C6-LCD](https://www.waveshare.com/w/upload/thumb/6/6e/ESP32-C6-LCD-1.47-1.jpg/300px-ESP32-C6-LCD-1.47-1.jpg)

**Hardware:** Waveshare ESP32-C6-LCD-1.47 (~$20)

**Features:**
- Real-time voltage monitoring (+12V, +5V, signals)
- Current measurement (INA219)
- Automated stimulation tests
- GO/NO-GO verdict in 10 seconds

---

## 📋 Bill of Materials (Highlights)

| Component | Reference | ~Price |
|-----------|-----------|--------|
| Pressure Switch | SMC ISE30A-01-N | $80 |
| Solenoid Valve | MAC 35A-AAA-DDBA-1BA | $45 |
| Pressure Regulator | SMC IR1000-01BG | $35 |
| High-Side Driver | Infineon BTS5090-1EJA | $3 |
| Toyota Button 22mm | LED illuminated | $8 |
| Relay | Omron G5Q-1-HA-DC12 | $4 |

**Full BOM in [`VALVE_v4_4.docx`](./VALVE_v4_4.docx)**

---

## ⚠️ Safety Notice

This system is designed **fail-safe**:

> **Power loss = Valve CLOSES = Increased backpressure**

If you experience sudden power loss while on boost:
1. **Lift off throttle immediately**
2. Check fuse and wiring
3. System will not damage engine, but performance will be reduced

---

## 🏁 Tested On

- **2016 Ford Mustang EcoBoost**
- **Stage 4+ E85 tune**
- **600+ HP / 550+ TQ**
- **Daily driven + track days**

---

## 🤝 Contributing

Found a bug? Have an improvement? PRs welcome!

1. Fork the repo
2. Create your feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

---

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

---

## 🙏 Acknowledgments

- SMC Corporation for quality pneumatic components
- Infineon for bulletproof automotive drivers
- The EcoBoost community for pushing these turbos to the limit

---

<p align="center">
  <b>Built with boost. Tested with passion.</b><br>
  <i>Because 600 HP should sound like 600 HP... when you want it to.</i>
</p>

<p align="center">
  <img src="https://img.shields.io/badge/BOOST-ON-brightgreen?style=for-the-badge" />
  <img src="https://img.shields.io/badge/VALVE-OPEN-blue?style=for-the-badge" />
  <img src="https://img.shields.io/badge/NEIGHBORS-SORRY-red?style=for-the-badge" />
</p>
