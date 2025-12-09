# Schéma Électrique v4.2

## Circuit VALVE - Mustang EcoBoost 2016

**Version :** 4.2  
**Date :** Décembre 2025  
**Origine :** Audit technique Gemini — Corrections critiques intégrées

---

## ⚠️ AVERTISSEMENT SÉCURITÉ

```
┌─────────────────────────────────────────────────────────────────┐
│                    ⚠️ FAIL-SAFE VALVE                           │
├─────────────────────────────────────────────────────────────────┤
│ L'actionneur est FAIL-CLOSED (ressort ferme, pression ouvre).   │
│                                                                 │
│ En cas de panne électrique ou pneumatique en pleine charge:     │
│ → La valve se FERME → BACKPRESSURE sur le turbo                │
│                                                                 │
│ RÉACTION: Si perte de puissance soudaine, LEVER LE PIED        │
│ PROBABILITÉ: Faible — Circuit protégé (BTS5090, TVS/MOV)       │
└─────────────────────────────────────────────────────────────────┘
```

---

## Architecture du Circuit

Le circuit est divisé en blocs fonctionnels interconnectés :

```
BLOC A0-R: Relais de puissance Omron G5Q-1
BLOC A:    Alimentation protégée 12V (Ferrite, TVS, MOV)
BLOC B:    Régulateur 5V (78L05)
BLOC C:    Interface Pressostat (SMC ISE30A + 74HC14)
BLOC D:    Bouton LED Toyota 22×22mm
BLOC E:    Bascule Manuel/Auto (CD4013)
BLOC F:    Logique OR (1N4148)
BLOC G:    Driver haute puissance (BTS5090-1EJA) → MAC 35A
```

---

## BLOC A0-R — Relais Omron G5Q-1-HA-DC12-TY

### Composant
- **Référence :** Omron G5Q-1-HA-DC12-TY
- **Type :** SPDT 12V DC, 360Ω, 10A
- **Package :** Micro-ISO PCB
- **Brochage (vue dessous) :** 1=Coil, 2=Coil, 3=COM, 4=NC, 5=NO

### Câblage

```
BATTERIE (+) → Fusible lame 5A externe (≤15cm borne) → [+12V_BATT]
[+12V_BATT] → Pin 3 (COM)
Pin 5 (NO) → [+12V_IN]
Pin 1 (Coil) ← [+12V_ACC]
Pin 2 (Coil) → [GND_STAR]
1N5819 : Cathode → Pin 1, Anode → [GND_STAR]
```

### Protection anti-vibrations
⚠️ **OBLIGATOIRE :** Collage au silicone RTV après soudure (ISO 16750-3)

---

## BLOC A — Alimentation Protégée 12V

### Chaîne de protection

```
[+12V_IN] → Ferrite 220Ω@100MHz → Fusible 1A T → 1N5822 → [+12V_PROT]
                                                    ↑
                                              Bande cathode
```

### Composants de protection

| Composant | Référence | Position | Fonction |
|-----------|-----------|----------|----------|
| Ferrite | BLM31PG221SN1L (1206) ou Wurth 742792093 (axial) | Entrée | Filtrage EMI 220Ω@100MHz |
| Fusible | 1A Temporisé 5×20mm | Série après ferrite | Protection court-circuit |
| Diode | 1N5822 (Schottky 40V 3A) | Série | Anti-polarité Vf=0,45V |
| TVS | 5KP18CA (DO-201AE) | [+12V_PROT]↔[GND_STAR] ≤10mm | Pics rapides 5000W |
| MOV | S20K20 (20mm) | [+12V_PROT]↔[GND_STAR] ≤10mm | Load-dump 45J |
| MOV | 14mm ~26VDC | [+12V_PROT]↔[GND_STAR] ≤5mm 78L05 | Second rideau |

### Découplage 12V

```
[+12V_PROT] → 100nF X7R 50V ∥ 22µF Low-ESR 50V → [GND_STAR]
```

⚠️ **Orientation 1N5822 :** Bande blanche (cathode) côté [+12V_PROT]

---

## BLOC B — Régulateur 5V (78L05)

### Composant
- **Référence :** 78L05 (TO-92), LM78L05-TT
- **Brochage :** 1=Vout, 2=GND, 3=Vin
- **Specs :** Vin 7-30V, Vout 5V ±5%, 100mA max

### Câblage

```
[+12V_PROT] → 78L05 Pin 3 (Vin)
78L05 Pin 2 (GND) → [GND_STAR]
78L05 Pin 1 (Vout) → [+5V_LOG]
```

### Découplages obligatoires

