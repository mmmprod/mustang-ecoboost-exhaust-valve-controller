# BREAKOUT BOX V1.4 - Circuit

## Hardware: Waveshare ESP32-C6-LCD-1.47 + INA219
## Target: Circuit VALVE v4.5
## Date: Decembre 2025

---

## CHANGELOG V1.4

**CORRECTION CRITIQUE:**
- Diviseur 12V: 10k/3.3k -> **15k/3.3k** (ratio 5.545)
- Protection clamp: **BAT54S** vers +3.3V sur canaux 12V
- R serie: **1k** entre diviseur et GPIO (tous canaux)
- Filtre HF: **100nF** sur chaque GPIO ADC

**Calcul securite V1.4:**
```
V_in = 16.0V (alternateur max) -> V_gpio = 2.88V < 3.0V OK
V_in = 29.0V (TVS clamp)       -> V_gpio = 5.23V -> clampe 3.6V par BAT54S
```

**CONSERVE V1.3:**
- Q1/Q2: NPN open-collector pour STIM1/STIM2
- R23/D4: Protection INA219 (Zener 24V)
- Mode STRESS TEST, Min/Max Hold, Calibration ADC

---

## BLOC A - ESP32-C6-LCD-1.47

### Alimentation
- USB-C: 5V via cable (recommande)
- Ou: VIN (5V) depuis alimentation externe

### Affectation GPIO

| GPIO | Fonction | Direction | Signal |
|------|----------|-----------|--------|
| 1 | ADC | IN | +12V_PROT (div 15k/3.3k + clamp) |
| 2 | ADC | IN | +5V_LOG (div 10k/10k) |
| 3 | ADC | IN | AUTO_REQ (div 10k/10k) |
| 4 | ADC | IN | OUT_DRIVER (div 15k/3.3k + clamp) |
| 5 | ADC | IN | CMD (div 10k/10k) |
| 6 | I2C SDA | I/O | INA219 |
| 7 | I2C SCL | OUT | INA219 |
| 8 | ADC | IN | MANUAL_REQ (div 10k/10k) |
| 10 | STIM1 | OUT | -> Q1 base (NPN) |
| 11 | STIM2 | OUT | -> Q2 base (NPN) |
| 14 | SPI CS | OUT | ST7789 (ecran) |
| 15 | TFT DC | OUT | ST7789 |
| 18 | SPI MOSI | OUT | ST7789 |
| 19 | SPI SCK | OUT | ST7789 |
| 22 | TFT BL | OUT | Backlight |

### Boutons
- GPIO0 (BOOT) = BTN1: Menu/Select
- GPIO21 = BTN2: Reset stats (optionnel)

---

## BLOC B - Diviseurs ADC [CORRIGE V1.4]

### B1. Diviseur 15V PROTEGE (12V_PROT, OUT_DRIVER)

```
                         +3.3V (ESP32)
                           |
                        BAT54S cathode
                           |
Signal_IN -> R_H 15k -> Node -> R_S 1k -> GPIO_ADC
                         |                    |
                       R_L 3.3k            100nF
                         |                    |
                        GND                  GND
```

**Composants par canal (x2):**
- R_H: 15k 1% 1/4W (diviseur haut)
- R_L: 3.3k 1% 1/4W (diviseur bas)
- R_S: 1k 1% 1/4W (serie protection)
- C_F: 100nF X7R 16V (filtre HF)
- D_clamp: BAT54S double Schottky (partage entre 2 canaux)

**Calculs:**
```
Ratio = R_L / (R_H + R_L) = 3.3 / 18.3 = 0.180
Ratio inverse (firmware) = 18.3 / 3.3 = 5.545

V_gpio @ 12.0V = 12.0 * 0.180 = 2.16V (nominal batterie)
V_gpio @ 14.4V = 14.4 * 0.180 = 2.60V (alternateur nominal)
V_gpio @ 16.0V = 16.0 * 0.180 = 2.88V (alternateur max)
V_gpio @ 29.0V = 29.0 * 0.180 = 5.23V -> CLAMPE 3.6V par BAT54S
```

**Protection Load Dump:**
```
Si V_in = 29V (TVS 5KP18CA clamp):
- V_diviseur = 5.23V
- BAT54S conduit des que V > 3.3V + 0.3V = 3.6V
- I_clamp = (5.23V - 3.6V) / 1k = 1.63mA
- BAT54S rating: 200mA -> Marge 99%
- Diode interne ESP32: 10mA max -> courant via Schottky, pas ESP32
```

### B2. Diviseur 6V (5V_LOG, AUTO_REQ, CMD, MANUAL_REQ)

