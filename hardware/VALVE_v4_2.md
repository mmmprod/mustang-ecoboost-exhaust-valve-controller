# Circuit VALVE v4.2

## Système Valve Échappement Automatique
**Mustang EcoBoost 2016 Stage 4+ E85 (600+ ch)**

---

## CHANGELOG v4.2

**Date:** Décembre 2025  
**Origine:** Audit technique Gemini — Points critiques validés

### 🔴 CORRECTIONS CRITIQUES

| Correction | Description |
|------------|-------------|
| Check valve SUPPRIMÉE | Ligne vacuum → pressostat (piège pression) |
| Régulateur SMC IR1000-01BG AJOUTÉ | Réduction 90 PSI → 1 bar |

### 🟡 DOCUMENTATION

- Fail-safe documenté (valve fail-closed, risque accepté)
- Protection thermique durite recommandée (gaine aluminisée)
- Schéma pneumatique complet mis à jour

---

## ⚠️ AVERTISSEMENT SÉCURITÉ

```
┌─────────────────────────────────────────────────────────────────┐
│                    ⚠️ FAIL-SAFE VALVE                           │
├─────────────────────────────────────────────────────────────────┤
│ L'actionneur est FAIL-CLOSED (ressort ferme, pression ouvre).   │
│                                                                 │
│ En cas de panne électrique ou pneumatique en pleine charge:     │
│ → La valve se FERME                                             │
│ → BACKPRESSURE sur le turbo                                     │
│                                                                 │
│ RÉACTION REQUISE: Si perte de puissance soudaine,              │
│                   LEVER LE PIED IMMÉDIATEMENT                   │
│                                                                 │
│ PROBABILITÉ: Faible — Circuit V4.2 protégé                      │
│ (BTS5090, fusibles, TVS/MOV)                                    │
│ Configuration standard dans la communauté Stage 3-4.            │
└─────────────────────────────────────────────────────────────────┘
```

---

## BLOC A0-R — Relais micro-ISO PCB 12V

### Composant
- **Relais:** Omron G5Q-1-HA-DC12-TY, SPDT 12V
- **Brochage dessous:** 1-Coil, 2-Coil, 3-COM, 4-NC, 5-NO
- **Données coil:** 12V typ. 360Ω (~33mA)
- **Fixation:** ✅ Coller au silicone RTV après soudure (anti-vibrations)

### Câblage

```
BATTERIE → fusible lame 5A externe (≤15cm borne) → [+12V_BATT] → pin 3 COM

Pin 5 NO → [+12V_IN]
Pin 1 Coil ← [+12V_ACC]
Pin 2 Coil → [GND_STAR]
1N5819 cathode côté pin 1, anode → [GND_STAR]
```

⚠️ **Aucune masse châssis directe ici.**

---

## BLOC A — Alimentation protégée 12V

### Chaîne d'entrée

```
[+12V_IN] → Ferrite 220Ω@100MHz (BLM31PG221SN1L 1206)
         → Fusible 1A T
         → 1N5822 (anode→cathode)
         → [+12V_PROT]
```

### Écrêtage transitoires

| Composant | Position | Fonction |
|-----------|----------|----------|
| TVS 5KP18CA (DO-201 axial) | Entre [+12V_PROT] et [GND_STAR], ≤10mm entrée | Pics rapides |
| MOV SIOV 20mm S20K20 | Entre [+12V_PROT] et [GND_STAR], ≤10mm entrée | Load-dump |
| MOV 14mm ~26VDC | Entre [+12V_PROT] et [GND_STAR], ≤5mm du 78L05 | Second rideau |

### Découplage 12V

```
[+12V_PROT] → 100nF X7R 50V ∥ 22µF Low-ESR 50V → [GND_STAR]
```

### Orientation 1N5822

```
[+12V_IN] ──►|── 1N5822 ──► [+12V_PROT]
              ↑
        Bande blanche côté [+12V_PROT]
```

---

## BLOC B — Régulateur 5V

### Composant
- **Régulateur:** 78L05 (TO-92), réf LM78L05-TT
- **Broches:** 1=Vout, 2=GND, 3=Vin

### Câblage