| Position | Composants | Distance max |
|----------|------------|--------------|
| VIN | 100nF X7R 50V ∥ 10µF Low-ESR 50V | ≤5mm pins |
| VOUT | 100nF X7R 25V + 10µF + 47µF Low-ESR 25V | ≤5mm pins |

---

## BLOC C — Pressostat SMC ISE30A-01-N

### Composant
- **Référence :** SMC ISE30A-01-N
- **Type :** NPN open-collector, 12-24V DC
- **Plage :** -0,1 à 1 MPa
- **Sortie :** NPN 100mA max

### Câblage pressostat

| Fil pressostat | Couleur | Destination |
|----------------|---------|-------------|
| Alimentation | Brun | [+12V_PROT] |
| Masse | Bleu | [GND_STAR] |
| Sortie | Noir | 1kΩ série → IN_A |

### Interface logique 74HC14

```
Pull-up 10kΩ : IN_A → [+5V_LOG]
Zener 5,1V/1W (1N4733A) : IN_A → [GND_STAR]
Découplage 100nF X7R : IN_A → [GND_STAR]

IN_A → 74HC14 Pin X (Schmitt Trigger)
74HC14 OUT → [AUTO_REQ]
```

### Réglages pressostat

| Paramètre | Valeur psi | Valeur bar |
|-----------|------------|------------|
| P_ON | 2,9-3,6 | 0,20-0,25 |
| P_OFF | 2,0-2,9 | 0,14-0,20 |
| Hystérésis | ≥0,7 | ≥0,05 |

### Brochage 74HC14 (DIP-14)

```
        ┌────┬────┐
   1A ──│1  ⚬│14 │── VCC (+5V_LOG)
   1Y ──│2   │13 │── 6A
   2A ──│3   │12 │── 6Y
   2Y ──│4   │11 │── 5A
   3A ──│5   │10 │── 5Y
   3Y ──│6   │9  │── 4A
  GND ──│7   │8  │── 4Y
        └────────┘
```

**Découplage VCC :** 100nF X7R ≤5mm pin 14

---

## BLOC D — Bouton Toyota 22×22mm LED

### Connecteur
- **Type :** JST-XH 5P (pas 2,5mm, 3A)
- **Référence :** S5B-XH-A (Mâle PCB)

### Brochage header carte

| Pin | Signal | Fonction |
|-----|--------|----------|
| 1 | [LED_12V] | +12V LED via résistances |
| 2 | GND_LED | Masse LED |
| 3 | [BTN_RET] | Retour bouton |
| 4 | [BTN_SIG] | Signal bouton |
| 5 | NC | Non connecté |

### Résistances LED ⚠️ CRITIQUE

| LED | Résistance | Puissance | Raison |
|-----|------------|-----------|--------|
| Blanche | 820Ω ±1% | **1/2W** | Thermique coffre été 50°C |
| Verte | 820Ω ±1% | **1/2W** | Thermique coffre été 50°C |

**Courant :** ~13-14mA par LED  
**Dissipation :** 150-170mW par résistance  
**Marge thermique avec 1/2W :** 62% → robustesse canicule garantie

⚠️ **NE PAS utiliser 1/4W** sous peine de défaillance thermique

---

## BLOC E — Bascule CD4013 (Toggle Manuel/Auto)

### Composant
- **Référence :** CD4013BE (DIP-14)
- **Type :** Dual D Flip-Flop CMOS
- **Alimentation :** 3-18V

### Brochage CD4013 (DIP-14)

```
        ┌────┬────┐
   Q1 ──│1  ⚬│14 │── VDD (+5V_LOG)
  Q̅1 ──│2   │13 │── Q2
 CLK1──│3   │12 │── Q̅2
  RS1──│4   │11 │── CLK2
  D1 ──│5   │10 │── RS2
  S1 ──│6   │9  │── D2
 VSS ──│7   │8  │── S2
        └────────┘
```

### Câblage flip-flop (utilisation FF1)

```
VDD (pin 14) ← [+5V_LOG]
VSS (pin 7) ← [GND_STAR]
D (pin 5) → Q̅ (pin 2)  [rebouclage toggle]
R (pin 4) → 100kΩ → [GND_STAR]
S (pin 6) → 100kΩ → [GND_STAR]
[BTN_SIG] → 10kΩ série → CLK (pin 3)
Pull-up 10kΩ : CLK (pin 3) → [+5V_LOG]
Anti-rebond 1µF X7R 16V : CLK (pin 3) → [GND_STAR]
Q (pin 1) = [MANUAL_REQ]
```

### Reset power-on

