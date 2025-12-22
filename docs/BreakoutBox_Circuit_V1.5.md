# Breakout Box Circuit V1.5

## Outil Diagnostic VALVE v4.7
### ESP32-C6-LCD-1.47 + INA219

---

## CHANGELOG V1.5

**Date:** Decembre 2025

**CORRECTIONS AUDIT:**

- **BAT54S SOT-23 remplace par 2x 1N5817 DO-41** (THT natif)
- **Cablage clamp explicite** (cathodes vers +3.3V)
- **Note timeout I2C ajoutee**

**CONSERVE V1.4:**

- Diviseur 15k/3.3k (ratio 5.545)
- R serie 1k protection GPIO
- INA219 shunt 0.1 Ohm

---

## BLOC A - Alimentation

**Entree:** +12V depuis VALVE J-BB pin 1

+12V_BB → fusible 500mA → [+12V_LOCAL]

[+12V_LOCAL] → 100nF X7R 25V → GND

ESP32-C6-LCD: Alimenter via USB-C (5V)

---

## BLOC B - Mesure tensions [MODIFIE V1.5]

### B1. Diviseur 12V

[+12V_LOCAL] → R1 (15k 1%) → [DIV_12V] → R2 (3.3k 1%) → GND

[DIV_12V] → R3 (1k) → GPIO4

**Calcul:** V_gpio = 12V × 3.3k / (15k + 3.3k) = 2.16V

**@ 16V:** V_gpio = 2.88V < 3.0V OK

### B2. Protection Load-Dump [MODIFIE V1.5]

**Composants:** 2x 1N5817 (DO-41 axial THT)

**Cablage:**

D1 (1N5817): Cathode → +3.3V, Anode → [DIV_12V]

D2 (1N5817): Cathode → +3.3V, Anode → [DIV_5V]

**Fonction:** Clamp a 3.3V + 0.3V = 3.6V max sur GPIO

**Calcul load-dump 29V:**

V_div = 29V × 3.3k / 18.3k = 5.23V

I_clamp = (5.23V - 3.6V) / 1k = 1.63mA (safe)

### B3. Diviseur 5V

[+5V_LOG] (J-BB pin 2) → R4 (10k 1%) → [DIV_5V] → R5 (10k 1%) → GND

[DIV_5V] → R6 (1k) → GPIO5

**Calcul:** V_gpio = 5V × 0.5 = 2.5V

### B4. Signaux logiques (direct)

AUTO_REQ (J-BB pin 3) → R7 (1k) → GPIO6

CMD (J-BB pin 5) → R8 (1k) → GPIO7

MANUAL_REQ (J-BB pin 6) → R9 (1k) → GPIO10

OUT_DRIVER (J-BB pin 4) → diviseur 15k/3.3k → R10 (1k) → GPIO11

---

## BLOC C - Mesure courant INA219

**Module:** INA219 breakout (Adafruit ou clone)

**Shunt:** 0.1 Ohm (mesure 0-3.2A pleine echelle)

**Cablage:**

VCC → +3.3V ESP32

GND → GND

SDA → GPIO21

SCL → GPIO22

VIN+ → J-BB pin 10 (I_SENSE+)

VIN- → J-BB pin 11 (I_SENSE-)

**Decouplage:** 100nF entre VCC et GND du module

**⚠️ NOTE V1.5:** Configurer timeout I2C dans firmware (Wire.setTimeOut)

---

## BLOC D - Stimulation sorties

### D1. STIM_1 (simulation pressostat)

GPIO18 → R11 (1k) → J-BB pin 8

**Fonction:** Force IN_A haut (simule pression OK)

### D2. STIM_2 (toggle manuel)

GPIO19 → R12 (1k) → J-BB pin 9

**Fonction:** Pulse CLK CD4013 (toggle etat manuel)

**⚠️ NOTE V1.5:** Pulse 100ms minimum (RC anti-rebond = 10ms)

---