```
[+12V_PROT] → 78L05 Vin (pin 3)
78L05 GND (pin 2) → [GND_STAR]
78L05 Vout (pin 1) → [+5V_LOG]
```

### Découplages

| Position | Composants |
|----------|------------|
| VIN | 100nF X7R 50V ∥ 10µF Low-ESR 50V au ras |
| VOUT | 100nF X7R 25V + 10µF + 47µF Low-ESR 25V au ras |

---

## BLOC C — Pressostat SMC ISE30A-01-N

### Type
- NPN open-collector
- Alimentation 12-24V

### Câblage fils

| Fil | Destination |
|-----|-------------|
| Brun | [+12V_PROT] |
| Bleu | [GND_STAR] |
| Noir (OUT) | 1kΩ série → 74HC14 IN_A |

### Interface logique

```
Pull-up 10kΩ de IN_A → [+5V_LOG]
Zener 5,1V/1W (1N4733A) de IN_A → [GND_STAR]
100nF X7R de IN_A → [GND_STAR]
```

### Réglages pressostat

| Paramètre | Valeur psi | Valeur bar |
|-----------|------------|------------|
| P_ON | 2,9-3,6 | 0,20-0,25 |
| P_OFF | 2,0-2,9 | 0,14-0,20 |
| Hystérésis | ≥0,7 | ≥0,05 |

**Anti-rebond circuit:** 100ms (RC + 74HC14)

---

## BLOC D — Bouton Toyota 22×22mm LED

### Connecteur
- JST-XH 5P (pas 2,5mm, 3A)

### Brochage header carte

| Pin | Signal |
|-----|--------|
| 1 | [LED_12V] |
| 2 | GND_LED |
| 3 | [BTN_RET] |
| 4 | [BTN_SIG] |
| 5 | NC |

### Câblage bouton (Option 2)

| Fil | Destination |
|-----|-------------|
| Vert | [LED_12V] |
| Noir+Blanc | GND_LED |
| Rouge+Jaune torsadés | [BTN_SIG] |
| [BTN_RET] | GND |

### Résistances LED (2× séparées)

| LED | Résistance | Courant | Puissance | Marge |
|-----|------------|---------|-----------|-------|
| Blanche | 820Ω 1/2W métal film ±1% | 13,7mA | 153mW | 31% rating |
| Verte | 820Ω 1/2W métal film ±1% | 14,5mA | 172mW | 34% rating |

**Marge thermique coffre été (50°C):** 62% avec 1/2W — robustesse canicule garantie

---

## BLOC E — Bascule CD4013 (toggle manuel/auto)

### Alimentation
- VDD = [+5V_LOG] (pin 14)
- VSS = [GND_STAR] (pin 7)

### Câblage flip-flop

```
D (pin 5) → Q̅ (pin 2)
R (pin 4) → 100kΩ → [GND_STAR]
S (pin 6) → 100kΩ → [GND_STAR]
[BTN_SIG] → 10kΩ série → CLK (pin 3)
Pull-up 10kΩ de CLK (pin 3) → [+5V_LOG]
CLK (pin 3) → 1µF X7R 16V → [GND_STAR] (filtrage rebonds)
Q (pin 1) = [MANUAL_REQ]
```

### Reset power-on

```
100nF entre [+5V_LOG] et R (pin 4)
100kΩ de R (pin 4) → [GND_STAR]
```

**Effet:** Impulsion brève sur R au power-on → Q=0 garanti au démarrage

---

## BLOC F — OR Auto/Manuel → [CMD]

```
[AUTO_REQ] → 1N4148 → [CMD]
[MANUAL_REQ] → 1N4148 → [CMD]
Pull-down 10kΩ de [CMD] → [GND_STAR]
[CMD] → 4,7kΩ → IN driver BTS5090
```

---

## BLOC G — Driver BTS5090 → MAC 35A

### G1. Driver Infineon BTS50901EJAXUMA1

**Brochage:** 1=GND, 2=IN, 3=DEN, 4=IS, 5=NC, 6-7-8=OUT, pad exposé=VS

