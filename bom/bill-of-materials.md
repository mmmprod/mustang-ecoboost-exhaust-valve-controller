# Bill of Materials (BOM) - Circuit VALVE v4.2

## Mustang EcoBoost 2016 Stage 4+ E85 (600+ ch)

---

## 🔴 NOUVEAUTÉS v4.2

| Composant | Action | Raison |
|-----------|--------|--------|
| **SMC IR1000-01BG** | AJOUTÉ | Régulateur pression 90 PSI → 1 bar |
| **SMC KQ2H06-01** (×2) | AJOUTÉ | Adaptateurs 1/8" → push-in Ø6mm |
| **Check valve ligne vacuum** | SUPPRIMÉ | Piégeait la pression → valve bloquée ouverte |
| **Gaine aluminisée** | AJOUTÉ | Protection thermique durite actionneur |

---

## SEMICONDUCTEURS

| Qté | Référence | Description | Package | Specs | Fabricant | Note |
|-----|-----------|-------------|---------|-------|-----------|------|
| 1 | 78L05 | Régulateur 5V 100mA | TO-92 | Vin 7-30V, Vout 5V | LM78L05ACZ | Standard |
| 1 | CD4013 | Dual D Flip-Flop | DIP-14 | CMOS 3-18V | CD4013BE | Standard |
| 1 | 74HC14 | Hex Schmitt Trigger | DIP-14 | CMOS 2-6V | 74HC14N | Standard |
| 1 | BTS5090-1EJA | Smart High-Side Switch | PG-TO252-5 | 60V, 2,5A, Rds=17mΩ | Infineon BTS5090-1EJAXUMA1 | **CRITIQUE** |
| 1 | 1N5822 | Diode Schottky | DO-201AD | 40V 3A, Vf=0,45V | Vishay 1N5822 | Anti-polarité |
| 2 | 1N4148 | Diode signal | DO-35 | 100V 200mA | Vishay 1N4148 | OR logic |
| 1 | 1N5819 | Diode Schottky | DO-41 | 40V 1A | Vishay 1N5819 | Protection relais |
| 1 | 1N4733A | Zener 5,1V | DO-41 | 5,1V 1W ±5% | Vishay 1N4733A | Clamp pressostat |

---

## PROTECTIONS TRANSITOIRES

| Qté | Référence | Description | Package | Specs | Fabricant | Note |
|-----|-----------|-------------|---------|-------|-----------|------|
| 1 | 5KP18CA | TVS bidirectionnelle | DO-201AE | 18V, 5000W, Vclamp 29,2V | Littelfuse 5KP18CA | Load-dump rapide |
| 1 | S20K20 | MOV 20mm | Radial 20mm | 20V DC, 45J (2ms) | EPCOS B72220S0200K101 | Load-dump lent |
| 1 | MOV 14mm | MOV 14mm | Radial 14mm | 26V DC, ~8J | EPCOS B72214S0200K101 | Protection 78L05 |

⚠️ **COLLAGE SILICONE RTV OBLIGATOIRE** après soudure (anti-vibrations ISO 16750-3)

---

## RÉSISTANCES (Métal-Film ±1%)

| Qté | Valeur | Puissance | Note |
|-----|--------|-----------|------|
| 2 | 820Ω | **1/2W** | LED bouton — **CRITIQUE thermique coffre** |
| 1 | 1kΩ | 0,25W | Série pressostat |
| 2 | 4,7kΩ | 0,25W | Driver IN, OR |
| 1 | 47kΩ | 0,25W | DEN driver |
| 4 | 10kΩ | 0,25W | Pull-up/down divers |
| 3 | 100kΩ | 0,25W | CD4013 R/S, reset |

⚠️ **NE PAS PRENDRE 1/4W pour 820Ω** → Thermique insuffisant coffre été

---

## CONDENSATEURS CÉRAMIQUES

