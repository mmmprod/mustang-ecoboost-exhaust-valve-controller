# BREAKOUT BOX V1.3 - Circuit

## Hardware: Waveshare ESP32-C6-LCD-1.47 + INA219
## Target: Circuit VALVE v4.5
## Date: Decembre 2025

---

## CHANGELOG V1.3

**CORRECTIONS CRITIQUES:**
- Q1/Q2: NPN open-collector pour STIM1/STIM2 (protection 5V -> 3.3V GPIO)
- R23/D4: Protection INA219 contre surtensions (Zener 24V)

**AJOUTS FIRMWARE:**
- Mode STRESS TEST (10Hz, 2 secondes)
- Mode LIVE avec Min/Max Hold
- Calibration ADC automatique

---

## BLOC A - ESP32-C6-LCD-1.47

### Alimentation
- USB-C: 5V via cable
- Ou: VIN (5V) depuis alimentation externe

### Affectation GPIO

| GPIO | Fonction | Direction | Signal |
|------|----------|-----------|--------|
| 0 | ADC | IN | +12V_PROT (div 10k/3.3k) |
| 1 | ADC | IN | +5V_LOG (div 10k/10k) |
| 2 | ADC | IN | AUTO_REQ (div 10k/10k) |
| 3 | ADC | IN | OUT_DRIVER (div 10k/3.3k) |
| 4 | I2C SDA | I/O | INA219 |
| 5 | I2C SCL | OUT | INA219 |
| 6 | SPI MOSI | OUT | ST7789 (ecran) |
| 7 | SPI SCK | OUT | ST7789 |
| 8 | STIM1 | OUT | -> Q1 base (NPN) |
| 9 | STIM2 | OUT | -> Q2 base (NPN) |
| 14 | SPI CS | OUT | ST7789 |
| 15 | TFT DC | OUT | ST7789 |
| 21 | BTN2 | IN | Bouton optionnel |
| 22 | TFT BL | OUT | Backlight |

### Bouton integre
- GPIO0 (BOOT) = BTN1: Menu/Select

---

## BLOC B - Diviseurs ADC

### B1. Diviseur 15V (12V_PROT, OUT_DRIVER)

```
Signal_IN -> R_H (10k) -> ADC_PIN
                    |
                R_L (3.3k)
                    |
                   GND
```

- Ratio: (10k + 3.3k) / 3.3k = 4.03
- Range: 0-13.3V -> 0-3.3V
- V_max mesurable: 13.3V (15V avec marge)

**Composants par canal:**
- R_H: 10k 1% 1/4W
- R_L: 3.3k 1% 1/4W

### B2. Diviseur 6V (5V_LOG, AUTO_REQ, CMD, MANUAL_REQ)

```
Signal_IN -> R_H (10k) -> ADC_PIN
                    |
                R_L (10k)
                    |
                   GND
```

- Ratio: (10k + 10k) / 10k = 2.0
- Range: 0-6.6V -> 0-3.3V

**Composants par canal:**
- R_H: 10k 1% 1/4W
- R_L: 10k 1% 1/4W

---

## BLOC C - Stimulation NPN [NOUVEAU V1.3]

### C1. STIM1 (Simulation pressostat)

**Probleme V1.2:** GPIO8 voyait 5V via pull-up 10k carte VALVE = hors specs ESP32

**Solution:** NPN open-collector imite comportement pressostat NPN

```
GPIO8 -> R21 (1k) -> Base Q1 (2N2222)
                          |
                       Emetteur -> GND_BB
                          |
                      Collecteur -> STIM1 (vers IN_A carte VALVE)
```

**Logique:**
- GPIO8 = LOW: Q1 bloque, IN_A = 5V (pull-up carte) = Pressostat OFF
- GPIO8 = HIGH: Q1 sature, IN_A = 0.2V = Pressostat ON (simule boost)

**Composants:**
- Q1: 2N2222A ou BC547 (NPN)
- R21: 1k 1/4W

**Calcul:**
- I_base = (3.3V - 0.7V) / 1k = 2.6mA
- Beta_min = 100 -> I_c_max = 260mA >> besoin
- V_ce_sat @ 2.6mA < 0.2V OK

### C2. STIM2 (Simulation bouton toggle)

**Meme topologie:**

```
GPIO9 -> R22 (1k) -> Base Q2 (2N2222)
                          |
                       Emetteur -> GND_BB
                          |
                      Collecteur -> STIM2 (vers CLK CD4013)
```

**Composants:**
- Q2: 2N2222A ou BC547 (NPN)
- R22: 1k 1/4W

**Logique:**
- GPIO9 pulse HIGH: Q2 sature, CLK tire vers GND
- GPIO9 retourne LOW: Q2 bloque, CLK remonte via pull-up = front montant = toggle

