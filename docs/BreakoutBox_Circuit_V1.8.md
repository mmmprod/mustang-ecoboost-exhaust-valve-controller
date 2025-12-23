# Breakout Box Circuit V1.8

## Outil Diagnostic Universel
### ESP32-C6 Touch LCD 1.9" + INA219 + DB-15

---

## CHANGELOG V1.8

**Date:** Decembre 2025

**REFONTE VERSATILITE:**

- Connecteur DB-15 universel en facade
- Port USB-C deporte en facade (cable panel mount)
- Cables adaptateurs par projet (VALVE, AUDIO, etc.)
- Nommage generique des E/S

---

## CONCEPT

```
┌─────────────────────────────────────────────────────────┐
│                    BREAKOUT BOX                         │
│                                                         │
│   ┌─────────────────────────────────────────────────┐   │
│   │                                                 │   │
│   │            ECRAN TACTILE 1.9"                   │   │
│   │              (170 x 320 px)                     │   │
│   │                                                 │   │
│   └─────────────────────────────────────────────────┘   │
│                                                         │
│   [USB-C]     [DB-15]      (o)PWR  (o)RUN    [BOOT]    │
│                                                         │
└─────────────────────────────────────────────────────────┘
        │           │
        │           └──────────────────────┐
        │                                  │
        ▼                                  ▼
   Alimentation                    Cable adaptateur
   5V USB                          vers circuit teste
                                   (VALVE, AUDIO, etc.)
```

**Principe:**
- La box a UN connecteur DB-15 femelle universel
- Chaque projet a son cable adaptateur: DB-15 male → connecteur du circuit
- Tu branches, tu testes

---

## BLOC A - Alimentation

### A1. USB-C Panel Mount

**Composant:** Cable USB-C panel mount 15cm

| Fournisseur | Reference | Prix |
|-------------|-----------|------|
| SparkFun | CAB-15455 | ~$15 |
| Amazon | AAOTOKK USB-C panel | ~$10 |
| AliExpress | USB-C chassis mount | ~$5 |

**Cablage:**

```
[Facade USB-C femelle] <--cable 15cm--> [ESP32 USB-C]
       ^
   Chargeur 5V / PC / Powerbank
```

### A2. Protection 12V entree

+12V (DB-15 pin1) -> F1 (fusible 500mA) -> [+12V_INT]

[+12V_INT] -> C1 (100nF X7R 25V) -> GND

---

## BLOC B - Connecteur DB-15 Universel

### Module Breakout Board

**Composant:** PremierCable DB15-G2-01 (ou equivalent)

- DB-15 femelle avec borniers a vis
- Numerotation 1-15 + GND
- Montage facade par vis DB-15

```
    DB-15 Femelle (vue facade)
    ________________________
    \  1  2  3  4  5  6  7  8  /
     \  9 10 11 12 13 14 15   /
      ------------------------
```

### Pinout DB-15

| Pin | Signal | Type | Description |
|-----|--------|------|-------------|
| 1 | +12V | PWR | Alimentation 12V |
| 2 | +5V | PWR | Alimentation 5V logique |
| 3 | GND | PWR | Masse commune |
| 4 | AIN1 | ADC | Analog IN 0-20V (diviseur interne) |
| 5 | AIN2 | ADC | Analog IN 0-10V (diviseur interne) |
| 6 | AIN3 | ADC | Analog IN 0-3.3V (direct) |
| 7 | DIN1 | GPIO | Digital IN 1 |
| 8 | DIN2 | GPIO | Digital IN 2 |
| 9 | DIN3 | GPIO | Digital IN 3 |
| 10 | DOUT1 | GPIO | Digital OUT 1 |
| 11 | DOUT2 | GPIO | Digital OUT 2 |
| 12 | I+ | SENSE | Mesure courant + (vers INA219) |
| 13 | I- | SENSE | Mesure courant - (vers INA219) |
| 14 | GND | PWR | Masse (redondant) |
| 15 | SPARE | - | Reserve futur |

### Cablage interne (borniers -> circuits)

| Bornier | Signal | Vers circuit interne |
|---------|--------|----------------------|
| 1 | +12V | F1 fusible 500mA entree |
| 2 | +5V | Alim externe 5V (si utilisee) |
| 3 | GND | GND commun stripboard |
| 4 | AIN1 | R1 (15k) diviseur 0-20V |
| 5 | AIN2 | R4 (10k) diviseur 0-10V |
| 6 | AIN3 | R7 (1k) -> GPIO2 |
| 7 | DIN1 | R8 (1k) -> GPIO3 |
| 8 | DIN2 | R9 (1k) -> GPIO19 |
| 9 | DIN3 | R10 (1k) -> GPIO23 |
| 10 | DOUT1 | R11 (1k) <- GPIO20 |
| 11 | DOUT2 | R12 (1k) <- GPIO21 |
| 12 | I+ | INA219 VIN+ |
| 13 | I- | INA219 VIN- |
| 14 | GND | GND commun stripboard |
| 15 | SPARE | Non connecte |
| GND | GND | GND commun stripboard |

