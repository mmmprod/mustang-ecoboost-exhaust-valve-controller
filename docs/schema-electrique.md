# Schéma Électrique

## Vue d'Ensemble

```
                                    ┌─────────────────────────────────────┐
                                    │         CIRCUIT PRINCIPAL           │
                                    └─────────────────────────────────────┘

    +12V ACC ──────┬────────────────────────────────────────────────────────────
    (après        │
    contact)      │
                  │
            ┌─────┴─────┐
            │  FUSIBLE  │
            │    5A     │
            └─────┬─────┘
                  │
            ┌─────┴─────┐
            │    TVS    │
            │1.5KE18CA  │────────┐
            └─────┬─────┘        │
                  │              │
                  │              │
    ┌─────────────┴──────────────┴─────────────────────────────────────────┐
    │                                                                       │
    │                         INTERRUPTEUR SPDT                            │
    │                          3 POSITIONS                                  │
    │                                                                       │
    │    Position 1        Position 2         Position 3                   │
    │     (FERMÉ)           (AUTO)            (OUVERT)                     │
    │        │                 │                  │                        │
    │        ○                 ○                  ○                        │
    │                          │                  │                        │
    │                    ┌─────┴─────┐            │                        │
    │                    │PRESSOSTAT │            │                        │
    │                    │  BOOST    │            │                        │
    │                    │   NO      │            │                        │
    │                    │0.20-0.25  │            │                        │
    │                    │   bar     │            │                        │
    │                    └─────┬─────┘            │                        │
    │                          │                  │                        │
    │                          └────────┬─────────┘                        │
    │                                   │                                   │
    └───────────────────────────────────┼───────────────────────────────────┘
                                        │
                              ┌─────────┴─────────┐
                              │      RELAIS       │
                              │    12V / 30A      │
                              │                   │
                              │  ┌───────────┐    │
                              │  │  BOBINE   │    │
                              │  │   12V     │────┼────┐
                              │  └───────────┘    │    │
                              │                   │    │
                              │  ┌───┐            │  ┌─┴──┐
                              │  │87 │────────────┼──│1N  │
                              │  └─┬─┘            │  │5819│
                              │    │              │  └─┬──┘
                              │  ┌─┴─┐            │    │
                              │  │30 │────────────┼────┘
                              │  └───┘            │
                              └───────────────────┘
                                        │
                                        │ 87 (sortie commutée)
                                        │
                              ┌─────────┴─────────┐
                              │   ÉLECTROVANNE    │
                              │       3/2         │
                              │                   │
                              │   Bobine 12V      │
                              └─────────┬─────────┘
                                        │
                                        │
    GND (Châssis) ──────────────────────┴──────────────────────────────────────
    (masse propre)
```

## Détail des Composants

### 1. Alimentation

| Composant | Valeur | Fonction |
|-----------|--------|----------|
| Source | +12V après contact | Alimentation uniquement moteur tournant |
| Fusible | 5A | Protection surintensité |
| TVS | 1.5KE18CA | Protection surtension transitoire |

### 2. Interrupteur SPDT 3 Positions

```
        ┌─────────────────────────────────────┐
        │                                     │
        │   ○ ─────── ○ ─────── ○            │
        │   │         │         │            │
        │  POS.1    POS.2     POS.3          │
        │  FERMÉ    AUTO      OUVERT         │
        │                                     │
        │  Aucune   Via       Direct         │
        │  action   Presso.   +12V           │
        │                                     │
        └─────────────────────────────────────┘
```

| Position | Nom | Action | Valve |
|----------|-----|--------|-------|
| 1 | FERMÉ | Circuit ouvert | Toujours fermée |
| 2 | AUTO | Via pressostat | Automatique selon boost |
| 3 | OUVERT | Direct +12V | Toujours ouverte |

### 3. Pressostat Boost

| Paramètre | Valeur |
|-----------|--------|
| Type | Normalement Ouvert (NO) |
| Seuil | 0,20 - 0,25 bar |
| Hystérésis | ≥ 0,05 bar |
| Raccordement | Ligne MAP |

**Fonctionnement :**
- Boost < 0,20 bar → Contact OUVERT → Relais OFF
- Boost ≥ 0,20 bar → Contact FERMÉ → Relais ON

### 4. Relais

| Paramètre | Valeur |
|-----------|--------|
| Tension bobine | 12V DC |
| Courant contacts | 30A max |
| Configuration | SPST-NO |

**Bornes :**
- 85 : Bobine + (commande)
- 86 : Bobine - (masse)
- 30 : Commun (alimentation)
- 87 : NO (sortie vers EV)

### 5. Diode de Protection

| Composant | Valeur | Fonction |
|-----------|--------|----------|
| Diode | 1N5819 (Schottky) | Protection contre surtension de la bobine relais |

**Montage :** En parallèle sur la bobine du relais, cathode côté +12V

### 6. Électrovanne

| Paramètre | Valeur |
|-----------|--------|
| Type | 3/2 voies |
| Tension | 12V DC |
| Temps réponse | ≤ 15 ms |
| Orifice | ≥ 2,5 mm |

## Câblage

### Sections de Câbles

| Circuit | Section | Couleur suggérée |
|---------|---------|------------------|
| +12V principal | 1,5 mm² | Rouge |
| Masse principale | 1,5 mm² | Noir |
| Commande pressostat | 1,0 mm² | Bleu |
| Commande interrupteur | 1,0 mm² | Jaune |
| Sortie relais vers EV | 1,5 mm² | Orange |

### Points de Connexion

| Point | Emplacement | Notes |
|-------|-------------|-------|
| +12V ACC | Boîte fusibles | Après contact uniquement |
| Masse | Châssis | Point propre, décapé |
| Pressostat | Ligne MAP | Raccord en T |

## Schéma de Câblage Simplifié

```
+12V ACC ─── FUSIBLE 5A ─── TVS ─┬─ INTER Pos.3 (OUVERT) ────────────┐
                                 │                                    │
                                 ├─ INTER Pos.2 (AUTO) ── PRESSO. ───┤
                                 │                                    │
                                 └─ INTER Pos.1 (FERMÉ) ─── X        │
                                                                      │
                                           ┌──────────────────────────┘
                                           │
                                           ▼
                                    ┌─────────────┐
                                    │   RELAIS    │
                                    │   BOBINE    │──── DIODE ────┐
                                    └──────┬──────┘               │
                                           │                      │
                                           │                      │
+12V ACC ── FUSIBLE ── RELAIS 30 ──────────┤                      │
                                           │                      │
                              RELAIS 87 ───┴──── ÉLECTROVANNE ────┴─── GND
```

## Protection et Sécurité

### Protection Surtension (TVS 1.5KE18CA)

- **Tension de travail** : 18V
- **Tension de claquage** : ~20V
- **Fonction** : Absorbe les pics de tension du circuit de charge

### Protection Relais (Diode 1N5819)

- **Type** : Schottky
- **Tension inverse** : 40V
- **Fonction** : Supprime la surtension à la coupure de la bobine

### Fusible

- **Valeur** : 5A
- **Fonction** : Protection contre court-circuit

## Test du Circuit

1. **Vérifier** la continuité de tous les câbles
2. **Mesurer** +12V au fusible (contact mis)
3. **Tester** chaque position de l'interrupteur
4. **Vérifier** le fonctionnement du pressostat avec pompe à main
5. **Contrôler** le claquement du relais
6. **Confirmer** l'activation de l'électrovanne