```
Pad VS → [+12V_PROT]
Découplage au ras VS: 100nF X7R 50V ∥ 22µF Low-ESR 50V → [GND_STAR]
IN (pin 2) ← [CMD] via 4,7kΩ
DEN (pin 3) → 47kΩ → [GND_STAR]
GND (pin 1) → [GND_STAR]
OUT (6-7-8 pontés) → [OUT_DRIVER]
```

⚠️ **Note:** Ne pas ajouter de diode sur la bobine — le driver gère l'induction

### G2. Protection bobine & retour

```
[OUT_DRIVER] → fusible mini-blade 1A → fil "+" bobine MAC
Fil "−" bobine MAC → [GND_STAR] uniquement (pas de masse châssis directe)
```

### G3. Électrovanne MAC 35A 3/2 NC 12V

| Paramètre | Valeur |
|-----------|--------|
| Puissance | 5,4W |
| Ports | 1/8 NPT: 1=P (supply), 2=A (actuator), 3=E (exhaust) |
| Fonction repos | A→E (purge), P bloqué |
| Fonction énergisée | P→A (alimentation actionneur), E bloqué |
| Temps réponse | ~6ms ON, ~2ms OFF |
| Port 3 | Silencieux fritté, pointe vers le bas |

---

## BLOC PNEUMATIQUE — Schéma complet v4.2

### 🔴 CORRECTIONS CRITIQUES AUDIT GEMINI INTÉGRÉES

### Ligne Détection (mesure boost)

```
┌─────────────────────────────────────────────────────────────────┐
│  ❌ ZÉRO CHECK VALVE SUR CETTE LIGNE                            │
│                                                                 │
│  Problème corrigé: Une check valve piégeait la pression         │
│  positive après accélération → pressostat restait ON            │
│  → valve ne se refermait jamais.                                │
└─────────────────────────────────────────────────────────────────┘
```

**Schéma ligne détection:**

```
Collecteur admission → Durite Ø6mm libre (sans restriction) → Passe-cloison coffre → SMC ISE30A-01-N
```

### Ligne Commande (actionnement valve)

```
┌─────────────────────────────────────────────────────────────────┐
│  ✅ RÉGULATEUR SMC IR1000-01BG AJOUTÉ                           │
│                                                                 │
│  Problème corrigé: Compresseur HS-551 délivre 90-120 PSI        │
│  (6-8 bar). Actionneur valve s'ouvre à 7 PSI (0,5 bar).         │
│  Ratio 13× → destruction membrane sans régulateur.              │
└─────────────────────────────────────────────────────────────────┘
```

**Schéma ligne commande:**

```
Compresseur HS-551 (90-120 PSI)
         │
         ▼
    ┌────┴────┐
    │   Té    │
    └────┬────┘
         │
    ┌────┴────┐
    │         │
    ▼         ▼
 Klaxon    Boîtier
           │
           │  Durite Ø6mm
           │
           ▼
    Raccord push-in Ø6mm → 1/8"
           │
           ▼
┌─────────────────────────────────┐
│  SMC IR1000-01BG                │
│  Réglé: 0,1 MPa = 1 bar = 15 PSI│
└─────────────────────────────────┘
           │
           │  Sortie régulateur
           │  Raccord 1/8" → push-in Ø6mm
           │
           ▼
    Passe-cloison J-AIR
           │
           ▼
    MAC 35A port P(1)
           │
           │  Port A(2)
           ▼
    Passe-cloison → Gaine aluminisée 50cm → Actionneur valve
    
    MAC port E(3) → Silencieux fritté vers le bas
```

---

## Régulateur SMC IR1000-01BG — Spécifications

| Paramètre | Valeur |
|-----------|--------|
| Série | IR1000 (compact) |
| Plage réglage | 0,005-0,2 MPa (0,05-2 bar) |
| Pression entrée max | 1,0 MPa (10 bar) ✅ |
| Filetage | Rc 1/8" (adaptateurs requis) |
| Manomètre | Inclus (code G) |
| Support fixation | Inclus (code B) |
| **Réglage cible** | **0,1 MPa (1 bar = 15 PSI)** |

### Adaptateurs requis

- 2× Raccord 1/8" mâle → push-in Ø6mm (ex: SMC KQ2H06-01)
- Ruban PTFE (Téflon) pour étanchéité filetages

### Protection thermique durite