```
Signal_IN -> R_H 10k -> Node -> R_S 1k -> GPIO_ADC
                         |                    |
                       R_L 10k             100nF
                         |                    |
                        GND                  GND
```

**Composants par canal (x4):**
- R_H: 10k 1% 1/4W
- R_L: 10k 1% 1/4W
- R_S: 1k 1% 1/4W (serie protection)
- C_F: 100nF X7R 16V (filtre HF)

**Note:** Pas de clamp BAT54S necessaire (5V max, ratio /2 = 2.5V < 3.0V)

**Calculs:**
```
Ratio = 10k / (10k + 10k) = 0.5
Ratio inverse (firmware) = 2.0

V_gpio @ 5.0V = 5.0 * 0.5 = 2.5V (nominal)
V_gpio @ 5.5V = 5.5 * 0.5 = 2.75V (surtension)
```

---

## BLOC C - Stimulation NPN [INCHANGE V1.3]

### C1. STIM1 (Simulation pressostat)

```
GPIO10 -> R21 (1k) -> Base Q1 (2N2222)
                           |
                        Emetteur -> GND_BB
                           |
                       Collecteur -> STIM1 (vers IN_A carte VALVE)
```

**Logique:**
- GPIO10 = LOW: Q1 bloque, IN_A = 5V (pull-up carte) = Pressostat OFF
- GPIO10 = HIGH: Q1 sature, IN_A = 0.2V = Pressostat ON (simule boost)

**Composants:**
- Q1: 2N2222A ou BC547 (NPN)
- R21: 1k 1/4W

### C2. STIM2 (Simulation bouton toggle)

```
GPIO11 -> R22 (1k) -> Base Q2 (2N2222)
                           |
                        Emetteur -> GND_BB
                           |
                       Collecteur -> STIM2 (vers CLK CD4013)
```

**Composants:**
- Q2: 2N2222A ou BC547 (NPN)
- R22: 1k 1/4W

---

## BLOC D - INA219 Current Sensor [INCHANGE V1.3]

### D1. Module INA219

- Adresse I2C: 0x40 (default)
- Shunt: 0.1 Ohm (sur module)
- Range: 0-1A, resolution 0.1mA

### D2. Connexions

```
INA219 VCC -> 3.3V ESP32
INA219 GND -> GND_BB
INA219 SDA -> GPIO6
INA219 SCL -> GPIO7
```

### D3. Protection entree

```
I_SENSE+ (J-BB pin 10) -> R23 (1k) -> INA219 VIN+
                                  |
                              D4 cathode (1N4749A 24V)
                                  |
                              D4 anode -> GND_BB
```

**Composants:**
- R23: 1k 1/4W
- D4: 1N4749A (Zener 24V 1W) DO-41

### D4. Connexion shunt

```
I_SENSE+ (apres R23) -> INA219 VIN+
I_SENSE- (J-BB pin 11) -> INA219 VIN-
```

---

## BLOC E - Connecteur Breakout Box

### Header 2x 1x6 pins (vers carte VALVE J-BB)

**Rangee A (Mesures):**

| Pin | Signal | Diviseur | Clamp | GPIO |
|-----|--------|----------|-------|------|
| 1 | +12V_PROT | 15k/3.3k | BAT54S | GPIO1 |
| 2 | +5V_LOG | 10k/10k | - | GPIO2 |
| 3 | AUTO_REQ | 10k/10k | - | GPIO3 |
| 4 | OUT_DRIVER | 15k/3.3k | BAT54S | GPIO4 |
| 5 | CMD | 10k/10k | - | GPIO5 |
| 6 | MANUAL_REQ | 10k/10k | - | GPIO8 |

**Rangee B (Commandes + Courant):**

| Pin | Signal | Connexion |
|-----|--------|-----------|
| 7 | GND | GND_BB |
| 8 | STIM_1 | Collecteur Q1 |
| 9 | STIM_2 | Collecteur Q2 |
| 10 | I_SENSE+ | R23 -> INA219 VIN+ |
| 11 | I_SENSE- | INA219 VIN- |
| 12 | GND | GND_BB |

---

## BLOC F - Alimentation Breakout Box

### USB-C (recommande)
- Brancher cable USB-C sur ESP32-C6-LCD
- Alimente ESP32 + INA219
- Isolation galvanique vs vehicule

---

## BOM Breakout Box V1.4

### Semiconducteurs

| Qty | Composant | Valeur | Package | Ref |
|-----|-----------|--------|---------|-----|
| 1 | ESP32-C6-LCD-1.47 | Waveshare | Module | U1 |
| 1 | INA219 | Module I2C | Breakout | U2 |
| 2 | 2N2222A | NPN | TO-92 | Q1, Q2 |
| 1 | 1N4749A | Zener 24V 1W | DO-41 | D4 |
| 1 | BAT54S | Schottky double | SOT-23 | D5 |