---

## BLOC C - Entrees analogiques

### C1. AIN1 - Mesure haute tension (0-20V)

DB-15 pin4 -> R1 (15k 1%) -> [DIV_1] -> R2 (3.3k 1%) -> GND

[DIV_1] -> D1 (1N5817) anode, cathode -> +3.3V (clamp)

[DIV_1] -> R3 (1k) -> **GPIO0**

**Calcul:** Ratio = 0.180, V_max = 18.3V (marge a 20V avec clamp)

### C2. AIN2 - Mesure moyenne tension (0-10V)

DB-15 pin5 -> R4 (10k 1%) -> [DIV_2] -> R5 (10k 1%) -> GND

[DIV_2] -> D2 (1N5817) anode, cathode -> +3.3V (clamp)

[DIV_2] -> R6 (1k) -> **GPIO1**

**Calcul:** Ratio = 0.5, V_max = 6.6V (marge a 10V avec clamp)

### C3. AIN3 - Mesure directe (0-3.3V)

DB-15 pin6 -> R7 (1k) -> **GPIO2**

---

## BLOC D - Entrees digitales

DB-15 pin7 (DIN1) -> R8 (1k) -> **GPIO3**

DB-15 pin8 (DIN2) -> R9 (1k) -> **GPIO19**

DB-15 pin9 (DIN3) -> R10 (1k) -> **GPIO23**

---

## BLOC E - Sorties digitales

**GPIO20** -> R11 (1k) -> DB-15 pin10 (DOUT1)

**GPIO21** -> R12 (1k) -> DB-15 pin11 (DOUT2)

---

## BLOC F - Mesure courant INA219

**Module:** INA219 (Adafruit ou HW-831B)

VCC -> +3.3V ESP32

GND -> GND

SDA -> **GPIO18**

SCL -> **GPIO8**

VIN+ -> DB-15 pin12 (I+)

VIN- -> DB-15 pin13 (I-)

---

## BLOC G - Interface

**Ecran:** ESP32-C6 Touch LCD 1.9" (Waveshare)

**Tactile:** CST816 sur I2C 0x15

**Bouton BOOT:** GPIO9 (secours)

---

## PINOUT ESP32-C6 V1.8

| GPIO | Signal | DB-15 Pin |
|------|--------|-----------|
| 0 | AIN1 | 4 |
| 1 | AIN2 | 5 |
| 2 | AIN3 | 6 |
| 3 | DIN1 | 7 |
| 8 | I2C SCL | - |
| 18 | I2C SDA | - |
| 19 | DIN2 | 8 |
| 20 | DOUT1 | 10 |
| 21 | DOUT2 | 11 |
| 22 | Touch INT | - |
| 23 | DIN3 | 9 |

**Reserves LCD:** 4, 5, 6, 7, 9, 12, 13, 14, 15

---

## BOM V1.8

### Connectique

| Qty | Composant | Reference |
|-----|-----------|-----------|
| 1 | DB-15 breakout board femelle | PremierCable DB15-G2-01 |
| 1 | USB-C panel mount 15cm | SparkFun CAB-15455 ou equiv |

### Semiconducteurs

| Qty | Ref | Composant | Package |
|-----|-----|-----------|---------|
| 2 | D1-D2 | 1N5817 | DO-41 Axial |

### Resistances

| Qty | Ref | Valeur | Package |
|-----|-----|--------|---------|
| 1 | R1 | 15k 1% | Axial MF |
| 1 | R2 | 3.3k 1% | Axial MF |
| 2 | R4-R5 | 10k 1% | Axial MF |
| 6 | R3,R6-R12 | 1k 5% | Axial MF |

### Condensateurs

| Qty | Ref | Valeur | Package |
|-----|-----|--------|---------|
| 2 | C1-C2 | 100nF 25V X7R | Radial 5mm |

### Modules

| Qty | Composant | Reference |
|-----|-----------|-----------|
| 1 | MCU + Ecran | ESP32-C6 Touch LCD 1.9" Waveshare |
| 1 | Mesure courant | INA219 module |

### Divers

| Qty | Composant |
|-----|-----------|
| 1 | Fusible 500mA 5x20 |
| 1 | Boitier ABS ~120x80x40mm |

---

## CABLE ADAPTATEUR VALVE

### Description

Cable: DB-15 male <-> JST XH 2x6

Longueur: 30-50cm

### Cablage DB-15 -> JST VALVE

**Cable:** DB-15 male fils nus (AliExpress D-SUB15)

