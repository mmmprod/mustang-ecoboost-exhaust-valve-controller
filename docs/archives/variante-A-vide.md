# Variante A - Actionneur Fermé par Vide

## Principe de Fonctionnement

Cette variante utilise le vide du collecteur d'admission pour maintenir la valve fermée au repos. L'ouverture se fait par mise à l'atmosphère.

## Schéma de Principe

```
┌─────────────────────────────────────────────────────────────┐
│                    CIRCUIT PNEUMATIQUE                       │
├─────────────────────────────────────────────────────────────┤
│                                                              │
│  ┌──────────────┐                                           │
│  │  Collecteur  │                                           │
│  │  Admission   │                                           │
│  └──────┬───────┘                                           │
│         │                                                    │
│         ▼                                                    │
│  ┌──────────────┐                                           │
│  │   Clapet     │                                           │
│  │ Anti-Retour  │                                           │
│  └──────┬───────┘                                           │
│         │                                                    │
│         ▼                                                    │
│  ┌──────────────┐                                           │
│  │  Réservoir   │                                           │
│  │  0,3-0,5 L   │                                           │
│  └──────┬───────┘                                           │
│         │                                                    │
│         │ P (Pression/Vide)                                 │
│         ▼                                                    │
│  ┌──────────────┐      A (Actionneur)    ┌──────────────┐  │
│  │    EV 3/2    │─────────────────────────│  Actionneur  │  │
│  │     NC       │                         │    Valve     │  │
│  └──────┬───────┘                         └──────────────┘  │
│         │                                                    │
│         │ R (Évent)                                         │
│         ▼                                                    │
│  ┌──────────────┐                                           │
│  │    Filtre    │                                           │
│  │    Évent     │                                           │
│  └──────────────┘                                           │
│                                                              │
└─────────────────────────────────────────────────────────────┘
```

## Configuration Électrovanne

| Port | Raccordement |
|------|--------------|
| **P** | Réservoir vide (collecteur) |
| **A** | Actionneur valve |
| **R** | Évent filtré (atmosphère) |

**Type** : 3/2 Normalement Fermé (NC)

## États de Fonctionnement

### État Repos (Pressostat inactif - boost < 0,20 bar)

```
EV Position : NC (Fermée)
├── P → A : CONNECTÉ (vide appliqué)
├── R → A : BLOQUÉ
└── Résultat : Valve FERMÉE (silencieux)
```

| Élément | État |
|---------|------|
| Pressostat | Ouvert (NO au repos) |
| Électrovanne | Non alimentée |
| Circuit P-A | Connecté |
| Actionneur | Sous vide |
| Valve échappement | **FERMÉE** |

### État Actif (Pressostat actif - boost ≥ 0,20 bar)

```
EV Position : Activée (Ouverte)
├── P → A : BLOQUÉ
├── R → A : CONNECTÉ (mise à l'air)
└── Résultat : Valve OUVERTE (libre)
```

| Élément | État |
|---------|------|
| Pressostat | Fermé (détection boost) |
| Électrovanne | Alimentée |
| Circuit R-A | Connecté |
| Actionneur | À l'atmosphère |
| Valve échappement | **OUVERTE** |

## Composants Requis

### Circuit Pneumatique

| Composant | Spécification | Quantité |
|-----------|---------------|----------|
| Clapet anti-retour | Ø adapté, sens collecteur→réservoir | 1 |
| Réservoir vide | 0,3 à 0,5 L | 1 |
| Électrovanne 3/2 NC | ≤15ms, orifice ≥2,5mm | 1 |
| Filtre évent | Protection poussières | 1 |
| Durite silicone | Ø adapté | ~2m |
| Raccords | T, droits selon montage | Selon besoin |

### Circuit Électrique

Voir [Schéma Électrique](schema-electrique.md)

## Avantages

✅ Utilise le vide existant du moteur  
✅ Valve fermée par défaut (sécurité si panne)  
✅ Ouverture instantanée par mise à l'air  
✅ Réservoir maintient le vide pendant les phases boost  

## Inconvénients

⚠️ Nécessite un clapet anti-retour fiable  
⚠️ Réservoir supplémentaire à installer  
⚠️ Vide disponible variable selon charge moteur  

## Installation

1. **Repérer** la prise de vide sur le collecteur d'admission
2. **Installer** le clapet anti-retour (sens : collecteur → réservoir)
3. **Monter** le réservoir dans un emplacement accessible
4. **Positionner** l'électrovanne à moins de 20 cm de l'actionneur
5. **Raccorder** le filtre évent dans un endroit protégé
6. **Connecter** le circuit électrique selon le schéma

## Points de Contrôle

- [ ] Étanchéité du circuit vide
- [ ] Sens du clapet anti-retour correct
- [ ] Réservoir fixé solidement
- [ ] Distance EV-actionneur < 20 cm
- [ ] Filtre évent protégé de l'eau