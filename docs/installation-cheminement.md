# Guide d'Installation - Cheminement Câblage & Pneumatique

## Vue d'Ensemble du Routage

```
┌─────────────────────────────────────────────────────────────────────────────────┐
│                           MUSTANG ECOBOOST 2016                                  │
│                         VUE DE DESSUS - ROUTAGE                                  │
└─────────────────────────────────────────────────────────────────────────────────┘

    ┌─────────────┐                                           ┌─────────────┐
    │   MOTEUR    │                                           │   COFFRE    │
    │             │                                           │             │
    │  ┌───────┐  │                                           │  ┌───────┐  │
    │  │ TURBO │  │                                           │  │BOÎTIER│  │
    │  └───┬───┘  │                                           │  │CIRCUIT│  │
    │      │      │                                           │  └───────┘  │
    │  ┌───┴───┐  │         PASSAGE SOUS VÉHICULE             │      │      │
    │  │ MAP   │◄─┼─────────── DURITE VACUUM ─────────────────┼──────┘      │
    │  └───────┘  │         (check valve inclus)              │             │
    │             │                                           │  ┌───────┐  │
    │  ┌───────┐  │                                           │  │COMPRES│  │
    │  │COLLECT│  │                                           │  │-SEUR  │  │
    │  │ADMIS. │  │                                           │  └───┬───┘  │
    │  └───────┘  │                                           │      │      │
    │             │                                           │      │ AIR  │
    └─────────────┘                                           └──────┼──────┘
                                                                     │
    ┌─────────────┐                                                  │
    │  BATTERIE   │         CÂBLE 2,5mm² (ROUGE/NOIR)               │
    │     ±       │◄────────────────────────────────────────────────┼────────┐
    └─────────────┘                                                  │        │
                                                                     │        │
    ┌─────────────┐                                           ┌──────┴────────┴──┐
    │   VALVE     │         DURITE AIR Ø6mm                   │    BOÎTIER       │
    │ ÉCHAPPEMENT │◄──────────────────────────────────────────│    CIRCUIT       │
    │ (ARRIÈRE)   │                                           │                  │
    └─────────────┘                                           └──────────────────┘
```

---

## PARTIE 1 : Circuit Électrique

### 1.1 Alimentation Batterie → Boîtier

```
BATTERIE (+)
    │
    ├── Cosse M6/M8 (DIN 46228)
    │
    ▼
┌─────────────┐
│ FUSIBLE 5A  │  ← ≤15 cm de la borne batterie !
│ Porte-fus.  │
│   IP67      │
└──────┬──────┘
       │
       │  Câble 2,5mm² ROUGE (FLRY-B 105°C)
       │  Gaine tressée PET Ø8-10mm
       │
       │  CHEMINEMENT:
       │  1. Longer le passage de roue côté conducteur
       │  2. Passer sous les seuils de porte (gaines existantes)
       │  3. Entrer dans le coffre par passe-câble existant
       │
       ▼
┌─────────────────────────────────────────┐
│  BOÎTIER CIRCUIT                        │
│  Connecteur Deutsch DT 2 voies          │
│  Pin 1 = +12V_IN                        │
└─────────────────────────────────────────┘
```

### 1.2 Masse Batterie → Boîtier

```
BATTERIE (-)
    │
    ├── Cosse M6/M8 (DIN 46228)
    │
    │  Câble 2,5mm² NOIR (FLRY-B 105°C)
    │  Gaine tressée PET Ø8-10mm (même gaine que +12V)
    │
    │  CHEMINEMENT: Identique au +12V
    │
    ▼
┌─────────────────────────────────────────┐
│  BOÎTIER CIRCUIT                        │
│  Connecteur Deutsch DT 2 voies          │
│  Pin 2 = GND_IN                         │
└─────────────────────────────────────────┘
```

### 1.3 Points de Fixation Câbles Électriques

| N° | Position | Méthode | Notes |
|----|----------|---------|-------|
| 1 | Sortie batterie | Collier + gaine | Éviter chaleur moteur |
| 2 | Passage aile | Passe-câble caoutchouc | Étanchéité ! |
| 3 | Sous seuil conducteur | Colliers tous les 20cm | Utiliser gaines OEM |
| 4 | Sous seuil arrière | Colliers tous les 20cm | - |
| 5 | Entrée coffre | Passe-câble existant | Ne pas percer ! |

---

## PARTIE 2 : Circuit Pneumatique - Pression (Compresseur)

### 2.1 Compresseur → Boîtier → Valve

