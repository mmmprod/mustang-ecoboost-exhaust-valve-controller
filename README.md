# Mustang EcoBoost Exhaust Valve Controller

**Automatic exhaust valve control system for 2016 Ford Mustang EcoBoost Stage 4+ E85 (600+ HP)**

ISO 7637-2 Pulse 1 Compliant | Fail-Safe Design | Boost-Activated

---

## Overview

This system automatically opens an exhaust bypass valve when boost pressure exceeds a configurable threshold (2.9-3.6 PSI), while providing manual override capability via an OEM-style Toyota push button.

### Key Features

- **Automatic Mode**: Opens valve when SMC ISE30A detects boost above threshold
- **Manual Override**: Toggle valve state with illuminated push button
- **Fail-Safe**: Pneumatic actuator is fail-closed (valve closes on power loss)
- **Automotive-Grade**: Full ISO 7637-2 transient protection, -40°C to +85°C operation
- **Diagnostic**: 5-second visual check via LED + TEST button

---

## System Architecture

```
┌─────────────────────────────────────────────────────────────────────┐
│                        VALVE CONTROLLER v4.6                        │
├─────────────────────────────────────────────────────────────────────┤
│                                                                     │
│  +12V_BATT ──┬── Relay G5Q ──┬── P6KE18CA ── Ferrite ── Fuse 1A    │
│              │   (ACC ctrl)   │   (TVS 18V)   (EMI)     (protect)   │
│              │               │                                      │
│              │               └── 1N5822 ── +12V_PROT ── 78L05       │
│              │                   (reverse)              (5V reg)    │
│              │                                                      │
│  BOOST ──────┼── SMC ISE30A ── 74HC14 ── OR ──┬── BTS5090 ── MAC   │
│  (intake)    │   (pressure)    (buffer)  gate │   (driver)   35A   │
│              │                               │                      │
│  BUTTON ─────┼── CD4013 ─────────────────────┘                     │
│  (Toyota)    │   (toggle)                                           │
│              │                                                      │
│  DIAGNOSTIC ─┼── LED green (power) + TEST button                   │
│              │                                                      │
│  BREAKOUT ───┴── J-BB 2x6 header (validation interface)            │
│                                                                     │
└─────────────────────────────────────────────────────────────────────┘
```

---

## Hardware Components

### Protection Chain (BLOC A) - v4.6

| Stage | Component | Function | Specs |
|-------|-----------|----------|-------|
| 1 | P6KE18CA | TVS clamping | V_C=24.4V, 600W, DO-15 THT |
| 2 | Wurth 742792093 | EMI filtering | 90Ω @ 100MHz, Axial |
| 3 | Fuse 1A T | Overcurrent | 5x20mm glass, Fast-blow |
| 4 | 1N5822 | Reverse polarity | 40V 3A Schottky, DO-201AD |

**⚠️ CRITICAL v4.6**: TVS orientation - Cathode (band) to +12V_IN, Anode to GND

### Logic & Control

| Block | Component | Package | Function |
|-------|-----------|---------|----------|
| B | 78L05 | TO-92 | 5V regulation |
| C | SMC ISE30A-01-N | - | Boost pressure sensing |
| C | 74HC14 | DIP-14 | Schmitt buffer |
| D | Toyota 22x22mm | - | Illuminated push button |
| E | CD4013 | DIP-14 | Toggle flip-flop |
| G | BTS5090-1EJA | PG-TDSO-8 | High-side driver |

### Pneumatic System

| Component | Reference | Function |
|-----------|-----------|----------|
| Pressure switch | SMC ISE30A-01-N | Boost detection (NPN, 12-24V) |
| Pressure regulator | SMC IR1000-01BG | Compressor → 1 bar output |
| Solenoid valve | MAC 35A 12V | Pneumatic control (5.4W) |
| Actuator | Fail-closed | Exhaust valve actuation |

**⚠️ NO CHECK VALVE on detection line** - Direct vacuum path required

---

## Breakout Box Interface (V1.4)

Diagnostic interface for automated validation testing.

### Pinout