| DB-15 Pin | Couleur fil | Signal Box | JST Pin | Signal VALVE |
|-----------|-------------|------------|---------|--------------|
| 1 | **Noir** | +12V | A1 | +12V_PROT |
| 2 | **Marron** | +5V | A2 | +5V_LOG |
| 3 | **Rouge** | GND | B1 | GND |
| 4 | **Orange** | AIN1 | A4 | OUT_DRIVER |
| 7 | **Bleu** | DIN1 | A3 | AUTO_REQ |
| 8 | **Violet** | DIN2 | A5 | CMD |
| 9 | **Gris** | DIN3 | A6 | MANUAL_REQ |
| 10 | **Blanc** | DOUT1 | B2 | STIM_PRESS |
| 11 | **Noir/Blanc** | DOUT2 | B3 | STIM_TOGGLE |
| 12 | **Marron/Blanc** | I+ | B4 | I_SENSE+ |
| 13 | **Rose** | I- | B5 | I_SENSE- |
| 14 | **Vert clair** | GND | B6 | GND |

**Non utilises:** Jaune (pin 5), Vert (pin 6), Bleu clair (pin 15)

### Code couleur DB-15 complet (reference)

| Pin | Couleur |
|-----|---------|
| 1 | Noir |
| 2 | Marron |
| 3 | Rouge |
| 4 | Orange |
| 5 | Jaune |
| 6 | Vert |
| 7 | Bleu |
| 8 | Violet |
| 9 | Gris |
| 10 | Blanc |
| 11 | Noir/Blanc |
| 12 | Marron/Blanc |
| 13 | Rose |
| 14 | Vert clair |
| 15 | Bleu clair |

---

## CONNECTEUR JST COTE VALVE v4.10

### Embase a ajouter sur circuit VALVE

**Composant:** JST XH 2x6 male (embase)

```
JST XH 2x6 Male (vue dessus)
┌───────────────────────┐
│ A1  A2  A3  A4  A5  A6 │
│ B1  B2  B3  B4  B5  B6 │
└───────────────────────┘
```

### Connexions sur VALVE v4.10

| JST Pin | Signal | Point de connexion VALVE |
|---------|--------|--------------------------|
| A1 | +12V_PROT | Apres D1 (1N5822) |
| A2 | +5V_LOG | Sortie U1 (NCV2931) |
| A3 | AUTO_REQ | Sortie U2 pin2 (74HC14) |
| A4 | OUT_DRIVER | Sortie U3 (BTS5090) vers electrovalve |
| A5 | CMD | Point [VALVE_CMD] entree U3 |
| A6 | MANUAL_REQ | Sortie U4 pin1 (CD4013 Q) |
| B1 | GND | Masse |
| B2 | STIM_PRESS | Point avant R5 (entree 74HC14 pin1) |
| B3 | STIM_TOGGLE | Point avant R6 (entree CD4013 CLK) |
| B4 | I_SENSE+ | +12V_PROT |
| B5 | I_SENSE- | Ligne +12V vers electrovalve |
| B6 | GND | Masse |

**Note:** I_SENSE+ et I_SENSE- forment un point de coupure pour inserer le shunt INA219 en serie avec la charge.

---

## FIRMWARE MODES

### Mode VALVE

- Affiche: V12, V5, I_valve, AUTO, CMD, MANUAL
- Test: Toucher = cycle stimulation complet
- Resultat: GO / NOGO

### Mode Voltmetre

- 4 canaux: AIN1, AIN2, AIN3, I
- Rafraichissement 100ms

### Mode Logic

- 3 canaux: DIN1, DIN2, DIN3
- Etats + compteur transitions

### Mode Stim

- DOUT1/DOUT2 controle manuel
- Toggle, pulse, PWM

---

## CHECKLIST MONTAGE

### Facade

- [ ] Decoupe ecran
- [ ] Percage DB-15 (ouverture standard D-sub)
- [ ] Percage USB-C + vis M3
- [ ] Percage bouton BOOT

### Interne

- [ ] ESP32-C6 Touch LCD monte
- [ ] Cable USB-C panel mount connecte
- [ ] Module DB-15 breakout board fixe en facade
- [ ] Borniers DB-15 cables vers stripboard:
  - [ ] Bornier 1 -> F1 fusible
  - [ ] Bornier 3, 14, GND -> GND commun
  - [ ] Bornier 4 -> R1 (15k)
  - [ ] Bornier 5 -> R4 (10k)
  - [ ] Bornier 6 -> R7 (1k)
  - [ ] Bornier 7 -> R8 (1k)
  - [ ] Bornier 8 -> R9 (1k)
  - [ ] Bornier 9 -> R10 (1k)
  - [ ] Bornier 10 <- R11 (1k)
  - [ ] Bornier 11 <- R12 (1k)
  - [ ] Bornier 12 -> INA219 VIN+
  - [ ] Bornier 13 -> INA219 VIN-
- [ ] Diviseurs R1/R2, R4/R5 soudes
- [ ] Diodes clamp D1/D2 installees
- [ ] INA219 sur I2C
- [ ] Fusible 500mA installe

### Test

- [ ] Alim USB-C OK
- [ ] Ecran + tactile OK
- [ ] I2C: 0x15 + 0x40
- [ ] Brancher cable VALVE
- [ ] Lectures coherentes

---

**--- FIN BREAKOUT BOX V1.8 ---**