```
100nF X7R : [+5V_LOG] → R (pin 4)
100kΩ : R (pin 4) → [GND_STAR]
```

**Effet :** Impulsion brève sur R au démarrage → Q=0 garanti

**Découplage VDD :** 100nF X7R ≤5mm pin 14

---

## BLOC F — OR Auto/Manuel → [CMD]

### Logique de commande

```
[AUTO_REQ] ────►|──┐
              1N4148│
                   ├──► [CMD]
[MANUAL_REQ] ──►|──┘
              1N4148

Pull-down 10kΩ : [CMD] → [GND_STAR]
[CMD] → 4,7kΩ → BTS5090 IN
```

**Fonction :** OR diode — activation si AUTO_REQ OU MANUAL_REQ actif

---

## BLOC G — Driver BTS5090-1EJA → MAC 35A

### G1. Driver Infineon BTS5090-1EJAXUMA1

**Package :** PG-TO252-5  
**Specs :** 60V, 2,5A, Rds(on)=17mΩ, Smart high-side switch

**Brochage (vue dessus) :**

```
       ┌─────────────┐
   GND │1         Pad│ VS
    IN │2          8 │ OUT
   DEN │3          7 │ OUT
    IS │4          6 │ OUT
    NC │5            │
       └─────────────┘
```

### Câblage driver

```
Pad VS → [+12V_PROT]
Découplage VS : 100nF X7R 50V ∥ 22µF Low-ESR 50V → [GND_STAR] (≤5mm)
IN (pin 2) ← [CMD] via 4,7kΩ
DEN (pin 3) → 47kΩ → [GND_STAR]
GND (pin 1) → [GND_STAR]
OUT (pins 6-7-8 pontés) → [OUT_DRIVER]
```

⚠️ **Pas de diode externe sur bobine** — le driver gère l'induction en interne

### G2. Protection bobine MAC 35A

```
[OUT_DRIVER] → Fusible mini-blade 1A → Fil "+" bobine MAC
Fil "−" bobine MAC → [GND_STAR] UNIQUEMENT
```

⚠️ **Pas de masse châssis directe sur retour bobine**

### G3. Électrovanne MAC 35A 3/2 NC 12V

| Paramètre | Valeur |
|-----------|--------|
| Puissance | 5,4W (~450mA @ 12V) |
| Ports | 1/8 NPT : P(1)=supply, A(2)=actuator, E(3)=exhaust |
| Fonction repos | A→E (purge), P bloqué |
| Fonction énergisée | P→A (alimentation actionneur), E bloqué |
| Temps réponse | ~6ms ON, ~2ms OFF |

**Port E(3) :** Silencieux fritté, pointe vers le bas

---

## Points Critiques de Routage

### Masses (Architecture étoile)

```
[GND_STAR] = Point étoile central
  │
  ├─ 78L05 GND (pin 2)
  ├─ 74HC14 VSS (pin 7)
  ├─ CD4013 VSS (pin 7)
  ├─ BTS5090 GND (pin 1)
  ├─ Pressostat Bleu
  ├─ Retour bobine MAC
  ├─ Découplages
  └─ [GND_IN] (Deutsch DT)
```

⚠️ **PAS de masse châssis directe sur logique ou bobine MAC**

### Découplages critiques

| CI | Position | Composants | Distance max |
|----|----------|------------|--------------|
| 78L05 | VIN/VOUT | 100nF + 10µF + 47µF | ≤5mm pins |
| 74HC14 | VCC (pin 14) | 100nF X7R | ≤5mm |
| CD4013 | VDD (pin 14) | 100nF X7R | ≤5mm |
| BTS5090 | VS (pad) | 100nF ∥ 22µF | ≤5mm |

### Séparation signaux

- **Ligne pressostat :** Éloigner des commutations 12V (BTS5090)
- **Câble bouton :** Torsadé, gaine si >50cm

---

## Valeurs Normalisées Composants

### Résistances (Métal-film ±1%)

| Valeur | Puissance | Quantité | Application |
|--------|-----------|----------|-------------|
| 820Ω | **1/2W** | 2 | LED bouton ⚠️ CRITIQUE |
| 1kΩ | 0,25W | 1 | Série pressostat |
| 4,7kΩ | 0,25W | 2 | Driver IN, OR |
| 10kΩ | 0,25W | 4 | Pull-up/down |
| 47kΩ | 0,25W | 1 | DEN driver |
| 100kΩ | 0,25W | 3 | CD4013 R/S, reset |

### Condensateurs céramiques X7R