| Qté | Valeur | Tension | Type | Note |
|-----|--------|---------|------|------|
| 7 | 100nF | 50V | X7R | Découplage local |
| 1 | 100nF | 25V | X7R | 78L05 VOUT |
| 1 | 100nF | 16V | X7R | Power-on reset CD4013 |
| 1 | 1µF | 16V | X7R | Anti-rebond CLK — **CRITIQUE** |

---

## CONDENSATEURS ÉLECTROLYTIQUES (Low-ESR, 105°C)

| Qté | Valeur | Tension | ESR | Application | Référence |
|-----|--------|---------|-----|-------------|-----------|
| 1 | 22µF | 50V | <0,5Ω | +12V_PROT bulk | Panasonic EEU-FM1H220 |
| 1 | 22µF | 50V | <0,5Ω | BTS5090 VS | Panasonic EEU-FM1H220 |
| 1 | 10µF | 50V | <0,5Ω | 78L05 VIN | Panasonic EEU-FM1E100 |
| 1 | 10µF | 25V | <0,5Ω | 78L05 VOUT | Panasonic EEU-FM1E100 |
| 1 | 47µF | 25V | <0,3Ω | 78L05 VOUT bulk | Panasonic EEU-FM1E470 |

---

## FERRITE EMI

| Qté | Référence | Impédance | Package | Note |
|-----|-----------|-----------|---------|------|
| 1 | Murata BLM31PG221SN1L | 220Ω @ 100MHz | 1206 CMS | Nécessite adaptateur stripboard |

**Alternative stripboard (recommandée):**

| Qté | Référence | Impédance | Package | Note |
|-----|-----------|-----------|---------|------|
| 1 | Wurth 742792093 | 90Ω @ 100MHz | Axial 3A | Montage direct — **PLUS SIMPLE** |

---

## RELAIS & ÉLECTROMÉCANIQUE

| Qté | Référence | Specs | Note |
|-----|-----------|-------|------|
| 1 | Omron G5Q-1-HA-DC12-TY | 12V DC, 360Ω, 10A | ✅ Collage silicone **OBLIGATOIRE** |
| 1 | MAC 35A (3/2 NC) | 12V DC, 5,4W, 1/8 NPT, 120psi | Ports P/A/E |

---

## CAPTEUR PRESSOSTAT

| Qté | Référence | Specs | Réglages |
|-----|-----------|-------|----------|
| 1 | SMC ISE30A-01-N | 12-24V DC, -0,1 à 1MPa, NPN OC | P_ON=3,0 psi, P_OFF=2,2 psi |

---

## 🔴 RÉGULATEUR PRESSION (NOUVEAU v4.2)

| Qté | Référence | Description | Specs | Note |
|-----|-----------|-------------|-------|------|
| 1 | **SMC IR1000-01BG** | Régulateur pression compact | 0,005-0,2 MPa, 1/8", manomètre inclus | **CRITIQUE** |
| 2 | **SMC KQ2H06-01** | Raccord 1/8" mâle → push-in Ø6mm | Ø6mm tube | Adaptateurs régulateur |
| 1 | Ruban PTFE | Téflon étanchéité | 12mm × 10m | Filetages 1/8" |

**Réglage cible:** 0,1 MPa = 1 bar = 15 PSI

---

## FUSIBLES & PROTECTION

| Qté | Type | Rating/Format | Note |
|-----|------|---------------|------|
| 1 | Fusible lame + porte-fusible IP67 | 5A Fast, mini-blade | ≤15cm batterie — **CRITIQUE** |
| 1 | Fusible axial T | 1A, 5×20mm | Entre ferrite et 1N5822 |
| 1 | Fusible mini-blade | 1A | Série bobine MAC |

---

## CONNECTEURS

| Qté | Type | Specs | Application |
|-----|------|-------|-------------|
| 1 | Deutsch DT panel-mount 2 voies | IP67 | Alimentation +12V/GND |
| 1 | GX12 panel-mount 5 pins | IP67 | Bouton LED |
| 3 | Push-in Ø6mm | IP65 | Passe-cloison pneumatique |
| 1 | Header JST-XH 5P mâle PCB | Pitch 2,5mm, 3A | Bouton LED (S5B-XH-A) |
| 1 | Bouton Toyota 22×22mm LED | JST-XH 5P, 12V | 2 LEDs blanc/vert |

