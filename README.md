# Mustang EcoBoost Exhaust Valve Controller

**Automatic exhaust valve control system for 2016 Ford Mustang EcoBoost Stage 4+ E85 (600+ HP)**

![License](https://img.shields.io/badge/license-MIT-blue.svg)
![Version](https://img.shields.io/badge/version-4.5-green.svg)
![Status](https://img.shields.io/badge/status-active-brightgreen.svg)

---

## Overview

This project implements an automatic exhaust valve controller that opens the valve under boost conditions to eliminate backpressure on the turbine. Features include:

- **Automatic mode**: Valve opens when boost pressure exceeds 2.9-3.6 PSI (0.20-0.25 bar)
- **Manual override**: Toggle button for permanent open state
- **Fail-safe design**: Fail-closed actuator ensures valve closes on power loss
- **Full automotive protection**: ISO 7637-2 / ISO 16750 compliant surge protection

## System Architecture

```
┌─────────────────────────────────────────────────────────────────────────┐
│                         VALVE CONTROLLER v4.5                           │
├─────────────────────────────────────────────────────────────────────────┤
│                                                                         │
│  ┌─────────────┐    ┌─────────────┐    ┌─────────────┐    ┌──────────┐ │
│  │ SMC ISE30A  │───►│   74HC14    │───►│    OR       │───►│ BTS5090  │ │
│  │ Pressostat  │    │   Buffer    │    │ (Diodes)    │    │  Driver  │ │
│  └─────────────┘    └─────────────┘    └─────────────┘    └────┬─────┘ │
│                                              ▲                  │       │
│  ┌─────────────┐    ┌─────────────┐          │                  ▼       │
│  │   Toyota    │───►│   CD4013    │──────────┘           ┌──────────┐  │
│  │   Button    │    │  Flip-Flop  │                      │ MAC 35A  │  │
│  └─────────────┘    └─────────────┘                      │ Solenoid │  │
│                                                          └──────────┘  │
│  ┌─────────────────────────────────────────────────────────────────┐   │
│  │ PROTECTION: TVS SM5S22A + MOV S20K20 + 1N5822 + Fuse 1A        │   │
│  └─────────────────────────────────────────────────────────────────┘   │
└─────────────────────────────────────────────────────────────────────────┘
```

## Hardware Components

### Main Circuit (VALVE v4.5)

| Component | Reference | Function |
|-----------|-----------|----------|
| Pressure Switch | SMC ISE30A-01-N | Boost detection (NPN output) |
| Solenoid Valve | MAC 35A 12V 3/2 NC | Pneumatic control |
| High-Side Driver | BTS5090-1EJA | Solenoid driver with protection |
| Pressure Regulator | SMC IR1000-01BG | Reduces compressor 90 PSI → 15 PSI |
| Schmitt Buffer | 74HC14 | Signal conditioning |
| D Flip-Flop | CD4013 | Manual toggle latch |
| TVS Diode | SM5S22A | 22V 3kW automotive surge protection |
| MOV | S20K20 | Additional transient suppression |
| Schottky Diode | 1N5822 | Reverse polarity protection |

### Breakout Box (v1.4)

Diagnostic tool for testing and validating the VALVE circuit.

| Component | Reference | Function |
|-----------|-----------|----------|
| MCU | ESP32-C6-LCD-1.47 | Waveshare module with integrated 172×320 color LCD |
| Current Sensor | INA219 | I²C current/voltage monitor |
| Protection | BAT54S + 1k series | GPIO clamp for load dump protection |

## Project Structure

```
├── hardware/
│   ├── VALVE_v4_5.docx          # Main circuit schematic & BOM
│   ├── VALVE_v4_3.docx          # Previous version with J-BB connector
│   └── BreakoutBox_Circuit_V1_4.md  # Diagnostic tool circuit
│
├── firmware/
│   ├── BreakoutBox_V1_4/
│   │   ├── BreakoutBox_V1_4.ino # Main firmware
│   │   └── User_Setup_C6_LCD.h  # TFT_eSPI configuration
│   └── README.md                # Firmware installation guide
│
├── docs/
│   ├── Valve_Description.txt    # Project requirements & specs
│   └── datasheets/              # Component datasheets
│
└── README.md
```

## Breakout Box Features (v1.4)

### Modes

| Mode | Function |
|------|----------|
| **LIVE** | Real-time monitoring of all signals with color-coded status |
| **FULL AUTO** | Automated 4-step validation test (~10 seconds) |
| **STRESS TEST** | 10 Hz stimulation for 2 seconds |
| **CALIBRATION** | ADC calibration with min/max hold |

### Monitored Signals

- `+12V_PROT` - Protected 12V rail (green: 11-14.5V)
- `+5V_LOG` - Logic supply (green: 4.90-5.10V)
- `AUTO_REQ` - Pressure switch output
- `MANUAL_REQ` - Toggle state (CD4013 Q)
- `CMD` - OR gate output
- `OUT_DRIVER` - BTS5090 output state
- `I_SENSE` - Current consumption via INA219

### v1.4 Corrections

| Issue | v1.3 | v1.4 |
|-------|------|------|
| 12V Divider | 10k/3.3k (ratio 4.03) | **15k/3.3k (ratio 5.545)** |
| V_GPIO @ 16V | 3.97V ⚠️ DANGER | 2.88V ✅ OK |
| Load Dump Protection | None | **BAT54S clamp to 3.3V** |
| Series Resistor | None | **1kΩ per channel** |
| HF Filter | None | **100nF per GPIO** |

## Installation

### Prerequisites

- Arduino IDE 2.x
- ESP32 Board Support (Espressif v3.x)
- Libraries: `TFT_eSPI`, `Adafruit INA219`

### Firmware Upload

1. Clone this repository
2. Copy `User_Setup_C6_LCD.h` content to `Arduino/libraries/TFT_eSPI/User_Setup.h`
3. Select board: **ESP32C6 Dev Module**
4. Set **USB CDC On Boot: Enabled**
5. Upload `BreakoutBox_V1_4.ino`

## Pneumatic System

### Detection Line (Boost Measurement)

```
Intake Manifold → 6mm Tubing (NO CHECK VALVE) → Bulkhead → SMC ISE30A
```

⚠️ **CRITICAL**: No check valve on this line. A check valve traps positive pressure and prevents valve closure.

### Command Line (Actuator Control)

```
Compressor (90-120 PSI) → Tee → SMC IR1000-01BG (set 1 bar) → MAC Port P → Actuator
```

⚠️ **CRITICAL**: Pressure regulator is mandatory. Actuator rated for 7 PSI; direct compressor pressure = membrane destruction.

## Safety Considerations

| Risk | Mitigation |
|------|------------|
| Power loss | Fail-closed actuator design |
| Voltage surge (load dump) | SM5S22A + MOV + BAT54S clamp |
| Reverse polarity | 1N5822 series diode |
| Overcurrent | 1A fuse on solenoid |
| Overpressure | SMC IR1000-01BG regulator |

### Fail-Safe Behavior

**If any failure occurs**: Actuator returns to closed position → Valve closed → Normal exhaust backpressure → **Lift off throttle immediately** to protect turbine.

## Connector Pinout (J-BB)

### Header A (Measurements)

| Pin | Signal | Divider |
|-----|--------|---------|
| 1 | +12V_PROT | 15k/3.3k + BAT54S |
| 2 | +5V_LOG | 10k/10k |
| 3 | AUTO_REQ | 10k/10k |
| 4 | OUT_DRIVER | 15k/3.3k + BAT54S |
| 5 | CMD | 10k/10k |
| 6 | MANUAL_REQ | 10k/10k |

### Header B (Control + Current)

| Pin | Signal | Connection |
|-----|--------|------------|
| 7 | GND | Common ground |
| 8 | STIM_1 | NPN collector (pressostat sim) |
| 9 | STIM_2 | Button signal injection |
| 10 | I_SENSE+ | After fuse (INA219 VIN+) |
| 11 | I_SENSE- | Before ferrite (INA219 VIN-) |
| 12 | GND | Common ground |

## Version History

| Version | Date | Changes |
|---------|------|---------|
| **v4.5** | Dec 2025 | SM5S22A TVS, updated BOM |
| v4.3 | Dec 2025 | Added J-BB connector for Breakout Box |
| v4.2 | Dec 2025 | Removed check valve, added pressure regulator |
| v3.7 | Nov 2025 | Zener gate clamp correction |

### Breakout Box

| Version | Date | Changes |
|---------|------|---------|
| **v1.4** | Dec 2025 | Fixed 12V divider, added BAT54S clamp, 1k series R |
| v1.3 | Dec 2025 | Added NPN stimulation, INA219 protection |
| v1.2 | Dec 2025 | Added STRESS TEST mode |
| v1.1 | Dec 2025 | Migrated to ESP32-C6-LCD integrated module |

## Contributing

Issues and pull requests are welcome. Please ensure any hardware modifications are thoroughly tested before submission.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Disclaimer

This project involves modifications to a high-performance vehicle's exhaust system. Incorrect installation may result in:
- Engine damage
- Fire hazard
- Legal issues (emissions compliance)

**Use at your own risk.** The author assumes no liability for any damages resulting from the use of this project.

---

**Author**: Mehdi  
**Vehicle**: 2016 Ford Mustang EcoBoost Stage 4+ E85  
**Power**: 600+ HP
