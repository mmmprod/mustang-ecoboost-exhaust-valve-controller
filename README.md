# Valve d'Échappement Automatique - Mustang EcoBoost 2016

## Circuit VALVE v4.2

Circuit pneumatique et électrique pour contrôle automatique de valve d'échappement.

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

## 🔴 CORRECTIONS CRITIQUES v4.2

| Correction | Problème résolu |
|------------|-----------------|
| ❌ **Check valve SUPPRIMÉE** | Ligne vacuum → pressostat piégeait la pression → valve bloquée ouverte |
| ✅ **Régulateur SMC IR1000-01BG** | Compresseur 90 PSI vs actionneur 7 PSI = ratio 13× → destruction membrane |
| ✅ **Gaine aluminisée** | Protection thermique 50cm section actionneur |

---

## Véhicule

- **Modèle** : Ford Mustang EcoBoost 2016
- **Configuration** : Stage 4+ E85 (600+ ch)
- **Équipement existant** : OPR V2, Plunger, Drain correct

## Objectif

Ouvrir automatiquement la valve d'échappement en charge pour éviter toute contre-pression, avec un mode manuel "toujours ouvert".

## Cibles Techniques

| Paramètre | Valeur cible | Acceptable |
|-----------|--------------|------------|
| Contre-pression turbine-out (valve ouverte) | < 1,5 psi | 1,5 - 3 psi |
| Pression huile moteur (ralenti chaud) | 25 - 35 psi | - |
| Pression entrée turbo après plunger (ralenti) | 15 - 30 psi | - |
| Pression entrée turbo après plunger (haut régime) | 40 - 45 psi | - |

---

## Schéma Pneumatique v4.2

### Ligne Détection (Pressostat)
```
❌ ZÉRO CHECK VALVE SUR CETTE LIGNE
Collecteur admission → Durite Ø6mm LIBRE → Passe-cloison → SMC ISE30A-01-N
```

### Ligne Commande (Actionnement)
```
Compresseur HS-551 (90-120 PSI)
         │
         ▼
      Té → Klaxon
         │
         ▼
  SMC IR1000-01BG (réglé 1 bar)  ← NOUVEAU v4.2
         │
         ▼
    MAC 35A port P(1)
         │
         ▼ Port A(2)
    Gaine aluminisée 50cm → Actionneur valve
```

---

## Documentation

### Circuit v4.2
- 🔴 [**Circuit VALVE v4.2**](docs/circuit-valve-v4.2.md) ← **DOCUMENT PRINCIPAL**
- 🛒 [**BOM v4.2**](bom/bill-of-materials.md) ← **Liste composants**

### Guides
- 📋 [Spécifications techniques](docs/specifications.md)
- ⚡ [Schéma électrique](docs/schema-electrique.md)
- 🔧 [Installation & cheminement](docs/installation-cheminement.md)
- ✅ [Procédures de validation](docs/validation.md)

---

## Composants Critiques v4.2

| Composant | Référence | Fonction |
|-----------|-----------|----------|
| Régulateur pression | **SMC IR1000-01BG** | 90 PSI → 1 bar |
| Pressostat | **SMC ISE30A-01-N** | Détection boost NPN |
| Électrovanne | **MAC 35A 3/2 NC** | Commande actionneur |
| Driver | **BTS5090-1EJAXUMA1** | Smart high-side switch |

**Coût estimé : ~430-610 €**

---

## Modes d'Utilisation

| Mode | Usage |
|------|-------|
| **Fermé** | Ville, autoroute (silencieux) |
| **Auto** | Ouverture dès 0,20-0,25 bar MAP |
| **Ouvert** | Bypass manuel permanent |

---

## License

MIT License

**Projet** : Circuit valve échappement automatique v4.2  
**Véhicule** : Mustang EcoBoost 2016 Stage 4+ E85 (600+ ch)  
**Auteur** : @mmmprod