**Row A (Measurements):**
| Pin | Signal | Description |
|-----|--------|-------------|
| 1 | +12V_PROT | Protected 12V rail |
| 2 | +5V_LOG | Logic supply |
| 3 | AUTO_REQ | Pressure switch output |
| 4 | OUT_DRIVER | BTS5090 output |
| 5 | CMD | OR gate output |
| 6 | MANUAL_REQ | Button toggle state |

**Row B (Stimulation):**
| Pin | Signal | Description |
|-----|--------|-------------|
| 7 | GND | Ground reference |
| 8 | STIM_1 | After 1k resistor |
| 9 | STIM_2 | Before 10k resistor |
| 10 | I_SENSE+ | Current sense + |
| 11 | I_SENSE- | Current sense - |
| 12 | GND | Ground reference |

---

## Safety Considerations

### Fail-Safe Design

The pneumatic actuator is **fail-closed** by design:
- Power loss → Solenoid de-energizes → Valve closes
- This creates turbo backpressure
- **IMMEDIATELY lift throttle** if system fails

### Transient Protection (v4.6)

| Event | Voltage | Protection | Result |
|-------|---------|------------|--------|
| Normal | 12-14.8V | Pass-through | Normal operation |
| Alternator max | 14.8V | V_WRM=15.3V margin | TVS inactive |
| Jump start | 24V | Fuse 1A blows | Circuit protected |
| Load dump | 100V/400ms | V_C=24.4V clamp | SMC < 28V max |
| Cold crank | 6V | 78L05 dropout | Degraded mode |

### Component Limits Verification

| Component | Max Rating | Worst Case (v4.6) | Margin |
|-----------|------------|-------------------|--------|
| SMC ISE30A | 28V | 24.4V (clamp) | 12.9% |
| 78L05 | 30V | 24.4V (clamp) | 18.7% |
| 1N5822 | 40V | 24.4V (reverse) | 39% |

---

## Installation

### Prerequisites

- 2016 Ford Mustang EcoBoost with boost reference port
- 12V ACC-switched power source
- Air compressor system (90-120 PSI)
- Exhaust bypass valve with pneumatic actuator

### Wiring

1. **Power**: Connect +12V_BATT to battery via 5A fuse (≤15cm from terminal)
2. **ACC**: Connect +12V_ACC to ignition-switched source
3. **Ground**: Single point ground (GND_STAR) to chassis
4. **Boost**: 6mm tubing from intake manifold to SMC ISE30A
5. **Pneumatic**: Compressor → Regulator (1 bar) → MAC 35A → Actuator

### Pressure Settings

| Parameter | Value | Notes |
|-----------|-------|-------|
| P_ON | 2.9-3.6 PSI | Valve opens |
| P_OFF | 2.0-2.9 PSI | Valve closes |
| Hysteresis | ≥0.7 PSI | Prevent chatter |

---

## Version History

| Version | Date | Changes |
|---------|------|---------|
| v4.6 | Dec 2025 | **CRITICAL FIX**: TVS SM5S22A→P6KE18CA, orientation corrected, removed conflicting 5KP18CA+MOV |
| v4.5 | Dec 2025 | Added SM5S22A TVS for ISO 7637-2 Pulse 1 |
| v4.4 | Dec 2025 | Added diagnostic LED + TEST button |
| v4.3 | Dec 2025 | Added Breakout Box connector J-BB |
| v4.2 | Dec 2025 | Removed check valve, added SMC IR1000-01BG regulator |

---

## Files

| File | Description |
|------|-------------|
| `VALVE_v4_6.docx` | Complete circuit documentation |
| `BreakoutBox_Circuit_V1_4.md` | Diagnostic tool schematic |
| `PROTOCOLE_PREMORTEM_V4_2_1.txt` | Design validation protocol |

---

## License

Personal project - Not for commercial use without authorization.

**⚠️ WARNING**: This system controls exhaust flow on a high-power turbocharged vehicle. Improper installation or failure can cause engine damage or safety hazards. Professional installation recommended.

---

*Mustang EcoBoost Exhaust Valve Controller - Mehdi - December 2025*
