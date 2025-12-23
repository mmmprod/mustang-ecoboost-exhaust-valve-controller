# Breakout Box Circuit V1.6

## Outil Diagnostic VALVE v4.9+
### ESP32-C6-LCD-1.47 + INA219

---

## CHANGELOG V1.6

**Date:** Decembre 2025

**AUDIT FIRMWARE V1.7:**

Validation 9 points audit externe. Resultats:
- 2 bugs firmware corriges (V1.7)
- 1 bug hardware clarifie (protection existante OK)
- 5 faux positifs rejetes
- 1 documentation (ESR condensateur)

**CLARIFICATION BUG #7 (GPIO court-circuit):**

L'audit externe a identifie un risque court-circuit GPIO18/19.
Analyse du circuit V1.5 montre: **Protection DEJA presente.**

R11 (1k) entre GPIO18 et J-BB pin 8
R12 (1k) entre GPIO19 et J-BB pin 9

Calcul court-circuit cable:
I_max = 3.3V / 1000 Ohm = 3.3mA << 40mA limite GPIO

**Verdict: Aucune modification hardware requise.**

**CONSERVE V1.5:**

- Diviseur 15k/3.3k (ratio 5.545)
- Protection clamp 2x 1N5817
- INA219 shunt 0.1 Ohm
- R serie 1k sur toutes sorties GPIO

---

## BLOC A - Alimentation

**Entree:** +12V depuis VALVE J-BB pin 1

+12V_BB → fusible 500mA → [+12V_LOCAL]

[+12V_LOCAL] → C1 (100nF X7R 25V) → GND

ESP32-C6-LCD: Alimenter via USB-C (5V)

---

## BLOC B - Mesure tensions

### B1. Diviseur 12V

[+12V_LOCAL] → R1 (15k 1%) → [DIV_12V] → R2 (3.3k 1%) → GND

[DIV_12V] → R3 (1k) → GPIO4

**Calcul:** V_gpio = 12V x 3.3k / (15k + 3.3k) = 2.16V

**@ 16V:** V_gpio = 2.88V < 3.0V OK

### B2. Protection Load-Dump (Clamp)

**Composants:** 2x 1N5817 (DO-41 axial THT)

**Cablage:**

D1 (1N5817): Anode → [DIV_12V], Cathode → +3.3V

D2 (1N5817): Anode → [DIV_5V], Cathode → +3.3V

**Fonction:** Clamp a 3.3V + 0.3V = 3.6V max sur GPIO

**Calcul load-dump 29V:**

V_div = 29V x 3.3k / 18.3k = 5.23V

I_clamp = (5.23V - 3.6V) / 1k = 1.63mA (safe pour 1N5817)

### B3. Diviseur 5V

[+5V_LOG] (J-BB pin 2) → R4 (10k 1%) → [DIV_5V] → R5 (10k 1%) → GND

[DIV_5V] → R6 (1k) → GPIO5

**Calcul:** V_gpio = 5V x 0.5 = 2.5V

### B4. Signaux logiques (entrees)

AUTO_REQ (J-BB pin 3) → R7 (1k) → GPIO6

CMD (J-BB pin 5) → R8 (1k) → GPIO7

MANUAL_REQ (J-BB pin 6) → R9 (1k) → GPIO10

OUT_DRIVER (J-BB pin 4) → R13 (15k) → [DIV_OUT] → R14 (3.3k) → GND

[DIV_OUT] → R10 (1k) → GPIO11

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

**Decouplage:** C2 (100nF) entre VCC et GND du module

**Firmware V1.7:** Timeout I2C 100ms, validation isfinite(), plage [-100, 1000] mA

---

## BLOC D - Stimulation sorties [VERIFIE V1.6]

### D1. STIM_1 (simulation pressostat)

GPIO18 → R11 (1k) → J-BB pin 8

**Protection court-circuit:** I_max = 3.3mA (R11 limite)

**Fonction:** Force IN_A haut (simule pression OK)

**Firmware V1.7:** Timeout auto-OFF 60s

### D2. STIM_2 (toggle manuel)

GPIO19 → R12 (1k) → J-BB pin 9

**Protection court-circuit:** I_max = 3.3mA (R12 limite)

**Fonction:** Pulse CLK CD4013 (toggle etat manuel)

