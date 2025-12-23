# Breakout Box Circuit V1.7

## Outil Diagnostic VALVE v4.9+
### ESP32-C6 Touch LCD 1.9" (Waveshare) + INA219

---

## CHANGELOG V1.7

**Date:** Decembre 2025

**MIGRATION ECRAN TACTILE:**

Module change: ESP32-C6-LCD-1.47 → ESP32-C6 Touch LCD 1.9"

Raison: Interface technicien tactile (toucher = tester)

**NOUVEAU PINOUT V1.7:**

GPIO reserves par hardware LCD/Touch:
- LCD SPI: GPIO4, 5, 6, 7, 14, 15 (INTERDIT)
- Touch I2C: GPIO8 (SCL), GPIO18 (SDA)
- Touch INT: GPIO22
- USB: GPIO12, 13

Remapping complet signaux BreakoutBox:

| Signal | V1.6 (ancien) | V1.7 (nouveau) | Raison |
|--------|---------------|----------------|--------|
| DIV_12V | GPIO4 | **GPIO0** | GPIO4 = LCD_DIN |
| DIV_5V | GPIO5 | **GPIO1** | GPIO5 = LCD_CLK |
| OUT_DIV | GPIO11 | **GPIO2** | Regrouper ADC |
| AUTO_REQ | GPIO6 | **GPIO3** | GPIO6 = LCD_DC |
| CMD | GPIO7 | **GPIO19** | GPIO7 = LCD_CS |
| STIM1 | GPIO18 | **GPIO20** | GPIO18 = Touch SDA |
| STIM2 | GPIO19 | **GPIO21** | Decaler |
| MANUAL_REQ | GPIO10 | **GPIO23** | Liberer zone |
| I2C SDA | GPIO21 | **GPIO18** | Hardware Touch |
| I2C SCL | GPIO22 | **GPIO8** | Hardware Touch |

**I2C PARTAGE:**

Bus unique GPIO8/GPIO18 pour:
- INA219 @ 0x40 (mesure courant)
- CST816 @ 0x15 (controleur tactile)

Pas de conflit adresse. Fonctionnement simultane OK.

**CONSERVE V1.6:**

- Diviseur 15k/3.3k (ratio 5.545)
- Protection clamp 2x 1N5817
- INA219 shunt 0.1 Ohm
- R serie 1k sur toutes sorties GPIO
- Firmware protections (isfinite, timeout, WDT)

---

## BLOC A - Alimentation

**Entree:** +12V depuis VALVE J-BB pin 1

+12V_BB → fusible 500mA → [+12V_LOCAL]

[+12V_LOCAL] → C1 (100nF X7R 25V) → GND

ESP32-C6 Touch LCD: Alimenter via USB-C (5V)

---

## BLOC B - Mesure tensions

### B1. Diviseur 12V

[+12V_LOCAL] → R1 (15k 1%) → [DIV_12V] → R2 (3.3k 1%) → GND

[DIV_12V] → R3 (1k) → **GPIO0**

**Calcul:** V_gpio = 12V x 3.3k / (15k + 3.3k) = 2.16V

**@ 16V:** V_gpio = 2.88V < 3.0V OK

### B2. Protection Load-Dump (Clamp)

**Composants:** 2x 1N5817 (DO-41 axial THT)

**Cablage:**

D1 (1N5817): Anode → [DIV_12V], Cathode → +3.3V

D2 (1N5817): Anode → [DIV_5V], Cathode → +3.3V

**Fonction:** Clamp a 3.3V + 0.3V = 3.6V max sur GPIO

### B3. Diviseur 5V

[+5V_LOG] (J-BB pin 2) → R4 (10k 1%) → [DIV_5V] → R5 (10k 1%) → GND

[DIV_5V] → R6 (1k) → **GPIO1**

**Calcul:** V_gpio = 5V x 0.5 = 2.5V

### B4. Diviseur sortie driver