```
┌──────────────────┐
│   COMPRESSEUR    │
│   (dans coffre)  │
│                  │
│   Sortie 1/4 NPT │
└────────┬─────────┘
         │
         ▼
┌──────────────────┐
│   TÉ 1/4 NPT     │  ← Répartiteur
│   → 2× Ø6mm      │
└────────┬─────────┘
         │
    ┌────┴────┐
    │         │
    ▼         ▼
KLAXON    BOÎTIER
(existant) CIRCUIT
              │
              │  Tube Ø6mm polyuréthane
              │
              ▼
┌─────────────────────────────────────────┐
│  BOÎTIER CIRCUIT                        │
│                                         │
│  Passe-cloison push-in Ø6mm             │
│           │                             │
│           ▼                             │
│  ┌─────────────────┐                    │
│  │ CHECK VALVE     │  (optionnel)       │
│  │ sens: réservoir │                    │
│  │      → MAC      │                    │
│  └────────┬────────┘                    │
│           │                             │
│           ▼                             │
│  ┌─────────────────┐                    │
│  │   MAC 35A       │                    │
│  │   Port P (1)    │  ← Entrée pression │
│  └─────────────────┘                    │
└─────────────────────────────────────────┘
```

### 2.2 MAC 35A → Actionneur Valve

```
┌─────────────────────────────────────────┐
│  BOÎTIER CIRCUIT                        │
│                                         │
│  ┌─────────────────┐                    │
│  │   MAC 35A       │                    │
│  │   Port A (2)    │  ← Sortie actionneur
│  └────────┬────────┘                    │
│           │                             │
│           ▼                             │
│  Passe-cloison push-in Ø6mm             │
│                                         │
└───────────┬─────────────────────────────┘
            │
            │  Tube Ø6mm polyuréthane
            │
            │  CHEMINEMENT:
            │  1. Sortir du coffre par passe-câble
            │  2. Longer le dessous de caisse côté échappement
            │  3. Protéger avec gaine thermique près échappement
            │  4. Remonter vers actionneur valve
            │
            ▼
┌─────────────────────────────────────────┐
│  ACTIONNEUR VALVE ÉCHAPPEMENT           │
│  (sous le véhicule, arrière)            │
│                                         │
│  Raccord push-in Ø6mm                   │
└─────────────────────────────────────────┘
```

### 2.3 Évent MAC (Port E)

```
┌─────────────────────────────────────────┐
│  BOÎTIER CIRCUIT                        │
│                                         │
│  ┌─────────────────┐                    │
│  │   MAC 35A       │                    │
│  │   Port E (3)    │  ← Évent/Exhaust   │
│  └────────┬────────┘                    │
│           │                             │
│           ▼                             │
│  ┌─────────────────┐                    │
│  │  SILENCIEUX     │                    │
│  │  FRITTÉ         │                    │
│  │  (pointe bas)   │  ← IMPORTANT !     │
│  └─────────────────┘                    │
│                                         │
└─────────────────────────────────────────┘
```

---

## PARTIE 3 : Circuit Pneumatique - Vacuum (Pressostat)

### 3.1 Collecteur Admission → Pressostat

```
┌─────────────────────────────────────────┐
│  COMPARTIMENT MOTEUR                    │
│                                         │
│  ┌─────────────────┐                    │
│  │  COLLECTEUR     │                    │
│  │  ADMISSION      │                    │
│  │  (ligne MAP)    │                    │
│  └────────┬────────┘                    │
│           │                             │
│           │  Raccord en T sur ligne MAP │
│           │                             │
│           ▼                             │
│  ┌─────────────────┐                    │
│  │  CHECK VALVE    │                    │
│  │  (flèche vers   │                    │
│  │   coffre)       │                    │
│  └────────┬────────┘                    │
│           │                             │
└───────────┼─────────────────────────────┘
            │
            │  Durite silicone/caoutchouc Ø6mm
            │
            │  CHEMINEMENT:
            │  1. Passer côté passager (éviter chaleur échappement)
            │  2. Longer le tablier pare-feu
            │  3. Utiliser passe-câble existant vers habitacle
            │  4. Sous moquette côté passager
            │  5. Vers coffre via passage seuil
            │
            ▼
┌─────────────────────────────────────────┐
│  BOÎTIER CIRCUIT (COFFRE)               │
│                                         │
│  Passe-cloison push-in Ø6mm             │
│           │                             │
│           ▼                             │
│  ┌─────────────────┐                    │
│  │  PRESSOSTAT     │                    │
│  │  SMC ISE30A     │                    │
│  │  -01-N          │                    │
│  └─────────────────┘                    │
│                                         │
└─────────────────────────────────────────┘
```