| Valeur | Tension | Quantité | Application |
|--------|---------|----------|-------------|
| 100nF | 50V | 7 | Découplage local |
| 100nF | 25V | 1 | 78L05 VOUT |
| 100nF | 16V | 1 | Power-on reset CD4013 |
| 1µF | 16V | 1 | Anti-rebond CLK ⚠️ CRITIQUE |

### Condensateurs électrolytiques (Low-ESR 105°C)

| Valeur | Tension | Quantité | Application |
|--------|---------|----------|-------------|
| 22µF | 50V | 2 | +12V_PROT bulk, BTS5090 VS |
| 10µF | 50V | 1 | 78L05 VIN |
| 10µF | 25V | 1 | 78L05 VOUT |
| 47µF | 25V | 1 | 78L05 VOUT bulk |

### Semiconducteurs

| Référence | Type | Package | Quantité |
|-----------|------|---------|----------|
| 78L05 | Régulateur 5V | TO-92 | 1 |
| CD4013 | Dual D Flip-Flop | DIP-14 | 1 |
| 74HC14 | Hex Schmitt Trigger | DIP-14 | 1 |
| BTS5090-1EJAXUMA1 | Smart High-Side Switch | PG-TO252-5 | 1 |
| 1N5822 | Diode Schottky 40V 3A | DO-201AD | 1 |
| 1N5819 | Diode Schottky 40V 1A | DO-41 | 1 |
| 1N4148 | Diode signal | DO-35 | 2 |
| 1N4733A | Zener 5,1V 1W | DO-41 | 1 |

### Protections transitoires

| Référence | Type | Quantité |
|-----------|------|----------|
| 5KP18CA | TVS bidirectionnelle 5000W | 1 |
| S20K20 | MOV 20mm 45J | 1 |
| MOV 14mm | MOV ~26VDC | 1 |

⚠️ **Collage silicone RTV obligatoire** sur MOV et TVS (anti-vibrations)

---

## Connecteurs Façade Boîtier

### J1 — POWER IN (Deutsch DT 2-voies)

| Pin | Signal | Destination interne |
|-----|--------|---------------------|
| 1 | +12V_IN | → Ferrite BLOC A |
| 2 | GND_IN | → [GND_STAR] |

### J2 — BOUTON LED (GX12 5 pins)

| Pin | Signal | Fonction |
|-----|--------|----------|
| 1 | [LED_12V] | +12V LED via 820Ω 1/2W |
| 2 | GND_LED | Masse LED |
| 3 | [BTN_RET] | Retour bouton → GND |
| 4 | [BTN_SIG] | Signal bouton → CD4013 CLK |
| 5 | NC | Non connecté |

---

## ⚠️ ATTENTIONS CRITIQUES

| Point | Description |
|-------|-------------|
| Polarité 1N5822 | Bande cathode côté [+12V_PROT]. Inversion = perte protection |
| MOV + TVS | S20K20 et 5KP18CA en parallèle ≤10mm entrée [+12V_PROT] |
| Retour bobine MAC | [GND_STAR] UNIQUEMENT, pas de masse châssis |
| Fusible bobine MAC | 1A entre [OUT_DRIVER] et MAC (protection driver) |
| Découplages | 100nF au ras VCC de chaque CI (≤5mm) |
| Résistances LED | 2× 820Ω **1/2W** obligatoire (thermique coffre) |
| Relais G5Q-1 | Collage silicone RTV après soudure (vibrations) |
| Ferrite | BLM31PG221SN1L (CMS) ou Wurth 742792093 (axial plus simple) |

---

## Test et Validation

### Tests électriques (hors véhicule)

1. **Continuité** : Vérifier toutes connexions avant mise sous tension
2. **Polarité 1N5822** : Multimètre diode, Vf ~0,45V sens correct
3. **Régulateur 78L05** : [+5V_LOG] = 5V ±0,25V sans charge
4. **Pressostat** : Simuler pression avec pompe manuelle
5. **Bouton** : Test toggle LED + commutation CD4013
6. **Driver BTS5090** : [OUT_DRIVER] = 12V si [CMD] actif

### Tests fonctionnels (sur véhicule)

1. **Mode Manuel Fermé** : Valve reste fermée en toute condition
2. **Mode Auto** : Valve s'ouvre à 0,20-0,25 bar MAP
3. **Mode Manuel Ouvert** : Valve reste ouverte en toute condition
4. **Étanchéité pneumatique** : Aucune fuite détectable au savon
5. **Temps réponse** : Valve s'ouvre en <20ms (vérifier au son)

---

**— FIN SCHÉMA ÉLECTRIQUE v4.2 —**
