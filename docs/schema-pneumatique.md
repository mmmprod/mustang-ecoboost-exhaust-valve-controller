# Schéma Pneumatique v4.2

## Circuit VALVE - Mustang EcoBoost 2016

**Version :** 4.2  
**Date :** Décembre 2025  
**Origine :** Audit technique Gemini — Corrections critiques intégrées

---

## 🔴 CORRECTIONS CRITIQUES v4.2

| Correction | Problème résolu | Impact |
|------------|-----------------|--------|
| ❌ **Check valve SUPPRIMÉE** | Ligne vacuum → pressostat piégeait la pression | Valve bloquée ouverte après accélération |
| ✅ **Régulateur SMC IR1000-01BG AJOUTÉ** | Compresseur 90 PSI vs actionneur 7 PSI = ratio 13× | Destruction membrane actionneur |
| ✅ **Gaine aluminisée recommandée** | Protection thermique durite près collecteur | Durabilité durite |

---

## Vue d'Ensemble du Système

Le circuit pneumatique se divise en deux lignes distinctes :

1. **Ligne Détection (Vacuum)** : Collecteur admission → Pressostat (mesure boost)
2. **Ligne Commande (Pression)** : Compresseur → Régulateur → MAC 35A → Actionneur valve

---

## LIGNE DÉTECTION — Mesure Boost (Vacuum)

### ❌ CORRECTION CRITIQUE : ZÉRO CHECK VALVE

```
┌─────────────────────────────────────────────────────────────────┐
│  ⚠️ AUCUNE CHECK VALVE SUR CETTE LIGNE                          │
├─────────────────────────────────────────────────────────────────┤
│  Problème corrigé v4.2:                                         │
│  Une check valve piégeait la pression positive après            │
│  accélération → pressostat restait ON → valve ne se refermait   │
│  jamais → backpressure permanente.                              │
│                                                                 │
│  Solution: Durite LIBRE sans restriction.                       │
└─────────────────────────────────────────────────────────────────┘
```

### Schéma ligne détection

```
┌─────────────────────────────────────────┐
│  COMPARTIMENT MOTEUR                    │
│                                         │
│  ┌─────────────────┐                    │
│  │  COLLECTEUR     │                    │
│  │  ADMISSION      │                    │
│  │  (Ligne MAP)    │                    │
│  └────────┬────────┘                    │
│           │                             │
│           │  Raccord T                  │
│           │  (existant)                 │
│           │                             │
└───────────┼─────────────────────────────┘
            │
            │  Durite Ø6mm LIBRE
            │  (silicone/caoutchouc résistant vacuum)
            │
            │  CHEMINEMENT:
            │  1. Passer côté passager (éviter chaleur)
            │  2. Longer tablier pare-feu
            │  3. Passe-câble existant vers habitacle
            │  4. Sous moquette côté passager
            │  5. Vers coffre via passage seuil
            │
            ▼
┌─────────────────────────────────────────┐
│  COFFRE - BOÎTIER CIRCUIT               │
│                                         │
│  Passe-cloison push-in Ø6mm             │
│           │                             │
│           ▼                             │
│  ┌─────────────────┐                    │
│  │  PRESSOSTAT     │                    │
│  │  SMC ISE30A-01-N│                    │
│  └─────────────────┘                    │
│                                         │
└─────────────────────────────────────────┘
```

### Composants ligne détection

| Composant | Référence | Specs | Note |
|-----------|-----------|-------|------|
| Pressostat | SMC ISE30A-01-N | -0,1 à 1 MPa, NPN 12-24V | Réglé P_ON=0,20-0,25 bar |
| Durite vacuum | Ø6mm silicone/caoutchouc | Résistant vacuum | 5-6m longueur |
| Raccord T | Existing sur ligne MAP | Brass/nylon | Déjà présent |
| Passe-cloison | Push-in Ø6mm | IP65 | Boîtier coffre |

### ⚠️ À NE PAS FAIRE

- ❌ **Ne PAS installer de check valve** sur cette ligne
- ❌ **Ne PAS utiliser de régulateur de pression** sur cette ligne
- ❌ **Ne PAS utiliser de réservoir tampon** sur cette ligne

La ligne doit être **LIBRE** pour permettre le retour rapide à la pression atmosphérique.

---

## LIGNE COMMANDE — Actionnement Valve (Pression)

### ✅ NOUVEAU v4.2 : RÉGULATEUR SMC IR1000-01BG

