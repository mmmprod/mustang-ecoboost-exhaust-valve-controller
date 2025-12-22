# BREAKOUT BOX V1.2 - ESP32-C6-LCD-1.47

**Version:** 1.2  
**Date:** Decembre 2025  
**Module:** Waveshare ESP32-C6-LCD-1.47

---

## CHANGELOG V1.2

- Connecteur 2x headers 1x6 pins (compatible stripboard)
- Ajout lecture AUTO_REQ, CMD, MANUAL_REQ pour diagnostic complet
- Test verifie CHAQUE etage de la chaine de commande

---

## CONNECTEUR STRIPBOARD

**Probleme header 2x6:** Pistes horizontales court-circuitent les pins face a face.

**Solution:** 2x headers males 1x6 pins, espaces de 3 rangees (7.62mm)

```
STRIPBOARD (vue dessus, pistes horizontales)
═══════════════════════════════════════════════════════════════

Rangee A                    3 rangees           Rangee B
(mesures)                    vides              (commandes)
    ║                          ║                    ║
    ▼                          ▼                    ▼
┌───────┐                                      ┌───────┐
│ 1 │ o │════════════X═══════════════════════│ o │ 7 │  <- couper piste
│ 2 │ o │════════════X═══════════════════════│ o │ 8 │  <- couper piste
│ 3 │ o │════════════X═══════════════════════│ o │ 9 │  <- couper piste
│ 4 │ o │════════════X═══════════════════════│ o │10 │  <- couper piste
│ 5 │ o │════════════X═══════════════════════│ o │11 │  <- couper piste
│ 6 │ o │════════════X═══════════════════════│ o │12 │  <- couper piste
└───────┘                                      └───────┘

X = couper la piste entre les deux headers (foret 3mm)
```

---

## BROCHAGE CONNECTEUR

### Rangee A - Mesures (6 pins)

| Pin | Signal | ADC | Diviseur | Plage |
|-----|--------|-----|----------|-------|
| 1 | +12V_PROT | GPIO0 | 10k/3.3k | 0-15V |
| 2 | +5V_LOG | GPIO1 | 10k/10k | 0-6V |
| 3 | AUTO_REQ | GPIO2 | 10k/10k | 0-5V |
| 4 | OUT_DRIVER | GPIO3 | 10k/3.3k | 0-15V |
| 5 | CMD | GPIO4 | 10k/10k | 0-5V |
| 6 | MANUAL_REQ | GPIO5 | 10k/10k | 0-5V |

### Rangee B - Commandes/Alim (6 pins)

| Pin | Signal | GPIO | Type |
|-----|--------|------|------|
| 7 | GND | - | Masse |
| 8 | STIM_1 | GPIO8 | OC via 2N7000 |
| 9 | STIM_2 | GPIO9 | OC via 2N7000 |
| 10 | I_SENSE+ | INA219 VIN+ | Mesure I |
| 11 | I_SENSE- | INA219 VIN- | Mesure I |
| 12 | GND | - | Masse |

---

## CABLAGE VALVE v4.3

| Pin BB | Signal | Connexion sur carte VALVE |
|--------|--------|---------------------------|
| 1 | +12V_PROT | Cathode 1N5822 (apres diode protection) |
| 2 | +5V_LOG | 78L05 pin 1 (Vout) |
| 3 | AUTO_REQ | 74HC14 pin 2 (sortie buffer A) |
| 4 | OUT_DRIVER | BTS5090 pins 6-7-8 (OUT) |
| 5 | CMD | Noeud [CMD] (jonction diodes OR) |
| 6 | MANUAL_REQ | CD4013 pin 1 (Q) |
| 7 | GND | [GND_STAR] |
| 8 | STIM_1 | 74HC14 pin 1 (IN_A) apres R 1k |
| 9 | STIM_2 | CD4013 pin 3 (CLK) avant R 10k |
| 10 | I_SENSE+ | Sortie fusible F1 (+12V_IN fuse) |
| 11 | I_SENSE- | Entree ferrite FB1 |
| 12 | GND | [GND_STAR] |

---

## SCHEMA DIVISEURS ADC

### Diviseur 15V (pins 1, 4)

```
Signal (0-15V) ─── R_haut (10k) ───┬─── GPIO (ADC)
                                   │
                                   ├─── R_bas (3.3k) ─── GND
                                   │
                                   ├─── Zener 3.3V ─── GND
                                   │
                                   └─── C (100nF) ─── GND

Ratio = (10k + 3.3k) / 3.3k = 4.03
15V entree = 3.72V sur GPIO (protege par Zener)
```

### Diviseur 6V (pins 2, 3, 5, 6)

```
Signal (0-5V) ─── R_haut (10k) ───┬─── GPIO (ADC)
                                  │
                                  ├─── R_bas (10k) ─── GND
                                  │
                                  ├─── Zener 3.3V ─── GND
                                  │
                                  └─── C (100nF) ─── GND

Ratio = 2.0
5V entree = 2.5V sur GPIO (dans plage ADC)
```