**Firmware V1.7:** Pulse 100ms

**NOTE V1.6:** R11-R12 = 1k fournissent protection adequate.
Le circuit VALVE a R 1k supplementaire (total 2k en serie).
Courant nominal = (3.3V - 0.7V) / 2k = 1.3mA (74HC14 seuil 0.9mA OK)

---

## BLOC E - Interface utilisateur

**Ecran:** Integre ESP32-C6-LCD-1.47 (ST7789 172x320)

**Boutons:** Boot (GPIO9), optionnel externe

**LED:** RGB integree ou LED externe status

---

## BOM V1.6

### Semiconducteurs

| Qty | Ref | Composant | Valeur | Package |
|-----|-----|-----------|--------|---------|
| 2 | D1-D2 | Schottky clamp | 1N5817 40V 1A | DO-41 Axial THT |

### Resistances

| Qty | Ref | Valeur | Tolerance | Puissance | Package |
|-----|-----|--------|-----------|-----------|---------|
| 2 | R1, R13 | 15k | 1% | 1/4W | Axial MF THT |
| 2 | R2, R14 | 3.3k | 1% | 1/4W | Axial MF THT |
| 2 | R4-R5 | 10k | 1% | 1/4W | Axial MF THT |
| 8 | R3,R6-R12 | 1k | 5% | 1/4W | Axial MF THT |

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

## PINOUT ESP32-C6-LCD

| GPIO | Fonction | Signal | Protection |
|------|----------|--------|------------|
| 4 | ADC | DIV_12V | R3 1k + D1 clamp |
| 5 | ADC | DIV_5V | R6 1k + D2 clamp |
| 6 | Digital IN | AUTO_REQ | R7 1k |
| 7 | Digital IN | CMD | R8 1k |
| 10 | Digital IN | MANUAL_REQ | R9 1k |
| 11 | ADC | OUT_DRIVER (div) | R10 1k |
| 18 | Digital OUT | STIM_1 | R11 1k |
| 19 | Digital OUT | STIM_2 | R12 1k |
| 21 | I2C SDA | INA219 | - |
| 22 | I2C SCL | INA219 | - |

---

## VALIDATION AUDIT V1.6

### Bugs firmware corriges (V1.7)

| # | Probleme | Correction |
|---|----------|------------|
| 3 | readVoltageOut() sans validation | Ajout raw<10 + saturation |
| 6 | INA219 peut retourner NaN | Ajout isfinite() avant plage |

### Bug hardware clarifie

| # | Probleme | Statut |
|---|----------|--------|
| 7 | GPIO court-circuit | R11-R12 1k DEJA presentes, aucune modif |

### Faux positifs rejetes

| # | Probleme | Raison rejet |
|---|----------|--------------|
| 1 | Float == constante | Constantes entieres, representation exacte |
| 2 | Race condition g_stim1_start | Mono-thread, pas de concurrence |
| 5 | STIM1 bloque si crash | WDT + timeout 60s = double protection |
| 8 | NCV2931 thermique | Charge reelle 10mA, pas 100mA |

### Documentation

| # | Probleme | Action |
|---|----------|--------|
| 9 | ESR condensateur | Specifier Low-ESR dans BOM VALVE |

---

## CHECKLIST MONTAGE V1.6

- [ ] 2x 1N5817: Cathodes (bague) vers +3.3V
- [ ] Diviseurs: Verifier ratios au multimetre avant branchement
- [ ] R11-R12: Confirmer 1k (protection GPIO sorties)
- [ ] INA219: VIN+ et VIN- en serie avec charge MAC
- [ ] Headers femelles: Alignement avec VALVE J-BB
- [ ] Fusible 500mA: Accessible pour remplacement
- [ ] Test GPIO: Tension max 3.0V sans clamp actif

---

## FIRMWARE ASSOCIE

**Version:** BreakoutBox_V1_7.ino

**Protections implementees:**
- INA219: isfinite() + plage [-100, 1000] mA
- ADC: Detection capteur HS (raw<10) et saturation
- readVoltageOut(): Validation coherente avec 12V/5V
- NVS: Validation calibration [0.8, 1.2]
- STIM1: Timeout auto-OFF 60s
- Watchdog: 5s reset automatique

---

**--- FIN DOCUMENT BREAKOUT BOX V1.6 ---**
