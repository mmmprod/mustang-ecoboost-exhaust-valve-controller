# BOM VALVE v4.6 - Composants THT Stripboard

**Date:** Decembre 2025
**Projet:** Mustang EcoBoost Exhaust Valve Controller

---

## MODIFICATIONS v4.6 vs v4.5

| Action | Composant | v4.5 | v4.6 |
|--------|-----------|------|------|
| REMPLACER | TVS frontale | SM5S22A (SMD) | P6KE18CA (THT) |
| SUPPRIMER | TVS interne | 5KP18CA | - |
| SUPPRIMER | MOV 1 | S20K20 | - |
| SUPPRIMER | MOV 2 | 14mm 26V | - |
| REMPLACER | Ferrite | BLM31PG221 (1206) | Wurth 742792093 (Axial) |

**Economie:** 3 composants en moins, BOM simplifiee

---

## SEMICONDUCTEURS

| Qty | Reference | Composant | Valeur | Package | Fournisseur |
|-----|-----------|-----------|--------|---------|-------------|
| 1 | D_TVS | TVS unidirectionnelle | P6KE18CA 18V 600W | DO-15 Axial | TME/Mouser |
| 1 | D1 | Schottky | 1N5822 40V 3A | DO-201AD Axial | TME |
| 1 | D2 | Zener | 1N4733A 5.1V 1W | DO-41 Axial | TME |
| 2 | D3-D4 | Signal | 1N4148 | DO-35 Axial | TME |
| 1 | D5 | Schottky relais | 1N5819 40V 1A | DO-41 Axial | TME |

## CIRCUITS INTEGRES

| Qty | Reference | Composant | Valeur | Package | Fournisseur |
|-----|-----------|-----------|--------|---------|-------------|
| 1 | U1 | Regulateur 5V | 78L05 / LM78L05 | TO-92 | TME |
| 1 | U2 | Buffer Schmitt | 74HC14 | DIP-14 | TME |
| 1 | U3 | Flip-flop D | CD4013 | DIP-14 | TME |
| 1 | U4 | High-side driver | BTS5090-1EJA | PG-TDSO-8 | Mouser |

## PASSIFS - RESISTANCES

| Qty | Reference | Valeur | Tolerance | Puissance | Package |
|-----|-----------|--------|-----------|-----------|---------|
| 2 | R1-R2 | 10k | 5% | 1/4W | Axial MF |
| 1 | R3 | 1k | 5% | 1/4W | Axial MF |
| 2 | R4-R5 | 100k | 5% | 1/4W | Axial MF |
| 1 | R6 | 4.7k | 5% | 1/4W | Axial MF |
| 1 | R7 | 47k | 5% | 1/4W | Axial MF |
| 2 | R8-R9 | 820 | 5% | 1/2W | Axial MF |
| 1 | R20 | 1k | 5% | 1/4W | Axial MF |

## PASSIFS - CONDENSATEURS

| Qty | Reference | Valeur | Tension | Type | Package |
|-----|-----------|--------|---------|------|---------|
| 6 | C1-C6 | 100nF | 50V | X7R ceramique | Radial 5mm |
| 1 | C7 | 100uF | 50V | Low-ESR electro | Radial 8mm |
| 1 | C8 | 22uF | 50V | Low-ESR electro | Radial 5mm |
| 2 | C9-C10 | 10uF | 25V | Low-ESR electro | Radial 5mm |
| 1 | C11 | 47uF | 25V | Low-ESR electro | Radial 6mm |
| 1 | C12 | 1uF | 16V | X7R ceramique | Radial 5mm |

## INDUCTANCES / FERRITES

| Qty | Reference | Composant | Valeur | Package |
|-----|-----------|-----------|--------|---------|
| 1 | FB1 | Ferrite EMI | Wurth 742792093 (90R@100MHz) | Axial |

## ELECTROMECANIQUE

| Qty | Reference | Composant | Valeur | Package |
|-----|-----------|-----------|--------|---------|
| 1 | K1 | Relais SPDT | Omron G5Q-1-HA-DC12-TY | PCB |
| 1 | F1 | Fusible | 1A T Fast-blow | Verre 5x20mm |
| 1 | F2 | Fusible | 1A ATO mini-blade | ATO |
| 1 | SW1 | Bouton TEST | Tactile NO | 6x6mm THT |
| 1 | LED1 | LED verte | Standard 3mm | 3mm THT |
| 2 | J1-J2 | Header male | 1x6 pins pas 2.54mm | THT |

## PNEUMATIQUE / EXTERNE

| Qty | Composant | Reference | Notes |
|-----|-----------|-----------|-------|
| 1 | Pressostat | SMC ISE30A-01-N | NPN 12-24V |
| 1 | Electrovanne | MAC 35A 12V | 1/8 NPT, 5.4W |
| 1 | Regulateur pression | SMC IR1000-01BG | 1 bar output |
| 1 | Bouton Toyota | 22x22mm LED | JST-XH 5P |

---

## NOTES COMMANDE

### TME (recommande)

```
P6KE18CA - 1 pcs
1N5822 - 1 pcs
1N4733A - 1 pcs
1N4148 - 5 pcs (marge)
1N5819 - 2 pcs (marge)
78L05 - 2 pcs (marge)
74HC14 DIP-14 - 1 pcs
CD4013 DIP-14 - 1 pcs
Wurth 742792093 - 2 pcs (marge)
```

### Mouser/DigiKey

```
BTS5090-1EJA (Infineon BTS50901EJAXUMA1) - 1 pcs
```

### SMC (distributeur pneumatique)

```
ISE30A-01-N - 1 pcs
IR1000-01BG - 1 pcs
```

---

## ORIENTATION CRITIQUE

**P6KE18CA:**
- Cathode (BAGUE/BAND) vers +12V_IN
- Anode vers GND_STAR
- Test multimetre: Rouge sur bague, Noir sur autre = OL (open)

**1N5822:**
- Cathode (BAGUE) vers +12V_PROT
- Anode vers fusible

**1N4733A (Zener):**
- Cathode (BAGUE) vers IN_A
- Anode vers GND

---

*BOM generee le 22 decembre 2025 - VALVE v4.6*