### Resistances

| Qty | Valeur | Tolerance | Package | Usage |
|-----|--------|-----------|---------|-------|
| 2 | 15k | 1% | Axial/0603 | Div 12V R_H |
| 2 | 3.3k | 1% | Axial/0603 | Div 12V R_L |
| 4 | 10k | 1% | Axial/0603 | Div 5V |
| 8 | 1k | 1% | Axial/0603 | R serie + STIM + INA |

### Condensateurs

| Qty | Valeur | Type | Tension | Usage |
|-----|--------|------|---------|-------|
| 6 | 100nF | X7R | 16V | Filtre ADC |

### Connecteurs

| Qty | Type | Ref |
|-----|------|-----|
| 2 | Header femelle 1x6 2.54mm | J-BB |
| 1 | Nappe 12 fils 50cm | Cable |

### Cout total V1.4

- ESP32-C6-LCD-1.47: ~18 EUR
- INA219 module: ~3 EUR
- BAT54S: ~0.15 EUR
- Composants discrets: ~2.50 EUR
- **Total: ~24 EUR**

---

## SCHEMA IMPLANTATION V1.4

```
┌─────────────────────────────────────────────────────────────────┐
│                    BREAKOUT BOX V1.4                            │
│                 ISO 7637 ADC Protection                         │
│                                                                 │
│  ┌──────────────────┐                                           │
│  │  ESP32-C6-LCD    │                                           │
│  │  ┌───────────┐   │      ┌─────────┐                          │
│  │  │  ST7789   │   │      │ INA219  │                          │
│  │  │  Display  │   │      │ Module  │                          │
│  │  │           │   │      └────┬────┘                          │
│  │  └───────────┘   │           │                               │
│  │                  │      SDA──┘──SCL                          │
│  │  [USB-C]         │           │                               │
│  └──────────────────┘           │                               │
│                                 │                               │
│  ┌────────────────────────────────────────────────────────────┐ │
│  │           ZONE PROTECTION ADC V1.4 [NOUVEAU]               │ │
│  │                                                            │ │
│  │                              +3.3V                         │ │
│  │                                │                           │ │
│  │                            BAT54S (D5)                     │ │
│  │                           /         \                      │ │
│  │   +12V_PROT ─15k─┬─1k─┬─●           ●─┬─1k─┬─15k─ OUT_DRV │ │
│  │                  │    │               │    │              │ │
│  │                3.3k  100n           100n  3.3k            │ │
│  │                  │    │               │    │              │ │
│  │                 GND  GND             GND  GND             │ │
│  │                       │               │                   │ │
│  │                    GPIO1           GPIO4                  │ │
│  │                                                            │ │
│  │   Canaux 5V (sans clamp, V_max = 2.75V):                  │ │
│  │                                                            │ │
│  │   +5V_LOG ──10k──┬──1k──┬── GPIO2                         │ │
│  │                  │      │                                 │ │
│  │                 10k   100n                                │ │
│  │                  │      │                                 │ │
│  │                 GND    GND                                │ │
│  │                                                            │ │
│  │   (Idem pour AUTO_REQ->GPIO3, CMD->GPIO5, MAN->GPIO8)     │ │
│  └────────────────────────────────────────────────────────────┘ │
│                                                                 │
│  ┌────────────────────────────────────────────────────────────┐ │
│  │              ZONE NPN STIM [V1.3]                          │ │
│  │                                                            │ │
│  │   GPIO10 ─ 1k ─┬─ Q1 base      GPIO11 ─ 1k ─┬─ Q2 base    │ │
│  │                │  2N2222                     │  2N2222     │ │
│  │              E─┴─GND                       E─┴─GND         │ │
│  │              C───STIM1                     C───STIM2       │ │
│  └────────────────────────────────────────────────────────────┘ │
│                                                                 │
│  ┌────────────────────────────────────────────────────────────┐ │
│  │              ZONE INA219 PROTECTION [V1.3]                 │ │
│  │                                                            │ │
│  │   I_SENSE+ ─── 1k ───┬─── INA219 VIN+                     │ │
│  │                      │                                     │ │
│  │                  1N4749A (24V)                             │ │
│  │                      │                                     │ │
│  │                     GND                                    │ │
│  └────────────────────────────────────────────────────────────┘ │
│                                                                 │
│  ═══════════════════════════════════════════════════════════════│
│           CONNECTEUR J-BB (vers carte VALVE)                    │
│  ┌─────┬─────┬─────┬─────┬─────┬─────┐                         │
│  │  1  │  2  │  3  │  4  │  5  │  6  │ Rangee A               │
│  │ 12V │ 5V  │AUTO │ OUT │ CMD │ MAN │                         │
│  └─────┴─────┴─────┴─────┴─────┴─────┘                         │
│  ┌─────┬─────┬─────┬─────┬─────┬─────┐                         │
│  │  7  │  8  │  9  │ 10  │ 11  │ 12  │ Rangee B               │
│  │ GND │STM1 │STM2 │ I+  │ I-  │ GND │                         │
│  └─────┴─────┴─────┴─────┴─────┴─────┘                         │
└─────────────────────────────────────────────────────────────────┘
```