```
┌─────────────────────────────────────────────────────────────────┐
│  ✅ RÉGULATEUR DE PRESSION OBLIGATOIRE                          │
├─────────────────────────────────────────────────────────────────┤
│  Problème corrigé v4.2:                                         │
│  Compresseur HS-551 délivre 90-120 PSI (6-8 bar).               │
│  Actionneur valve s'ouvre à 7 PSI (0,5 bar).                    │
│  Ratio 13× → destruction membrane sans régulateur.              │
│                                                                 │
│  Solution: Régulateur SMC IR1000-01BG réglé à 1 bar (15 PSI).   │
└─────────────────────────────────────────────────────────────────┘
```

### Schéma ligne commande

```
┌─────────────────────────────────────────┐
│  COFFRE                                 │
│                                         │
│  ┌─────────────────┐                    │
│  │  COMPRESSEUR    │                    │
│  │  HS-551         │                    │
│  │  90-120 PSI     │                    │
│  │  Sortie 1/4 NPT │                    │
│  └────────┬────────┘                    │
│           │                             │
│           ▼                             │
│  ┌─────────────────┐                    │
│  │   TÉ 1/4 NPT    │                    │
│  │   → 2× Ø6mm     │                    │
│  └────────┬────────┘                    │
│           │                             │
│      ┌────┴────┐                        │
│      │         │                        │
│      ▼         ▼                        │
│   KLAXON    Tube Ø6mm                   │
│  (existant)    │                        │
│                │                        │
│                ▼                        │
│  Raccord Ø6mm → 1/8" mâle               │
│  (SMC KQ2H06-01)                        │
│                │                        │
│                ▼                        │
│  ┌─────────────────────────────┐        │
│  │  SMC IR1000-01BG            │        │
│  │  Régulateur pression         │        │
│  │  Réglé: 0,1 MPa (1 bar)     │        │
│  │  Manomètre inclus            │        │
│  └─────────────────────────────┘        │
│                │                        │
│                ▼                        │
│  Raccord 1/8" mâle → Ø6mm               │
│  (SMC KQ2H06-01)                        │
│                │                        │
│                ▼                        │
│  Passe-cloison push-in Ø6mm             │
│                │                        │
│                ▼                        │
│  ┌─────────────────────────────┐        │
│  │  BOÎTIER CIRCUIT            │        │
│  │                             │        │
│  │  ┌─────────────┐            │        │
│  │  │  MAC 35A    │            │        │
│  │  │  3/2 NC     │            │        │
│  │  │  12V        │            │        │
│  │  └─────────────┘            │        │
│  │       │                     │        │
│  │  Port P(1) ← Pression in    │        │
│  │  Port A(2) → Actionneur     │        │
│  │  Port E(3) → Évent          │        │
│  │       │                     │        │
│  │       ▼                     │        │
│  │  Silencieux fritté          │        │
│  │  (pointe vers bas)          │        │
│  │                             │        │
│  └─────────────┬───────────────┘        │
│                │                        │
│           Port A(2)                     │
│                │                        │
│                ▼                        │
│  Passe-cloison push-in Ø6mm             │
│                │                        │
└────────────────┼─────────────────────────┘
                 │
                 │  Tube Ø6mm polyuréthane
                 │  + Gaine aluminisée 50cm
                 │
                 │  CHEMINEMENT:
                 │  1. Sortir coffre par passe-câble
                 │  2. Longer dessous caisse
                 │  3. Gaine aluminisée près échappement
                 │  4. Remonter vers actionneur
                 │
                 ▼
┌─────────────────────────────────────────┐
│  SOUS VÉHICULE - ARRIÈRE                │
│                                         │
│  ┌─────────────────┐                    │
│  │  ACTIONNEUR     │                    │
│  │  VALVE          │                    │
│  │  ÉCHAPPEMENT    │                    │
│  │                 │                    │
│  │  Raccord Ø6mm   │                    │
│  │  Seuil: 7 PSI   │                    │
│  └─────────────────┘                    │
│                                         │
└─────────────────────────────────────────┘
```

### Composants ligne commande