---

## STIMULATION OPEN-COLLECTOR

```
GPIOx ─── R (1k) ───┬─── Gate 2N7000
                    │
                    └─── R (100k) ─── GND (pull-down)

                         2N7000
                         ┌─────┐
                  Gate ──┤     ├── Drain ─── vers circuit cible
                         │     │
                  GND  ──┤     ├── Source ─── GND
                         └─────┘
```

Fonctionnement:
- GPIO HIGH = 2N7000 ON = Drain tire vers GND = signal LOW sur cible
- GPIO LOW = 2N7000 OFF = Drain flottant = cible voit son pull-up

---

## BOM V1.2

### Modules
| Qty | Composant | Reference | Prix |
|-----|-----------|-----------|------|
| 1 | ESP32-C6-LCD-1.47 | Waveshare | ~18€ |
| 1 | INA219 module | GY-INA219 | ~3€ |

### Connecteurs (pour stripboard)
| Qty | Composant | Reference |
|-----|-----------|-----------|
| 2 | Header male 1x6 pins | 2.54mm |
| 1 | Cable 12 fils 20cm | AWG28 ou Dupont |

### Resistances 1/4W 1%
| Qty | Valeur | Usage |
|-----|--------|-------|
| 4 | 10k | Diviseurs haut (tous) |
| 2 | 3.3k | Diviseurs 15V bas |
| 4 | 10k | Diviseurs 6V bas |
| 2 | 1k | Gate 2N7000 |
| 2 | 100k | Pull-down 2N7000 |

### Autres
| Qty | Composant |
|-----|-----------|
| 6 | Condensateur 100nF ceramique |
| 6 | Zener 3.3V BZX79C3V3 |
| 2 | 2N7000 TO-92 |

---

## INSTALLATION FIRMWARE

### ETAPE 1 - Arduino IDE
```
Telecharger: https://www.arduino.cc/en/software
Installer Arduino IDE 2.x
Lancer
```

### ETAPE 2 - Support ESP32
```
Menu: File -> Preferences
Champ "Additional boards manager URLs":
https://espressif.github.io/arduino-esp32/package_esp32_index.json
OK

Menu: Tools -> Board -> Boards Manager
Chercher: esp32
Installer: esp32 by Espressif Systems (version 3.x)
Attendre ~5 min
```

### ETAPE 3 - Librairies
```
Menu: Tools -> Manage Libraries
Chercher: TFT_eSPI -> Installer (Bodmer)
Chercher: Adafruit INA219 -> Installer
Fermer
```

### ETAPE 4 - Patch TFT_eSPI
```
Aller dans:
  Windows: C:\Users\[USER]\Documents\Arduino\libraries\TFT_eSPI\
  Mac/Linux: ~/Documents/Arduino/libraries/TFT_eSPI/

Ouvrir User_Setup.h
EFFACER tout le contenu
COLLER le contenu de User_Setup_C6_LCD.h fourni
Sauvegarder
```

### ETAPE 5 - Configuration
```
Brancher ESP32-C6 en USB-C
Menu: Tools -> Board -> esp32 -> ESP32C6 Dev Module
Menu: Tools -> Port -> [port COM detecte]
Menu: Tools -> USB CDC On Boot -> Enabled
Menu: Tools -> Upload Speed -> 921600
```

### ETAPE 6 - Upload
```
Ouvrir BreakoutBox_V1.2.ino
Cliquer Upload (fleche)
Attendre "Done uploading"
```

### Depannage
```
"No serial port" -> Driver CH340/CP2102, verifier cable USB data
"Failed to connect" -> Maintenir BOOT pendant upload
Ecran noir -> Verifier User_Setup.h modifie
```

---

## TESTS EFFECTUES PAR LE FIRMWARE

### Mode LIVE
Affiche en temps reel:
- +12V_PROT (vert si 11-14.5V)
- +5V_LOG (vert si 4.90-5.10V)
- Courant (vert si <5mA)
- AUTO / MAN / CMD (cyan si actif)
- OUT_DRIVER (ON/OFF)

### Mode FULL AUTO
Enchaine 4 tests:
1. **Power** - Verifie +12V et +5V
2. **Current** - Verifie I_repos < 5mA
3. **StimAuto** - Stimule pressostat, verifie:
   - AUTO_REQ passe HIGH (74HC14 OK)
   - CMD passe HIGH (OR OK)
   - OUT_DRIVER passe ON (BTS5090 OK)
4. **StimManual** - Pulse bouton, verifie:
   - MANUAL_REQ toggle (CD4013 OK)
   - CMD suit (OR OK)
   - OUT_DRIVER suit (BTS5090 OK)

Resultat: ALL PASS ou FAILED avec diagnostic serial

---

**FIN CIRCUIT V1.2**