---

## PROCEDURE ASSEMBLAGE V1.4

### 1. Preparer ESP32-C6-LCD
- Souder headers si necessaire
- Verifier ecran fonctionne (upload exemple TFT_eSPI)

### 2. Assembler zone diviseurs ADC [NOUVEAU V1.4]

**Canaux 12V (2x):**
- Souder 15k (R_H) + 3.3k (R_L) en diviseur
- Souder 1k (R_S) en serie vers GPIO
- Souder 100nF entre GPIO et GND
- Souder BAT54S: anode commune au noeud diviseur, cathode vers +3.3V

**Canaux 5V (4x):**
- Souder 10k + 10k en diviseur
- Souder 1k en serie vers GPIO
- Souder 100nF entre GPIO et GND

### 3. Assembler zone NPN [V1.3]
- Souder Q1, Q2 (2N2222) avec R21, R22 (1k)
- Connecter emetteurs au GND

### 4. Assembler protection INA219 [V1.3]
- Souder R23 (1k) + D4 (1N4749A)
- Connecter INA219 VIN+ apres R23

### 5. Connecter INA219
- VCC -> 3.3V ESP32
- GND -> GND commun
- SDA -> GPIO6
- SCL -> GPIO7

### 6. Cabler connecteur J-BB
- 2x headers femelle 1x6
- Nappe 12 fils vers circuits

### 7. Upload firmware
- Board: ESP32C6 Dev Module
- Upload: BreakoutBox_V1.4.ino

---

## TESTS VALIDATION V1.4

### Test 1: Protection clamp BAT54S

**SANS carte VALVE connectee:**
1. Appliquer 20V sur pin 1 (12V_PROT) avec alim labo, limite 10mA
2. Mesurer tension sur GPIO1: doit etre < 3.6V
3. Si > 3.6V: BAT54S mal soudee ou absente

### Test 2: Diviseur ratio

1. Appliquer 12.00V sur pin 1 (12V_PROT)
2. Mesurer sur GPIO1: attendu 2.16V +/- 2%
3. Si ecart > 5%: resistances mal soudees

### Test 3: Load Dump simule

1. Appliquer 30V sur pin 1 (12V_PROT), limite 5mA
2. Mesurer sur GPIO1: doit etre clampe ~3.5-3.6V
3. Mesurer courant: doit etre ~1-2mA (via BAT54S)
4. ESP32 doit survivre sans reset

### Test 4: Firmware

1. Brancher USB-C
2. Ecran affiche "BREAKOUT BOX V1.4"
3. Mode LIVE: tensions coherentes

---

## TABLEAU COMPARATIF V1.3 vs V1.4

| Parametre | V1.3 (FAUX) | V1.4 (CORRIGE) |
|-----------|-------------|----------------|
| Diviseur 12V | 10k/3.3k | **15k/3.3k** |
| Ratio | 4.03 | **5.545** |
| V_gpio @ 14.4V | 3.57V DANGER | 2.60V OK |
| V_gpio @ 16.0V | 3.97V DESTRUCTION | 2.88V OK |
| V_gpio @ 29.0V | 7.19V MORT | 3.6V clampe |
| Protection clamp | Aucune | **BAT54S** |
| R serie GPIO | Aucune | **1k** |
| Filtre HF | Aucun | **100nF** |

---

## NOTES SECURITE V1.4

1. **BAT54S OBLIGATOIRE** sur canaux 12V - sans elle, Load Dump = ESP32 mort

2. **R serie 1k OBLIGATOIRE** - limite courant clamp a ~2mA max

3. **Ne jamais connecter sans BAT54S** meme pour "test rapide"

4. **Orientation BAT54S:** Anodes (centre) vers noeud diviseur, cathodes vers +3.3V

5. **Test clamp AVANT utilisation:** Appliquer 25V, verifier GPIO < 4V

---

**FIN DOCUMENT BREAKOUT BOX V1.4**