| Composant | Référence | Specs | Note |
|-----------|-----------|-------|------|
| Compresseur | HS-551 | 90-120 PSI, 1/4 NPT | Déjà installé |
| Té répartiteur | 1/4 NPT → 2× Ø6mm | Brass | Klaxon + régulateur |
| **Régulateur** | **SMC IR1000-01BG** | **0,005-0,2 MPa, 1/8"** | **NOUVEAU v4.2** |
| Adaptateurs | SMC KQ2H06-01 | 1/8" mâle → Ø6mm | 2× requis |
| Électrovanne | MAC 35A 3/2 NC | 12V, 5,4W, 1/8 NPT | Commande actionneur |
| Silencieux | Fritté 1/8 NPT | - | Port E(3) MAC |
| Tube pression | Polyuréthane Ø6mm | 10 bar | Compresseur → Actionneur |
| **Gaine thermique** | **Aluminisée Ø10-12mm** | **50cm** | **Section actionneur** |
| Passe-cloisons | Push-in Ø6mm | IP65 | 2× boîtier |

---

## Régulateur SMC IR1000-01BG — Spécifications

### Données techniques

| Paramètre | Valeur |
|-----------|--------|
| Série | IR1000 (compact) |
| Plage réglage | 0,005-0,2 MPa (0,05-2 bar) |
| Pression entrée max | 1,0 MPa (10 bar) ✅ Compatible HS-551 |
| Pression sortie réglée | **0,1 MPa (1 bar = 15 PSI)** |
| Filetage | Rc 1/8" (BSP) |
| Manomètre | Inclus (code G) |
| Support fixation | Inclus (code B) |
| Température | -5 à 60°C |

### Pourquoi 1 bar (15 PSI) ?

| Paramètre | Valeur | Raison |
|-----------|--------|--------|
| Pression ouverture actionneur | ~7 PSI (0,5 bar) | Seuil actionneur |
| Pression réglée régulateur | 15 PSI (1 bar) | 2× seuil = marge sécurité |
| Pression max compresseur | 120 PSI (8 bar) | Ratio 8× sans régulateur ! |

Sans régulateur : **Destruction membrane actionneur garantie**

### Installation régulateur

```
1. Filetages 1/8" : Appliquer ruban PTFE (2-3 tours)
2. Visser adaptateur SMC KQ2H06-01 côté entrée (IN)
3. Visser adaptateur SMC KQ2H06-01 côté sortie (OUT)
4. Connecter tube Ø6mm depuis Té compresseur → IN
5. Connecter tube Ø6mm vers passe-cloison boîtier → OUT
6. Régler manomètre à 0,1 MPa (1 bar / 15 PSI)
7. Vérifier étanchéité au savon
8. Fixer régulateur avec support inclus (vis M4)
```

---

## Électrovanne MAC 35A — Fonctionnement

### Configuration 3/2 NC (Normalement Closed)

| État | Bobine | Ports | Fonction |
|------|--------|-------|----------|
| Repos | OFF (0V) | A→E, P bloqué | Purge actionneur → valve FERME |
| Énergisé | ON (12V) | P→A, E bloqué | Alimentation actionneur → valve OUVRE |

### Brochage pneumatique

```
        ┌─────────────────┐
        │    MAC 35A      │
        │     3/2 NC      │
        │                 │
   P(1) │ ●               │  Port P : Pression IN (depuis régulateur)
        │                 │
   A(2) │ ●               │  Port A : Actionneur OUT (vers valve)
        │                 │
   E(3) │ ●               │  Port E : Exhaust (purge air)
        │                 │
        └─────────────────┘
```

### Port E(3) — Évent avec silencieux

```
⚠️ ORIENTATION CRITIQUE

Port E(3) → Silencieux fritté 1/8 NPT
            │
            ▼ VERS LE BAS
          (évite condensation)
```

Le silencieux réduit le bruit de purge et empêche l'entrée de contaminants.

---

## Protection Thermique Durite

### Zone critique

La section de durite entre le passe-cloison sous le véhicule et l'actionneur passe à proximité du collecteur d'échappement.

### Solution v4.2

```
┌─────────────────────────────────────────────────────────────────┐
│  ✅ GAINE ALUMINISÉE RECOMMANDÉE                                │
├─────────────────────────────────────────────────────────────────┤
│  Section critique: 50cm avant actionneur                        │
│  Gaine: Aluminisée Ø10-12mm résistant 300°C                     │
│  Application: Entourer durite polyuréthane Ø6mm                 │
│  Fixation: Colliers tous les 15cm                               │
└─────────────────────────────────────────────────────────────────┘
```

| Paramètre | Valeur |
|-----------|--------|
| Longueur gaine | 50cm minimum |
| Diamètre gaine | Ø10-12mm (pour tube Ø6mm) |
| Température max | 300°C |
| Type | Aluminisée réfléchissante |
| Fixation | Colliers inox tous les 15cm |

---

