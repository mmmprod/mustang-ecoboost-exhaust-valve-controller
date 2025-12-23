# 🔥 Mustang EcoBoost Exhaust Valve Controller

> **Boost-activated exhaust valve control for 2015-2023 Ford Mustang EcoBoost**  
> Open source • Automotive-grade • Zero microcontroller

![Version](https://img.shields.io/badge/version-4.10-blue)
![License](https://img.shields.io/badge/license-MIT-green)
![ISO 7637-2](https://img.shields.io/badge/ISO%207637--2-compliant-orange)
![Stage](https://img.shields.io/badge/tested-600%2B%20HP-red)

---

## What is this?

A standalone electronic controller that **automatically opens your aftermarket exhaust valve when you're on boost** — and keeps it closed when cruising.

No tuning. No ECU flash. No app. Just pure analog logic that responds in milliseconds.

**Built for:**
- Active exhaust valves (Varex, X-Force, custom)
- Pneumatic actuators (fail-closed type)
- Street cars that need to be quiet... until they're not

---

## Features

| Feature | Description |
|---------|-------------|
| ⚡ **Instant response** | Valve opens in ~100ms when boost detected |
| 🛡️ **Anti-flutter** | 2-second closing delay prevents hunting during gear changes |
| 🔘 **Manual override** | One-button toggle to force valve open |
| 🚗 **Automotive-grade** | Survives load dump, cold crank, EMI — ISO 7637-2 compliant |
| 🔧 **Adjustable threshold** | Set your own boost trigger point via ISE30A display |
| 💀 **Fail-safe** | Valve closes on power loss (fail-closed actuator) |
| 🔌 **Plug & play** | Works with any 12V pneumatic solenoid |

---

## How it works

```
                    ┌─────────────────┐
   Intake Manifold ─┤  SMC ISE30A     ├─── Boost signal
   (vacuum/boost)   │  Pressure Switch│
                    └────────┬────────┘
                             │
                             ▼
                    ┌─────────────────┐
                    │  Logic Board    │
                    │  ─────────────  │
   Manual Button ──▶│  74HC14 buffer  │
                    │  CD4013 toggle  │──▶ Status LED
                    │  RC tempo 2s    │
                    │  OR gate        │
                    └────────┬────────┘
                             │
                             ▼
                    ┌─────────────────┐
   12V Compressor ──┤  MAC 35A        ├──▶ Pneumatic Actuator
   (90-120 PSI)     │  Solenoid Valve │    (opens exhaust valve)
                    └─────────────────┘
```

**Logic:**
- Boost ≥ 0.20 bar (2.9 PSI) → Valve **OPEN** (instant)
- Boost < 0.15 bar (2.2 PSI) → Valve **CLOSED** (after 2s delay)
- Manual button ON → Valve **FORCED OPEN** (ignores boost)

---

## Specs

| Parameter | Value |
|-----------|-------|
| Input voltage | 9-16V DC (automotive 12V) |
| Transient protection | 60V (load dump) |
| Cold crank | Down to 6V |
| Quiescent current | < 50mA |
| Solenoid drive | Up to 500mA (BTS5090 smart switch) |
| Opening threshold | 0.20 bar / 2.9 PSI (adjustable) |
| Hysteresis | 0.05 bar / 0.7 PSI |
| Opening delay | 0 ms (instant) |
| Closing delay | 2 seconds (anti-flutter) |
| Operating temp | -40°C to +85°C |

---

## Bill of Materials

### Electronics (~€50)

| Component | Value | Package |
|-----------|-------|---------|
| TVS diode | 1.5KE18CA | DO-15 |
| Voltage regulator | NCV2931AZ-5.0G | TO-92 |
| Schmitt buffer | 74HC14 | DIP-14 |
| D flip-flop | CD4013 | DIP-14 |
| High-side driver | BTS5090-1EJA | TSSOP-8 + adapter |
| Schottky diodes | 1N5822, 1N5819 | Axial |
| Signal diodes | 1N4148 (x3) | DO-35 |
| Zener | 1N4733A 5.1V | DO-41 |
| Resistors | Various (see BOM) | Axial 1/4W |
| Capacitors | 100nF, 10µF, etc. | Radial |
| Ferrite bead | 90Ω @ 100MHz | Axial |
| Relay | Omron G5Q-1-HA | PCB |
| Fuses | 1A (x2) | Glass + blade |

### Pneumatic (~€150)

| Component | Reference |
|-----------|-----------|
| Pressure switch | SMC ISE30A-01-N |
| Solenoid valve | MAC 35A-AAA-DDBA-1BA |
| Pressure regulator | SMC IR1000-01BG |
| PRV safety valve | 3 bar preset |

### External

- Toyota-style 22mm LED push button
- Pneumatic fittings (6mm push-in)
- 12V air compressor with tank

---

## Build

### Circuit board

Designed for **stripboard/veroboard** construction. No custom PCB required.

All components are through-hole (THT) except the BTS5090 which uses a TSSOP-to-DIP adapter.

### Enclosure

Recommended: **110 × 250 × 190 mm** ABS enclosure, trunk-mounted.

- Max ambient temperature: 50°C (trunk)
- Keep away from exhaust heat
- Aluminum heat shield on pneumatic lines near exhaust

---

## Configuration

### ISE30A Pressure Switch Setup

1. Hold **SET** for 3 seconds to enter menu
2. Set parameters:

| Parameter | Value | Description |
|-----------|-------|-------------|
| P1 (SET) | 20.0 kPa | Opening threshold (0.20 bar) |
| HYST | 5.0 kPa | Hysteresis (0.05 bar) |
| OUT1 | NPN | Output type |
| FUNC | P1-HYS | Hysteresis mode |

3. Press **SET** to save

### Fine-tuning

- **Valve opens during cruise?** → Increase P1 threshold
- **Flutter on gear changes?** → Closing delay handles this (2s)
- **Want longer delay?** → Increase R12 (150kΩ → 3s)

---

## Safety

### ⚠️ Critical

- **PRV is MANDATORY** — The 3-bar pressure relief valve protects the actuator if the regulator or solenoid fails
- **Fail-closed actuator required** — On power loss, valve must close (safe default)
- **No forced-closed mode** — Forcing the valve closed under boost = backpressure = 💥

### Failure modes

| Failure | Result | Detection |
|---------|--------|-----------|
| Power loss | Valve closes | Silent exhaust |
| BTS5090 short | Valve stuck open | Loud at idle |
| Solenoid stuck | Valve stuck closed | Power loss, high EGT |
| Regulator fail | PRV opens at 3 bar | Hissing sound |

---

## Tested on

- **2016 Ford Mustang EcoBoost**
- Stage 4+ E85 tune
- 600+ HP / 700+ lb-ft
- Daily driven

---

## Project files

```
├── docs/
│   ├── VALVE_v4_10.md          # Full circuit documentation
│   ├── BOM_VALVE_v4_10.md      # Bill of materials
│   └── PREMORTEM_V4_2_4.txt    # Design validation protocol
└── README.md
```

---

## Why no microcontroller?

1. **Reliability** — Discrete logic doesn't crash or need firmware updates
2. **Speed** — Analog response is instant, no loop delay
3. **Simplicity** — Fewer failure modes, easier to debug
4. **Proven** — Same approach used in OEM valve controllers

The entire logic fits in 3 chips: one buffer, one flip-flop, one driver.

---

## License

MIT License — Do whatever you want with it.

---

## Contributing

Found a bug? Have an improvement? Open an issue or PR.

**Ground rules:**
- All changes must pass PREMORTEM validation
- No SMD components (stripboard compatibility)
- No microcontrollers (keep it analog)
- Document your math

---

## Disclaimer

DIY project for educational purposes. You're responsible for your own vehicle modifications.

That said — this design has been validated against ISO 7637-2 automotive transient standards and tested on a 600+ HP street car. It works.

---

<p align="center">
  <b>Built for boost. Silent when you need it. Loud when you want it.</b>
</p>
