# Procédures de Validation

## 1. Validation Pressostat

### 1.1 Réglages Cibles

| Paramètre | Valeur | Unité |
|-----------|--------|-------|
| P_ON (seuil activation) | 2,9 - 3,6 | psi |
| P_ON (seuil activation) | 0,20 - 0,25 | bar |
| P_OFF (seuil désactivation) | 2,0 - 2,9 | psi |
| P_OFF (seuil désactivation) | 0,14 - 0,20 | bar |
| Hystérésis minimum | ≥ 0,7 | psi |
| Hystérésis minimum | ≥ 0,05 | bar |
| Anti-rebond | 100 | ms |

### 1.2 Réglage Optimal Recommandé

```
P_ON  = 3,0 psi (0,207 bar)
P_OFF = 2,2 psi (0,152 bar)
Hystérésis = 0,8 psi ✅
```

### 1.3 Test Statique avec Pompe à Main

**Équipement requis :**
- Pompe à main avec manomètre
- Multimètre

**Procédure :**

1. **Connecter** la pompe à main à l'entrée du pressostat
2. **Mesurer** la continuité entre les fils de sortie (multimètre mode continuité)
3. **Augmenter** progressivement la pression
4. **Noter** la pression de basculement (P_ON) → doit être 2,9-3,6 psi
5. **Diminuer** progressivement la pression
6. **Noter** la pression de retour (P_OFF) → doit être 2,0-2,9 psi
7. **Calculer** l'hystérésis : P_ON - P_OFF ≥ 0,7 psi

**Critères de validation :**
- [ ] P_ON dans la plage 2,9-3,6 psi
- [ ] P_OFF dans la plage 2,0-2,9 psi
- [ ] Hystérésis ≥ 0,7 psi
- [ ] Pas de fuite d'air au pressostat

---

## 2. Validation Circuit Électrique

### 2.1 Test Alimentation

| Point de mesure | Valeur attendue | Tolérance |
|-----------------|-----------------|-----------|
| +12V_BATT (avant fusible) | 12,0 - 14,4 V | - |
| +12V_IN (après fusible 5A) | 12,0 - 14,4 V | - |
| +12V_PROT (après protections) | 11,5 - 14,0 V | -0,5V max |
| +5V_LOG (sortie 78L05) | 5,0 V | ±0,25 V |

### 2.2 Test Relais Principal (G5Q-1)

**Procédure :**
1. **Contact coupé** : mesurer résistance bobine → ~360 Ω
2. **Appliquer 12V** sur la bobine
3. **Vérifier** le claquement audible
4. **Mesurer** continuité COM-NO → doit être fermé

### 2.3 Test Driver BTS5090

| Condition | IN (Pin 2) | OUT (Pin 6-7-8) |
|-----------|------------|-----------------|
| Repos | 0V | 0V |
| Actif | 5V | ~12V |

### 2.4 Test Bouton 3 Positions

| Position | Fonction | LED | Valve |
|----------|----------|-----|-------|
| 1 - FERMÉ | Circuit ouvert | Éteinte | Fermée |
| 2 - AUTO | Via pressostat | Selon boost | Automatique |
| 3 - OUVERT | Direct | Allumée | Ouverte |

---

## 3. Validation Pneumatique

### 3.1 Test Étanchéité

**Procédure :**
1. **Boucher** la sortie vers l'actionneur
2. **Pressuriser** le circuit à 60 psi
3. **Attendre** 5 minutes
4. **Mesurer** la chute de pression

**Critère :** Chute < 2 psi en 5 min

### 3.2 Test MAC 35A

| État | Port P | Port A | Port E |
|------|--------|--------|--------|
| Repos (non alimenté) | Bloqué | → E | Ouvert |
| Actif (12V) | → A | Alimenté | Bloqué |

**Temps de réponse typiques :**
- Ouverture : ~6 ms
- Fermeture : ~2 ms

### 3.3 Test Circuit Complet

1. **Mode FERMÉ** : Valve doit rester fermée
2. **Mode AUTO** : 
   - Sans boost : Valve fermée
   - Avec boost > 3 psi : Valve ouverte