---

## BLOC D - INA219 Current Sensor [MODIFIE V1.3]

### D1. Module INA219

- Adresse I2C: 0x40 (default)
- Shunt: 0.1 Ohm (sur module)
- Range: 0-1A, resolution 0.1mA

### D2. Connexions

```
INA219 VCC -> 3.3V ESP32
INA219 GND -> GND_BB
INA219 SDA -> GPIO4
INA219 SCL -> GPIO5
```

### D3. Protection entree [NOUVEAU V1.3]

**Probleme:** I_SENSE+ voit +12V_IN avant protections = Load dump 60V+ = INA219 mort

**Solution:** R serie + Zener clamp

```
I_SENSE+ (J-BB pin 10) -> R23 (1k) -> INA219 VIN+
                                  |
                              D4 cathode (1N4749A 24V)
                                  |
                              D4 anode -> GND_BB
```

**Protection:**
- Normal (12V): Zener ne conduit pas, mesure OK
- Load dump (60V): I_zener = (60-24)/1k = 36mA, P = 0.86W < 1W
- Pulse negatif (-150V): D4 conduit en direct, V = -0.7V

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

| Pin | Signal | Diviseur | GPIO |
|-----|--------|----------|------|
| 1 | +12V_PROT | 10k/3.3k | GPIO0 |
| 2 | +5V_LOG | 10k/10k | GPIO1 |
| 3 | AUTO_REQ | 10k/10k | GPIO2 |
| 4 | OUT_DRIVER | 10k/3.3k | GPIO3 |
| 5 | CMD | 10k/10k | GPIO4* |
| 6 | MANUAL_REQ | 10k/10k | GPIO5* |

*GPIO4/5 partages avec I2C - utiliser multiplexage SW ou GPIO alternatifs

**Rangee B (Commandes + Courant):**

| Pin | Signal | Connexion |
|-----|--------|-----------|
| 7 | GND | GND_BB |
| 8 | STIM_1 | Collecteur Q1 |
| 9 | STIM_2 | Collecteur Q2 |
| 10 | I_SENSE+ | R23 -> INA219 VIN+ |
| 11 | I_SENSE- | INA219 VIN- |
| 12 | GND | GND_BB |

### Cablage nappe

- 12 fils Dupont femelle ou nappe AWG28
- Longueur: 30-50cm
- Pas de blindage requis (signaux lents)

---

## BLOC F - Alimentation Breakout Box

### Option 1: USB-C (recommande)

- Brancher cable USB-C sur ESP32-C6-LCD
- Alimente ESP32 + INA219
- Pas d'alimentation depuis carte VALVE

### Option 2: Depuis carte VALVE

```
J-BB pin 2 (+5V_LOG) -> Diode 1N4148 -> VIN ESP32 (optionnel)
```

**Attention:** Ne pas utiliser si USB connecte (double alimentation)

---

## BOM Breakout Box V1.3

### Semiconducteurs

| Qty | Composant | Valeur | Package | Ref |
|-----|-----------|--------|---------|-----|
| 1 | ESP32-C6-LCD-1.47 | Waveshare | Module | U1 |
| 1 | INA219 | Module I2C | Breakout | U2 |
| 2 | 2N2222A | NPN | TO-92 | Q1, Q2 |
| 1 | 1N4749A | Zener 24V 1W | DO-41 | D4 |

### Resistances

| Qty | Valeur | Tolerance | Package | Ref |
|-----|--------|-----------|---------|-----|
| 5 | 10k | 1% | 0603/Axial | Diviseurs |
| 2 | 3.3k | 1% | 0603/Axial | Div 15V |
| 3 | 1k | 5% | 0603/Axial | R21, R22, R23 |

### Connecteurs

| Qty | Type | Ref |
|-----|------|-----|
| 2 | Header femelle 1x6 2.54mm | J-BB |
| 1 | Nappe 12 fils 50cm | Cable |

### Cout total

- ESP32-C6-LCD-1.47: ~18 EUR
- INA219 module: ~3 EUR
- Composants discrets: ~2 EUR
- **Total: ~23 EUR**

---

## SCHEMA IMPLANTATION