---

## PARTIE 4 : Bouton de Commande (Habitacle)

### 4.1 Boîtier → Bouton Toyota

```
┌─────────────────────────────────────────┐
│  BOÎTIER CIRCUIT (COFFRE)               │
│                                         │
│  Connecteur GX12 5 pins                 │
│  ou Header JST-XH 5P                    │
│                                         │
└───────────┬─────────────────────────────┘
            │
            │  Faisceau 5 fils
            │  (longueur selon emplacement bouton)
            │
            │  CHEMINEMENT:
            │  1. Sortir du coffre sous la tablette arrière
            │  2. Longer le montant arrière (côté conducteur)
            │  3. Passer sous les seuils de porte
            │  4. Remonter vers console centrale ou tableau de bord
            │
            ▼
┌─────────────────────────────────────────┐
│  BOUTON TOYOTA 22×22mm                  │
│  (emplacement au choix)                 │
│                                         │
│  Options d'emplacement:                 │
│  • Emplacement vide console centrale    │
│  • Panneau interrupteurs tableau bord   │
│  • Support custom sous volant           │
└─────────────────────────────────────────┘
```

### 4.2 Câblage Bouton (Couleurs)

| Fil Bouton | Couleur | Fonction | Destination Boîtier |
|------------|---------|----------|---------------------|
| Pin 1 | VERT | +12V LED | +12V_PROT |
| Pin 2 | NOIR | GND LED | GND_STAR |
| Pin 3 | BLANC | GND LED | GND_STAR |
| Pin 4 | ROUGE | Contact + | BTN_RET |
| Pin 5 | JAUNE | Contact + | BTN_RET |

---

## PARTIE 5 : Schéma de Connexions Complet

```
══════════════════════════════════════════════════════════════════════════════════
                            SCHÉMA DE CONNEXIONS GLOBAL
══════════════════════════════════════════════════════════════════════════════════

COMPARTIMENT MOTEUR                    HABITACLE                     COFFRE
═══════════════════                    ═════════                     ══════

┌─────────────┐                                               ┌─────────────────┐
│  BATTERIE   │                                               │                 │
│     (+)     │═══ Câble 2,5mm² ROUGE ═══════════════════════►│ DEUTSCH DT Pin1 │
│     (-)     │═══ Câble 2,5mm² NOIR  ═══════════════════════►│ DEUTSCH DT Pin2 │
└─────────────┘                                               │                 │
                                                              │   ┌─────────┐   │
┌─────────────┐                                               │   │ BOÎTIER │   │
│ COLLECTEUR  │                                               │   │ CIRCUIT │   │
│  ADMISSION  │                                               │   │  v4.1   │   │
│  (MAP)      │═══ Durite Ø6mm + Check Valve ════════════════►│   └────┬────┘   │
└─────────────┘                                               │        │        │
                                                              │        │        │
                                                              │   ┌────┴────┐   │
                        ┌───────────┐                         │   │PRESSOSTAT│  │
                        │  BOUTON   │                         │   │SMC ISE30A│  │
                        │  TOYOTA   │◄═══ Faisceau 5 fils ════│   └─────────┘   │
                        │  22×22mm  │                         │                 │
                        └───────────┘                         │   ┌─────────┐   │
                                                              │   │ MAC 35A │   │
┌─────────────┐                                               │   └────┬────┘   │
│ COMPRESSEUR │═══ Tube Ø6mm ════════════════════════════════►│        │        │
│   (coffre)  │                                               │        │Port P  │
└─────────────┘                                               └────────┼────────┘
                                                                       │
                                                                       │Port A
SOUS VÉHICULE                                                          │
══════════════                                                         │
                                                                       │
┌─────────────────┐                                                    │
│   ACTIONNEUR    │◄═══ Tube Ø6mm (protégé gaine thermique) ═══════════┘
│     VALVE       │
│  ÉCHAPPEMENT    │
└─────────────────┘
```

---

## PARTIE 6 : Liste des Passages & Passe-Câbles

### 6.1 Passages Utilisés