OUT_DRIVER (J-BB pin 4) → R13 (15k) → [DIV_OUT] → R14 (3.3k) → GND

[DIV_OUT] → R10 (1k) → **GPIO2**

### B5. Signaux logiques (entrees)

AUTO_REQ (J-BB pin 3) → R7 (1k) → **GPIO3**

CMD (J-BB pin 5) → R8 (1k) → **GPIO19**

MANUAL_REQ (J-BB pin 6) → R9 (1k) → **GPIO23**

---

## BLOC C - Mesure courant INA219

**Module:** INA219 HW-831B (ou Adafruit)

**Specs module:**
- VCC: 3-5V
- Mesure: 0-26V, 0-3.2A
- Shunt: 0.1 Ohm integre
- Adresse: 0x40 (defaut)
- Resolution: 0.8mA

**Cablage:**

VCC → +3.3V ESP32

GND → GND commun

SDA → **GPIO18** (partage Touch)

SCL → **GPIO8** (partage Touch)

VIN+ → J-BB pin 10 (I_SENSE+, cote +12V)

VIN- → J-BB pin 11 (I_SENSE-, cote charge valve)

**Decouplage:** C2 (100nF) entre VCC et GND du module

**Note V1.7:** I2C partage avec controleur tactile CST816 (0x15). Pas de conflit.

---

## BLOC D - Stimulation sorties

### D1. STIM_1 (simulation pressostat)

**GPIO20** → R11 (1k) → J-BB pin 8

**Protection court-circuit:** I_max = 3.3V / 1k = 3.3mA

**Fonction:** Force IN_A haut (simule pression OK)

### D2. STIM_2 (toggle manuel)

**GPIO21** → R12 (1k) → J-BB pin 9

**Protection court-circuit:** I_max = 3.3mA

**Fonction:** Pulse CLK CD4013 (toggle etat manuel)

---

## BLOC E - Interface utilisateur

**Ecran:** Integre ESP32-C6 Touch LCD 1.9" (ST7789 170x320)

**Tactile:** CST816 sur I2C (0x15), INT sur GPIO22

**Interaction:** Toucher ecran = lancer test

**Boutons hardware:** Boot (GPIO9) - backup si tactile HS

---

## BOM V1.7

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
| 1 | MCU + Ecran tactile | ESP32-C6 Touch LCD 1.9" (Waveshare) |
| 1 | Mesure courant | INA219 HW-831B module |

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

## PINOUT ESP32-C6 Touch LCD 1.9" - V1.7

### GPIO utilises BreakoutBox

| GPIO | Fonction | Signal | Protection |
|------|----------|--------|------------|
| 0 | ADC | DIV_12V | R3 1k + D1 clamp |
| 1 | ADC | DIV_5V | R6 1k + D2 clamp |
| 2 | ADC | DIV_OUT | R10 1k |
| 3 | Digital IN | AUTO_REQ | R7 1k |
| 8 | I2C SCL | INA219 + Touch | Hardware |
| 18 | I2C SDA | INA219 + Touch | Hardware |
| 19 | Digital IN | CMD | R8 1k |
| 20 | Digital OUT | STIM_1 | R11 1k |
| 21 | Digital OUT | STIM_2 | R12 1k |
| 22 | Digital IN | Touch INT | Interne |
| 23 | Digital IN | MANUAL_REQ | R9 1k |

### GPIO reserves hardware (NE PAS UTILISER)

| GPIO | Fonction hardware |
|------|-------------------|
| 4 | LCD_DIN (MOSI) |
| 5 | LCD_CLK (SCK) |
| 6 | LCD_DC |
| 7 | LCD_CS |
| 9 | BOOT button |
| 12 | USB D- |
| 13 | USB D+ |
| 14 | LCD_RST |
| 15 | LCD_BL |

---

## SCHEMA CONNEXIONS SIMPLIFIE