```
┌─────────────────────────────────────────────────────────┐
│                    BREAKOUT BOX V1.3                    │
│                                                         │
│  ┌──────────────────┐                                   │
│  │  ESP32-C6-LCD    │                                   │
│  │  ┌───────────┐   │                                   │
│  │  │  ST7789   │   │      ┌─────────┐                  │
│  │  │  Display  │   │      │ INA219  │                  │
│  │  │           │   │      │ Module  │                  │
│  │  └───────────┘   │      └────┬────┘                  │
│  │                  │           │                       │
│  │  [USB-C]         │      SDA──┘──SCL                  │
│  └──────────────────┘           │                       │
│                                 │                       │
│  ┌────────────────────────────────────────────────────┐ │
│  │              ZONE PROTECTIONS V1.3                 │ │
│  │                                                    │ │
│  │   ┌─────┐ R21    ┌─────┐ R22                       │ │
│  │   │ Q1  │ 1k     │ Q2  │ 1k      R23    D4         │ │
│  │   │2N222│◄───    │2N222│◄───     1k   1N4749       │ │
│  │   └──┬──┘GPIO8   └──┬──┘GPIO9    ─┬─   ─┬─24V      │ │
│  │      │               │            │     │          │ │
│  │      ▼               ▼            │     │          │ │
│  │   STIM1           STIM2       I_SENSE+  │          │ │
│  │                                   │     │          │ │
│  │                               INA219────┘          │ │
│  └────────────────────────────────────────────────────┘ │
│                                                         │
│  ┌─────────────────────────────────────────────────────┐│
│  │ Diviseurs ADC (sur stripboard)                      ││
│  │ CH0: 10k/3.3k  CH1: 10k/10k  CH2: 10k/10k          ││
│  │ CH3: 10k/3.3k  (CH4/CH5 si GPIO dispo)             ││
│  └─────────────────────────────────────────────────────┘│
│                                                         │
│  ═══════════════════════════════════════════════════════│
│           CONNECTEUR J-BB (vers carte VALVE)            │
│  ┌─────┬─────┬─────┬─────┬─────┬─────┐                 │
│  │  1  │  2  │  3  │  4  │  5  │  6  │ Rangee A       │
│  │ 12V │ 5V  │AUTO │ OUT │ CMD │ MAN │                 │
│  └─────┴─────┴─────┴─────┴─────┴─────┘                 │
│  ┌─────┬─────┬─────┬─────┬─────┬─────┐                 │
│  │  7  │  8  │  9  │ 10  │ 11  │ 12  │ Rangee B       │
│  │ GND │STM1 │STM2 │ I+  │ I-  │ GND │                 │
│  └─────┴─────┴─────┴─────┴─────┴─────┘                 │
└─────────────────────────────────────────────────────────┘
```

---

## PROCEDURE ASSEMBLAGE

### 1. Preparer ESP32-C6-LCD
- Souder headers si necessaire
- Verifier ecran fonctionne (upload exemple TFT_eSPI)

### 2. Assembler zone protections (sur stripboard ou proto)
- Souder Q1, Q2 (2N2222) avec R21, R22 (1k)
- Souder R23 (1k) + D4 (1N4749A) pour INA219
- Connecter emetteurs Q1/Q2 au GND

### 3. Assembler diviseurs ADC
- 2x diviseurs 10k/3.3k (canaux 12V)
- 4x diviseurs 10k/10k (canaux 5V)
- GND commun

### 4. Connecter INA219
- VCC -> 3.3V ESP32
- GND -> GND commun
- SDA -> GPIO4
- SCL -> GPIO5
- VIN+ <- R23 (I_SENSE+)
- VIN- <- I_SENSE-

### 5. Cabler connecteur J-BB
- 2x headers femelle 1x6
- Nappe 12 fils vers stripboard/diviseurs

### 6. Upload firmware
- Voir instructions Arduino IDE (User_Setup_C6_LCD.h requis)
- Board: ESP32C6 Dev Module
- Upload: BreakoutBox_V1.3.ino

---

## TESTS VALIDATION

### Test 1: Alimentation
- Brancher USB-C
- Ecran doit afficher splash "BREAKOUT BOX V1.3"

### Test 2: INA219
- Menu -> Test Current
- Doit afficher courant (~0mA sans charge)

### Test 3: NPN STIM
- Multimetre sur STIM1 (collecteur Q1) vs GND
- Menu -> Stim AUTO
- Doit voir toggle 0V <-> 5V

### Test 4: Protection Zener
- SANS carte VALVE connectee
- Appliquer 30V sur I_SENSE+ (alim labo, limite 10mA)
- Mesurer tension sur INA219 VIN+: doit etre ~24V (clampe)

---

## NOTES SECURITE

1. **Ne jamais connecter Breakout Box pendant demarrage moteur** (cold-crank peut endommager)

2. **Toujours utiliser USB pour alimentation** (isolation galvanique)

3. **Mode STRESS TEST fait claquer la valve 20 fois** - bruit normal

4. **Protection INA219 limite a ~60V** - au-dela, destruction possible

---

**FIN DOCUMENT BREAKOUT BOX V1.3**