| N° | Passage | Type | Utilisation |
|----|---------|------|-------------|
| 1 | Passe-câble tablier (existant) | Caoutchouc OEM | Durite vacuum vers coffre |
| 2 | Passage seuil conducteur | Gaine OEM | Câbles batterie |
| 3 | Passage seuil passager | Gaine OEM | Durite vacuum |
| 4 | Passe-câble coffre (existant) | Caoutchouc OEM | Tous câbles/durites coffre |
| 5 | Passage plancher arrière | À créer ou existant | Tube air vers actionneur |

### 6.2 Passe-Câbles à Installer

| Composant | Quantité | Emplacement | Notes |
|-----------|----------|-------------|-------|
| Passe-cloison push-in Ø6mm | 3 | Boîtier circuit | Air P, Air A, Vacuum |
| Joint caoutchouc | Si nécessaire | Passages existants | Étanchéité |

---

## PARTIE 7 : Points de Fixation

### 7.1 Fixations Câbles Électriques

```
BATTERIE ──●──────●──────●──────●──────●──────●──────● BOÎTIER
           │      │      │      │      │      │      │
          15cm   20cm   20cm   20cm   20cm   20cm   Entrée
           │      │      │      │      │      │
         Collier Collier Collier Collier Collier Passe-câble
```

### 7.2 Fixations Durites Air

- **Tube pression (Compresseur → MAC → Actionneur)** : Colliers tous les 30cm
- **Tube vacuum** : Colliers tous les 30cm, protection thermique si proche échappement
- **Zone chaude** : Gaine thermique aluminium obligatoire

---

## PARTIE 8 : Distances & Longueurs

| Élément | Longueur estimée | Notes |
|---------|------------------|-------|
| Câble batterie (+) | ~4-5 m | Selon passage choisi |
| Câble batterie (-) | ~4-5 m | Même chemin que + |
| Durite vacuum (MAP → coffre) | ~5-6 m | Passage côté passager |
| Tube air (compresseur → boîtier) | ~0,5 m | Dans le coffre |
| Tube air (boîtier → actionneur) | ~3-4 m | Sous véhicule |
| Faisceau bouton | ~3-4 m | Coffre → habitacle |

---

## PARTIE 9 : Checklist Installation

### Avant de Commencer
- [ ] Débrancher batterie (-)
- [ ] Repérer tous les passages de câbles existants
- [ ] Vérifier accès à l'actionneur de valve
- [ ] Préparer outils : pinces, colliers, gaine, passe-câbles

### Circuit Électrique
- [ ] Câble + batterie passé et fixé
- [ ] Câble - batterie passé et fixé
- [ ] Fusible 5A installé ≤15cm de batterie
- [ ] Gaine de protection installée
- [ ] Connexion boîtier OK (Deutsch DT)

### Circuit Vacuum (Pressostat)
- [ ] Raccord T sur ligne MAP installé
- [ ] Check valve installé (flèche vers coffre)
- [ ] Durite passée et fixée
- [ ] Connexion pressostat OK

### Circuit Air (Commande Valve)
- [ ] Té compresseur installé
- [ ] Tube vers boîtier connecté (Port P)
- [ ] Tube vers actionneur passé et fixé
- [ ] Gaine thermique sur zones chaudes
- [ ] Connexion actionneur OK

### Bouton Habitacle
- [ ] Emplacement choisi et percé si nécessaire
- [ ] Faisceau 5 fils passé
- [ ] Bouton installé et connecté
- [ ] Test LED OK

### Tests Finaux
- [ ] Rebrancher batterie
- [ ] Test pressostat (pompe à main)
- [ ] Test modes : Fermé / Auto / Manuel
- [ ] Test étanchéité pneumatique
- [ ] Vérification valve : ouvre/ferme correctement

---

## PARTIE 10 : Conseils & Astuces

### Passage des Câbles
1. **Utiliser les passages OEM** autant que possible
2. **Éviter les zones chaudes** : échappement, turbo, collecteur
3. **Protéger** avec gaine tressée ou gaine thermique
4. **Fixer régulièrement** : tous les 15-20cm

### Points Critiques
⚠️ **Fusible batterie** : ≤15cm de la borne, accessible  
⚠️ **Check valve vacuum** : sens correct (flèche vers coffre)  
⚠️ **Zone échappement** : gaine thermique aluminium obligatoire  
⚠️ **Étanchéité** : vérifier tous les passe-câbles  

### Outils Recommandés
- Pince à dénuder
- Pince à sertir (cosses)
- Colliers plastique 150mm (pack de 100)
- Gaine tressée PET Ø8-10mm (5m)
- Gaine thermique aluminium (1m)
- Lubrifiant silicone (passage câbles)