3. **Mode OUVERT** : Valve toujours ouverte

---

## 4. Validation Contre-Pression

### 4.1 Mesure en Charge

**Équipement :**
- Manomètre 0-15 psi
- Raccord sur sortie turbine

**Procédure :**
1. Moteur à température
2. Valve en mode AUTO
3. Accélération en charge (2500-4000 rpm)
4. Mesurer contre-pression turbine-out

### 4.2 Critères

| Résultat | Contre-pression | Action |
|----------|-----------------|--------|
| ✅ IDÉAL | < 1,5 psi | RAS |
| ⚠️ ACCEPTABLE | 1,5 - 3 psi | Surveiller |
| ❌ NON CONFORME | > 3 psi | Investiguer |

---

## 5. Validation Optionnelle

### 5.1 Pression Après Plunger

| Régime | Pression attendue |
|--------|-------------------|
| Ralenti chaud | 15 - 30 psi |
| 2000 rpm | ~25 - 35 psi |
| 3000 rpm | ~35 - 40 psi |
| Haut régime | 40 - 45 psi |

### 5.2 Pression Huile Moteur

| Condition | Pression |
|-----------|----------|
| Ralenti chaud | 25 - 35 psi |
| 3000 rpm | 45 - 65 psi |

---

## 6. Checklist Validation Finale

### Circuit Électrique
- [ ] Tension +12V_PROT correcte
- [ ] Tension +5V_LOG = 5,0V ±0,25V
- [ ] Relais claque correctement
- [ ] Driver BTS5090 commute
- [ ] Bouton 3 positions fonctionne
- [ ] LEDs bouton OK

### Circuit Pneumatique
- [ ] Étanchéité OK (< 2 psi / 5 min)
- [ ] MAC 35A commute correctement
- [ ] Temps réponse < 15 ms
- [ ] Silencieux installé (pointe bas)

### Pressostat
- [ ] P_ON = 2,9 - 3,6 psi
- [ ] P_OFF = 2,0 - 2,9 psi
- [ ] Hystérésis ≥ 0,7 psi

### Fonctionnement Global
- [ ] Mode FERMÉ : valve fermée
- [ ] Mode AUTO : valve automatique selon boost
- [ ] Mode OUVERT : valve ouverte
- [ ] Contre-pression < 3 psi en charge

---

## 7. Fiche de Test

```
═══════════════════════════════════════════════════════
         FICHE DE VALIDATION - CIRCUIT VALVE v4.1
═══════════════════════════════════════════════════════

Date : _______________
Technicien : _______________
Véhicule : Mustang EcoBoost 2016 Stage 4+ E85
Kilométrage : _______________

MESURES ÉLECTRIQUES
───────────────────
+12V_BATT    : _______ V   [12,0-14,4V] □ OK □ NOK
+12V_PROT    : _______ V   [11,5-14,0V] □ OK □ NOK
+5V_LOG      : _______ V   [4,75-5,25V] □ OK □ NOK
R bobine     : _______ Ω   [~360Ω]      □ OK □ NOK

MESURES PRESSOSTAT
──────────────────
P_ON         : _______ psi [2,9-3,6]    □ OK □ NOK
P_OFF        : _______ psi [2,0-2,9]    □ OK □ NOK
Hystérésis   : _______ psi [≥0,7]       □ OK □ NOK

TESTS FONCTIONNELS
──────────────────
Mode FERMÉ   : Valve fermée             □ OK □ NOK
Mode AUTO    : Commutation auto         □ OK □ NOK
Mode OUVERT  : Valve ouverte            □ OK □ NOK
Étanchéité   : Chute ___ psi/5min [<2]  □ OK □ NOK

MESURES EN CHARGE
─────────────────
Contre-pression : _______ psi [<3]      □ OK □ NOK

RÉSULTAT GLOBAL : □ VALIDÉ  □ NON VALIDÉ

Observations :
_________________________________________________
_________________________________________________
_________________________________________________

Signature : _______________
═══════════════════════════════════════════════════════
```