## Réglages Pressostat SMC ISE30A-01-N

### Paramètres cibles

| Paramètre | Valeur PSI | Valeur bar | Fonction |
|-----------|------------|------------|----------|
| P_ON | 2,9-3,6 | 0,20-0,25 | Valve s'ouvre (boost détecté) |
| P_OFF | 2,0-2,9 | 0,14-0,20 | Valve se ferme (retour vacuum) |
| Hystérésis | ≥0,7 | ≥0,05 | Anti-oscillation |

### Procédure réglage

1. **Moteur arrêté** : Pressostat doit être OFF (pas de boost)
2. **Ralenti** : Pressostat doit être OFF (vacuum ~-0,5 bar)
3. **Accélération légère** : Pressostat passe ON à 0,20-0,25 bar MAP
4. **Décélération** : Pressostat passe OFF à 0,14-0,20 bar MAP
5. **Ajustement fin** : Potentiomètres P_ON et Hystérésis sur pressostat

---

## Pressions Système — Tableau Récapitulatif

| Point de mesure | Pression | Note |
|-----------------|----------|------|
| Compresseur sortie | 90-120 PSI (6-8 bar) | Réservoir HS-551 |
| Régulateur entrée | 90-120 PSI (6-8 bar) | Avant IR1000-01BG |
| **Régulateur sortie** | **15 PSI (1 bar)** | **Réglage v4.2** |
| MAC 35A port P | 15 PSI (1 bar) | Alimentation électrovanne |
| Actionneur valve | 7-15 PSI (0,5-1 bar) | Seuil ouverture ~7 PSI |
| Collecteur admission (ralenti) | -5 à -10 PSI (-0,3 à -0,7 bar) | Vacuum |
| Collecteur admission (boost) | 3-30 PSI (0,2-2 bar) | Pressostat ON à 3 PSI |

---

## ⚠️ POINTS CRITIQUES

### À FAIRE ABSOLUMENT

| Point | Action |
|-------|--------|
| ✅ Supprimer check valve | Ligne vacuum doit être LIBRE |
| ✅ Installer régulateur | SMC IR1000-01BG réglé 1 bar |
| ✅ Protection thermique | Gaine aluminisée 50cm |
| ✅ Orientation silencieux | Port E(3) MAC vers le bas |
| ✅ Étanchéité | Vérifier tous raccords au savon |

### À NE JAMAIS FAIRE

| Point | Raison |
|-------|--------|
| ❌ Check valve ligne vacuum | Piège pression → valve bloquée |
| ❌ Pression directe compresseur | Destruction membrane actionneur |
| ❌ Durite non protégée près échappement | Fusion polyuréthane |
| ❌ Silencieux vers le haut | Condensation dans MAC |
| ❌ Raccords non étanchés | Fuite pression → valve inopérante |

---

## Checklist Installation Pneumatique

### Ligne détection (vacuum)

- [ ] Raccord T sur ligne MAP installé
- [ ] ❌ **Vérifier AUCUNE check valve présente**
- [ ] Durite Ø6mm passée coffre → moteur
- [ ] Passe-cloison boîtier installé
- [ ] Connexion pressostat OK
- [ ] Test étanchéité vacuum OK

### Ligne commande (pression)

- [ ] Té compresseur installé (klaxon + régulateur)
- [ ] ✅ **Régulateur SMC IR1000-01BG installé**
- [ ] Adaptateurs 1/8" → Ø6mm installés
- [ ] Ruban PTFE appliqué sur filetages
- [ ] Tube Ø6mm compresseur → régulateur → MAC
- [ ] Passe-cloison boîtier installé
- [ ] Tube Ø6mm MAC → actionneur
- [ ] ✅ **Gaine aluminisée 50cm installée**
- [ ] Silencieux MAC port E(3) vers bas
- [ ] Réglage régulateur 1 bar (15 PSI)
- [ ] Test étanchéité pression au savon
- [ ] Connexion actionneur OK

### Tests fonctionnels

- [ ] Compresseur charge à 90-120 PSI
- [ ] Régulateur maintient 15 PSI ± 1
- [ ] Pressostat détecte boost à 0,20-0,25 bar
- [ ] Mode Auto : Valve s'ouvre en charge
- [ ] Mode Auto : Valve se ferme au ralenti
- [ ] Mode Manuel : Valve reste ouverte
- [ ] Aucune fuite détectable (savon)
- [ ] Temps réponse <20ms (au son)

---

**— FIN SCHÉMA PNEUMATIQUE v4.2 —**
