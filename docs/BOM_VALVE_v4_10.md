# BOM VALVE v4.10

## Ajouts v4.10 (Tempo Fermeture Anti-Flutter)

| Qty | Ref | Composant | Valeur | Package | Fournisseur | Prix unit |
|-----|-----|-----------|--------|---------|-------------|-----------|
| 1 | D4 | Diode signal | 1N4148 | DO-35 Axial | TME, Mouser | 0.05 EUR |
| 1 | R12 | Resistance | 100k 1% 1/4W | Axial MF | TME, Mouser | 0.10 EUR |
| 1 | C13 | Condensateur film | 10uF 16V MKT | Radial 5mm | TME, Mouser | 1.20 EUR |

**Total ajouts v4.10:** ~1.35 EUR

---

## BOM Complete v4.10

### Semiconducteurs

| Qty | Ref | Composant | Valeur | Package | Notes |
|-----|-----|-----------|--------|---------|-------|
| 1 | D_TVS | TVS | 1.5KE18CA 18V 1500W | DO-15 Axial | Cathode (bague) cote +12V_IN |
| 1 | D1 | Schottky | 1N5822 40V 3A | DO-201AD Axial | Bande cote +12V_PROT |
| 1 | D2 | Zener | 1N4733A 5.1V 1W | DO-41 Axial | Protection entree logique |
| 3 | D3,D4,D5 | Signal | 1N4148 | DO-35 Axial | D4=tempo, D3/D5=OR |
| 1 | D6 | Schottky | 1N5819 40V 1A | DO-41 Axial | Roue libre relais |

### Circuits Integres

| Qty | Ref | Composant | Valeur | Package | Notes |
|-----|-----|-----------|--------|---------|-------|
| 1 | U1 | Regulateur | NCV2931AZ-5.0G | TO-92 | **Pin 1=Vout, 2=GND, 3=Vin** |
| 1 | U2 | Buffer | 74HC14 | DIP-14 | Schmitt trigger |
| 1 | U3 | Flip-flop | CD4013 | DIP-14 | Toggle bouton |
| 1 | U4 | Driver | BTS5090-1EJA | TSSOP-8 | Sur adaptateur DIP |

### Adaptateurs

| Qty | Ref | Composant | Reference |
|-----|-----|-----------|-----------|
| 1 | ADAPT1 | TSSOP-8 vers DIP-8 | Aries 08-350000-11 |

### Resistances (toutes Axial MF THT)

| Qty | Ref | Valeur | Tolerance | Puissance |
|-----|-----|--------|-----------|-----------|
| 2 | R1,R2 | 10k | 5% | 1/4W |
| 1 | R3 | 1k | 5% | 1/4W |
| 2 | R4,R5 | 100k | 5% | 1/4W |
| 1 | R6 | 4.7k | 5% | 1/4W |
| 1 | R7 | 47k | 5% | 1/4W |
| 2 | R8,R9 | 820 | 5% | 1/2W |
| 1 | R20 | 470 | 5% | 1/4W |
| 1 | **R12** | **100k** | **1%** | **1/4W** |

### Condensateurs

| Qty | Ref | Valeur | Tension | Type | Package |
|-----|-----|--------|---------|------|---------|
| 6 | C1-C6 | 100nF | 50V | X7R ceramique | Radial 5mm |
| 1 | C7 | 100uF | 50V | Low-ESR electro | Radial 8mm |
| 1 | C8 | 22uF | 50V | Low-ESR electro | Radial 5mm |
| 2 | C9,C10 | 10uF | 25V | Low-ESR electro | Radial 5mm |
| 1 | C11 | 47uF | 25V | Low-ESR electro | Radial 6mm |
| 1 | C12 | 1uF | 16V | X7R ceramique | Radial 5mm |
| 1 | **C13** | **10uF** | **16V** | **Film MKT** | **Radial 5mm** |

### Ferrites

| Qty | Ref | Composant | Valeur | Package |
|-----|-----|-----------|--------|---------|
| 1 | FB1 | Ferrite EMI | Wurth 742792093 | Axial |

### Electromecanique

| Qty | Ref | Composant | Valeur | Package |
|-----|-----|-----------|--------|---------|
| 1 | K1 | Relais SPDT | Omron G5Q-1-HA-DC12-TY | PCB |
| 1 | F1 | Fusible | 1A T verre | 5x20mm |
| 1 | F2 | Fusible | 1A mini-blade | ATO |
| 1 | SW1 | Bouton TEST | Tactile NO | 6x6mm |
| 1 | LED1 | LED verte | 3mm | THT |
| 2 | J1,J2 | Header | 1x6 male 2.54mm | THT |

### Pneumatique / Externe

| Qty | Composant | Reference |
|-----|-----------|-----------|
| 1 | Pressostat | SMC ISE30A-01-N |
| 1 | Electrovanne | MAC 35A 12V |
| 1 | Regulateur | SMC IR1000-01BG |
| 1 | Bouton | Toyota 22x22mm LED |
| 1 | Te laiton | 2x push-in 6mm + 1/8" BSP |
| 1 | PRV | Soupape 3 bar preset |

---

## Reglages ISE30A v4.10

| Parametre | Valeur | Unite |
|-----------|--------|-------|
| P1 (SET) | 20.0 | kPa |
| HYST | 5.0 | kPa |
| OUT1 | NPN | - |
| FUNC | P1-HYS | - |

---

## Resume Modifications v4.9 -> v4.10

| Element | v4.9 | v4.10 |
|---------|------|-------|
| Tempo fermeture | Non | **2 secondes** |
| Composants tempo | - | D4 + R12 + C13 |
| Seuil ISE30A | 2.9-3.6 PSI | **2.9 PSI (20 kPa)** |
| Hysteresis | >=0.7 PSI | **0.7 PSI (5 kPa)** |
| Anti-flutter | Non | **Oui** |

---

**--- FIN BOM v4.10 ---**