## BLOC E - Interface utilisateur

**Ecran:** Integre ESP32-C6-LCD-1.47 (ST7789 172x320)

**Boutons:** Boot (GPIO9), optionnel externe

**LED:** RGB integree ou LED externe status

---

## BOM V1.5

### Semiconducteurs

| Qty | Ref | Composant | Valeur | Package |
|-----|-----|-----------|--------|---------|
| 2 | D1-D2 | Schottky clamp | 1N5817 40V 1A | DO-41 Axial THT |

### Resistances

| Qty | Ref | Valeur | Tolerance | Puissance | Package |
|-----|-----|--------|-----------|-----------|---------|
| 2 | R1, R_OUT | 15k | 1% | 1/4W | Axial MF THT |
| 2 | R2, R_OUT2 | 3.3k | 1% | 1/4W | Axial MF THT |
| 2 | R4-R5 | 10k | 1% | 1/4W | Axial MF THT |
| 6 | R3,R6-R10 | 1k | 5% | 1/4W | Axial MF THT |
| 2 | R11-R12 | 1k | 5% | 1/4W | Axial MF THT |

### Condensateurs

| Qty | Ref | Valeur | Tension | Type | Package |
|-----|-----|--------|---------|------|---------|
| 2 | C1-C2 | 100nF | 25V | X7R ceramique | Radial 5mm THT |

### Modules

| Qty | Composant | Reference |
|-----|-----------|-----------|
| 1 | MCU + Ecran | ESP32-C6-LCD-1.47 (Waveshare) |
| 1 | Mesure courant | INA219 module breakout |

### Connectique

| Qty | Composant | Reference |
|-----|-----------|-----------|
| 2 | Header femelle | 1x6 pins pas 2.54mm |
| 1 | Fusible | 500mA verre 5x20mm |

---

## CABLAGE J-BB

**Cote VALVE (males):**

```
Rangee A: [1:+12V] [2:+5V] [3:AUTO] [4:OUT] [5:CMD] [6:MAN]
Rangee B: [7:GND] [8:STIM1] [9:STIM2] [10:I+] [11:I-] [12:GND]
```

**Cote Breakout (femelles):** Miroir

---

## SCHEMA SIMPLIFIE

```
+12V_LOCAL ──┬── R1(15k) ──┬── R2(3.3k) ── GND
             │             │
             │         [DIV_12V]
             │             │
             │         R3(1k) ── GPIO4
             │             │
             │         D1(1N5817) cathode → +3.3V
             │
        100nF ── GND


+5V_LOG ──┬── R4(10k) ──┬── R5(10k) ── GND
          │             │
          │         [DIV_5V]
          │             │
          │         R6(1k) ── GPIO5
          │             │
          │         D2(1N5817) cathode → +3.3V
```

---

## PINOUT ESP32-C6-LCD

| GPIO | Fonction | Signal |
|------|----------|--------|
| 4 | ADC | DIV_12V |
| 5 | ADC | DIV_5V |
| 6 | Digital IN | AUTO_REQ |
| 7 | Digital IN | CMD |
| 10 | Digital IN | MANUAL_REQ |
| 11 | ADC | OUT_DRIVER (div) |
| 18 | Digital OUT | STIM_1 |
| 19 | Digital OUT | STIM_2 |
| 21 | I2C SDA | INA219 |
| 22 | I2C SCL | INA219 |

---

## CHECKLIST MONTAGE V1.5

- [ ] 2x 1N5817: Cathodes (bague) vers +3.3V
- [ ] Diviseurs: Verifier ratios au multimetre avant branchement
- [ ] INA219: VIN+ et VIN- en serie avec charge MAC
- [ ] Headers femelles: Alignement avec VALVE J-BB
- [ ] Fusible 500mA: Accessible pour remplacement
- [ ] Test GPIO: Tension max 3.0V sans clamp

---

**--- FIN DOCUMENT BREAKOUT BOX V1.5 ---**