```
ESP32-C6 Touch LCD 1.9"
========================

       GPIO0 ←──[1k]──┬──[3.3k]──GND
                      │
       DIV_12V ───────┴──[15k]──── +12V_LOCAL
                      │
                    [D1]──── +3.3V (clamp)


       GPIO1 ←──[1k]──┬──[10k]───GND
                      │
       DIV_5V ────────┴──[10k]──── +5V_LOG
                      │
                    [D2]──── +3.3V (clamp)


       GPIO2 ←──[1k]──┬──[3.3k]──GND
                      │
       DIV_OUT ───────┴──[15k]──── OUT_DRIVER


       GPIO3 ←──[1k]────────────── AUTO_REQ
       GPIO19 ←─[1k]────────────── CMD
       GPIO23 ←─[1k]────────────── MANUAL_REQ


       GPIO20 ──[1k]────────────── STIM1 → J-BB pin8
       GPIO21 ──[1k]────────────── STIM2 → J-BB pin9


       GPIO8  ────────────────┬─── INA219 SCL
       GPIO18 ────────────────┼─── INA219 SDA
                              │
                              └─── (Touch interne)


       INA219:
         VIN+ ← ─────────────── J-BB pin10 (I+)
         VIN- ─────────────── → J-BB pin11 (I-)
         (en serie avec charge valve)
```

---

## VERIFICATION I2C

**Scanner I2C attendu:**

```
Scanning I2C bus...
  0x15 - CST816 Touch Controller
  0x40 - INA219 Current Sensor
Done.
```

**Test rapide:**

```cpp
Wire.begin(18, 8);  // SDA=18, SCL=8
Wire.beginTransmission(0x40);
if (Wire.endTransmission() == 0) Serial.println("INA219 OK");
Wire.beginTransmission(0x15);
if (Wire.endTransmission() == 0) Serial.println("CST816 OK");
```

---

## CHECKLIST MONTAGE V1.7

- [ ] ESP32-C6 Touch LCD 1.9" (pas 1.47!)
- [ ] INA219 module: SDA→GPIO18, SCL→GPIO8
- [ ] 2x 1N5817: Cathodes (bague) vers +3.3V
- [ ] Diviseurs 12V: R1=15k, R2=3.3k vers GPIO0
- [ ] Diviseurs 5V: R4=R5=10k vers GPIO1
- [ ] Diviseurs OUT: R13=15k, R14=3.3k vers GPIO2
- [ ] AUTO_REQ: R7=1k vers GPIO3
- [ ] CMD: R8=1k vers GPIO19
- [ ] MANUAL: R9=1k vers GPIO23
- [ ] STIM1: GPIO20 → R11=1k → J-BB pin8
- [ ] STIM2: GPIO21 → R12=1k → J-BB pin9
- [ ] INA219 VIN+/VIN-: En serie avec +12V vers valve
- [ ] Fusible 500mA sur +12V entree
- [ ] USB-C pour alimentation ESP32

---

## FIRMWARE ASSOCIE

**Version:** BreakoutBox_V1_9.ino

**Fonctionnalites:**
- Interface tactile (toucher = test)
- Ecran resultat GO/NOGO plein ecran
- Rapport one-liner pour IA
- I2C partage Touch + INA219
- Protections V1.7 conservees (isfinite, timeout, WDT)

---

## MIGRATION V1.6 → V1.7

**Si circuit V1.6 existant:**

Recabler selon tableau:

| Fil actuel | Deplacer vers |
|------------|---------------|
| GPIO4 | GPIO0 |
| GPIO5 | GPIO1 |
| GPIO6 | GPIO3 |
| GPIO7 | GPIO19 |
| GPIO11 | GPIO2 |
| GPIO18 | GPIO20 |
| GPIO19 | GPIO21 |
| GPIO10 | GPIO23 |
| GPIO21 (SDA) | GPIO18 |
| GPIO22 (SCL) | GPIO8 |

**Module ESP32:** Remplacer par Touch LCD 1.9"

---

**--- FIN DOCUMENT BREAKOUT BOX V1.7 ---**