| Paramètre | Valeur |
|-----------|--------|
| Section critique | Durite entre passe-cloison moteur et actionneur (proche échappement) |
| Protection recommandée | Gaine aluminisée 50cm sur dernière section uniquement |
| Note | Boîtier dans coffre → pas d'exposition thermique directe |

---

## BLOC J — Connecteurs façade

### J1 — POWER IN 12V (Deutsch DT 2-voies)

| Pin | Fonction |
|-----|----------|
| 1 | +12V_IN → entre sur ferrite BLOC A |
| 2 | GND_IN → [GND_STAR] |

### J2 — BOUTON LED (GX12 5 fils)

| Pin | Fil | Fonction |
|-----|-----|----------|
| 1 | Rouge | +LED1 / sortie contact |
| 2 | Jaune | +LED2 / backlight |
| 3 | Noir | −LED |
| 4 | Blanc | −LED |
| 5 | Vert | +12V bouton |

### J-AIR — Passe-cloison pneumatique push-in Ø6mm

| Côté | Connexion |
|------|-----------|
| Extérieur | ← tube Ø6mm depuis régulateur SMC |
| Intérieur | → tube Ø6mm vers MAC port P(1) |

---

## BLOC I — Valeurs normalisées

### Résistances
- Métal-film ±1% 0,25W
- **Exception BLOC D:** 2× 820Ω **1/2W**

### Condensateurs
- 100nF X7R 50V en découplage local
- 10-22-47µF Low-ESR 105°C pour filtrage

### Diodes
- 1N4148 (OR logique)
- 1N5822 (anti-polarité série)

### Notes routage
- Masse logique (74HC14, CD4013): ramener en étoile au 78L05
- Pressostat: câble séparé des commutations 12V (BTS5090)
- Découplages: ≤5mm des pins Vcc de chaque CI

---

## ⚠️ ATTENTIONS CRITIQUES

| Point | Description |
|-------|-------------|
| Polarité 1N5822 | Bande cathode côté [+12V_PROT]. Inversion = perte anti-polarité |
| MOV + TVS | S20K20 et 5KP18CA en parallèle sur [+12V_PROT]↔[GND_STAR], ≤10mm entrée |
| TVS 5KP18CA | Axiale DO-201, bidirectionnelle. Soudure propre (composant puissance) |
| Retour bobine MAC | UNIQUEMENT [GND_STAR]. Pas de masse châssis directe |
| Fusible 1A série bobine | Entre [OUT_DRIVER] et MAC (court-circuit = fusible saute avant driver) |
| Découplages | 100nF au ras de chaque Vcc CI (≤5mm); sur VS driver 100nF ∥ 22µF |
| LEDs bouton | 2× 820Ω séparées. Vérifier brillance équilibrée blanc/vert |
| **Ligne vacuum pressostat** | **ZÉRO CHECK VALVE. Durite libre collecteur → coffre** |
| **Régulateur pression** | **OBLIGATOIRE. SMC IR1000-01BG réglé 1 bar entre compresseur et MAC** |

### Fixation mécanique vibrations

| Composant | Action |
|-----------|--------|
| Relais G5Q-1 | Coller au silicone après soudure |
| MOV S20K20 et TVS 5KP18CA | Collage silicone recommandé |
| Silicone | Loctite 5699 ou Dow Corning 3145 RTV |
| Application | Enrober base composant sans couvrir marquages |
| Objectif | Tenue ISO 16750-3 vibrations (10-500Hz, 20g) |

---

## 📋 ACTIONS AVANT MONTAGE

### Checklist obligatoire avant installation terrain

| Status | Action | Détail |
|--------|--------|--------|
| ❌ | Supprimer check valve | Ligne vacuum → pressostat (critique sécurité) |
| ✅ | Commander régulateur | SMC IR1000-01BG + adaptateurs 1/8" → Ø6mm |
| 🔧 | Installer régulateur | Compresseur → Té klaxon → Régulateur (1 bar) → MAC P |
| ✅ | Vérifier protection thermique | Gaine aluminisée 50cm section actionneur |
| 📝 | Documenter fail-safe | Valve fail-closed, réaction requise si panne |

---

**— FIN DOCUMENT VALVE v4.2 —**
