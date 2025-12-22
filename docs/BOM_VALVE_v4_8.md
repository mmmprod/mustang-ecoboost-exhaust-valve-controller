# BOM VALVE v4.8

**Date:** Decembre 2025
**Projet:** Systeme Valve Echappement Automatique
**Vehicule:** Mustang EcoBoost 2016 Stage 4+ E85

---

## MODIFICATIONS v4.8 vs v4.7

| Ref | v4.7 | v4.8 | Raison |
|-----|------|------|--------|
| U1 | 78L05 | MCP1702-5002E/TO | Cold crank 6V OK |
| D_TVS | P6KE18CA 600W | 1.5KE18CA 1500W | Load-dump 400ms |
| PRV | - | Te + soupape 3 bar | Optionnel |

---

## SEMICONDUCTEURS

| Qty | Reference | Description | Fournisseur | Notes |
|-----|-----------|-------------|-------------|-------|
| 1 | 1.5KE18CA | TVS 18V 1500W DO-15 | TME/Mouser | Upgrade v4.8 |
| 1 | MCP1702-5002E/TO | LDO 5V 250mA TO-92 | TME/Mouser | Upgrade v4.8 |
| 1 | 1N5822 | Schottky 40V 3A DO-201AD | TME | |
| 1 | 1N4733A | Zener 5.1V 1W DO-41 | TME | |
| 2 | 1N4148 | Signal DO-35 | TME | |
| 1 | 1N5819 | Schottky 40V 1A DO-41 | TME | Roue libre relais |

---

## CIRCUITS INTEGRES

| Qty | Reference | Description | Fournisseur | Notes |
|-----|-----------|-------------|-------------|-------|
| 1 | 74HC14 | Schmitt trigger DIP-14 | TME | |
| 1 | CD4013 | Dual D flip-flop DIP-14 | TME | |
| 1 | BTS5090-1EJA | High-side driver PG-TDSO-8 | TME/Mouser | SMD |
| 1 | Aries 08-350000-11 | Adaptateur TSSOP-8 vers DIP-8 | Mouser/DigiKey | Pour BTS5090 |

---

## RESISTANCES (Axial MF THT)

| Qty | Valeur | Tolerance | Puissance | Notes |
|-----|--------|-----------|-----------|-------|
| 2 | 10k | 5% | 1/4W | Pull-up |
| 1 | 1k | 5% | 1/4W | Serie pressostat |
| 2 | 100k | 5% | 1/4W | Pull-down CD4013 |
| 1 | 4.7k | 5% | 1/4W | Serie BTS5090 |
| 1 | 47k | 5% | 1/4W | DEN BTS5090 |
| 2 | 820 | 5% | 1/2W | LED bouton Toyota |
| 1 | 470 | 5% | 1/4W | LED POWER |

---

## CONDENSATEURS

| Qty | Valeur | Tension | Type | Package |
|-----|--------|---------|------|---------|
| 6 | 100nF | 50V | X7R ceramique | Radial 5mm |
| 1 | 100uF | 50V | Low-ESR electro | Radial 8mm |
| 1 | 22uF | 50V | Low-ESR electro | Radial 5mm |
| 2 | 10uF | 25V | Low-ESR electro | Radial 5mm |
| 1 | 47uF | 25V | Low-ESR electro | Radial 6mm |
| 1 | 1uF | 16V | X7R ceramique | Radial 5mm |

---

## INDUCTANCES / FERRITES

| Qty | Reference | Description | Fournisseur |
|-----|-----------|-------------|-------------|
| 1 | Wurth 742792093 | Ferrite 90R@100MHz Axial | TME |

---

## ELECTROMECANIQUE

| Qty | Reference | Description | Fournisseur |
|-----|-----------|-------------|-------------|
| 1 | Omron G5Q-1-HA-DC12-TY | Relais SPDT 12V | TME |
| 1 | - | Fusible 1A T verre 5x20mm | TME |
| 1 | - | Fusible 1A ATO mini-blade | TME |
| 1 | - | Bouton tactile 6x6mm | TME |
| 1 | - | LED verte 3mm | TME |
| 2 | - | Header male 1x6 pas 2.54mm | TME |

---

## PNEUMATIQUE / EXTERNE

| Qty | Reference | Description | Fournisseur |
|-----|-----------|-------------|-------------|
| 1 | SMC ISE30A-01-N | Pressostat NPN 12-24V | SMC |
| 1 | MAC 35A 12V | Electrovanne 1/8 NPT 5.4W | MAC |
| 1 | SMC IR1000-01BG | Regulateur 1 bar | SMC |
| 1 | Toyota 22x22mm | Bouton LED JST-XH 5P | AliExpress |

---

## PRV OPTIONNELLE [NOUVEAU v4.8]

| Qty | Description | Reference | Fournisseur |
|-----|-------------|-----------|-------------|
| 1 | Te laiton 2x push-in 6mm + 1/8" BSP (F) | - | Pneumatique |
| 1 | Soupape surpression laiton 3 bar | - | Pneumatique |

---

## RESUME COMMANDE v4.8

**Composants critiques a commander:**

1. MCP1702-5002E/TO (remplace 78L05)
2. 1.5KE18CA (remplace P6KE18CA)
3. Aries 08-350000-11 (adaptateur BTS5090)

**Ne PAS commander:**

- 78L05 (remplace par MCP1702)
- P6KE18CA (remplace par 1.5KE18CA)
- 5KP18CA (supprime v4.6)
- MOV S20K20 (supprime v4.6)

---

**--- FIN BOM VALVE v4.8 ---**