---

## CÂBLAGE ÉLECTRIQUE

| Qté | Description | Specs | Note |
|-----|-------------|-------|------|
| 5m | Câble automobile rouge | 2,5mm², 105°C, ISO 6722-1 FLRY-B | Batterie → boîtier |
| 5m | Câble automobile noir | 2,5mm², 105°C, ISO 6722-1 FLRY-B | GND batterie |
| 5m | Gaine tressée PET | Ø8-10mm | Protection mécanique |
| 20 | Colliers plastique | 150mm | Fixation tous 15-20cm |
| 2 | Cosses batterie | M6 ou M8, DIN 46228 | Batterie ± |

---

## PNEUMATIQUE

| Qté | Description | Specs | Note |
|-----|-------------|-------|------|
| 3m | Tube polyuréthane | Ø6mm, 10 bar | Ligne pression (compresseur → MAC → actionneur) |
| 6m | Durite silicone/caoutchouc | Ø6mm | Ligne vacuum (collecteur → pressostat) |
| 1 | Té raccord | 1/4 NPT → 2× Ø6mm | Répartiteur compresseur |
| 1 | Silencieux fritté | 1/8 NPT | MAC port E (évent) |
| 1 | **Gaine aluminisée** | Ø10-12mm × 50cm | **Protection thermique** section actionneur |

### ❌ SUPPRIMÉ v4.2

| Composant | Raison suppression |
|-----------|-------------------|
| ~~Check valve ligne vacuum~~ | Piégeait la pression → valve bloquée ouverte |

---

## CONSOMMABLES

| Description | Référence exemple | Usage |
|-------------|-------------------|-------|
| Silicone RTV | Loctite 5699 ou Dow Corning 3145 | Collage relais, MOV, TVS |
| Flux soudure | Kester 951 | Soudure |
| Alcool isopropylique 99% | - | Nettoyage PCB |
| Graisse contact cuivre | Dow Corning Molykote 44 | Point masse châssis |

---

## BOÎTIER

| Paramètre | Spécification |
|-----------|---------------|
| Matériau | Polycarbonate ou ABS UL94 V-0 (auto-extinguible) |
| IP | IP65 minimum |
| Température | 85°C minimum |
| Dimensions | 150×100×60mm minimum (selon layout) |
| Exemples | Hammond 1554, Bopla Euromas, OKW ROBUST-BOX |

---

## RÉSUMÉ COMMANDE v4.2

### Composants critiques à commander

| Priorité | Composant | Fournisseur suggéré |
|----------|-----------|---------------------|
| 🔴 | SMC IR1000-01BG | SMC Direct, RS, Farnell |
| 🔴 | SMC KQ2H06-01 (×2) | SMC Direct, RS, Farnell |
| 🔴 | BTS5090-1EJAXUMA1 | Mouser, Farnell, RS |
| 🔴 | Omron G5Q-1-HA-DC12-TY | Mouser, Farnell |
| 🔴 | SMC ISE30A-01-N | SMC Direct |
| 🔴 | MAC 35A 3/2 NC 12V | MAC Valves, distributeurs |
| 🟡 | 5KP18CA + S20K20 | Mouser, Farnell |
| 🟡 | Gaine aluminisée 50cm | Auto, bricolage |

---

## COÛT ESTIMÉ

| Catégorie | Estimation |
|-----------|------------|
| Semiconducteurs & CI | ~25-35 € |
| Protections (TVS, MOV) | ~15-20 € |
| Passifs (R, C) | ~10-15 € |
| Relais + MAC 35A | ~80-120 € |
| Pressostat SMC ISE30A | ~150-200 € |
| **Régulateur SMC IR1000-01BG** | ~60-80 € |
| Connecteurs | ~30-40 € |
| Câblage + pneumatique | ~40-60 € |
| Boîtier | ~20-40 € |
| **TOTAL ESTIMÉ** | **~430-610 €** |

---

**— FIN BOM VALVE v4.2 —**
