# Variante B - Actionneur Ouvert par Pression

## Principe de Fonctionnement

Cette variante utilise la pression de boost (après intercooler) pour ouvrir activement la valve. Au repos, la valve reste fermée.

## Schéma de Principe

```
┌─────────────────────────────────────────────────────────────┐
│                    CIRCUIT PNEUMATIQUE                       │
├─────────────────────────────────────────────────────────────┤
│                                                              │
│  ┌──────────────┐                                           │
│  │  Intercooler │                                           │
│  │    Sortie    │                                           │
│  └──────┬───────┘                                           │
│         │                                                    │
│         │ Pression Boost                                    │
│         │                                                    │
│         │ P (Pression)                                      │
│         ▼                                                    │
│  ┌──────────────┐      A (Actionneur)    ┌──────────────┐  │
│  │    EV 3/2    │─────────────────────────│  Actionneur  │  │
│  │     NO       │                         │    Valve     │  │
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
| **P** | Pression boost (après intercooler) |
| **A** | Actionneur valve |
| **R** | Évent filtré (atmosphère) |

**Type** : 3/2 Normalement Ouvert (NO)

## États de Fonctionnement

### État Repos (Pressostat inactif - boost < 0,20 bar)

```
EV Position : NO (Ouverte au repos)
├── P → A : BLOQUÉ
├── R → A : CONNECTÉ (atmosphère)
└── Résultat : Valve FERMÉE (ressort actionneur)
```

| Élément | État |
|---------|------|
| Pressostat | Ouvert (NO au repos) |
| Électrovanne | Non alimentée |
| Circuit R-A | Connecté |
| Actionneur | À l'atmosphère |
| Valve échappement | **FERMÉE** (ressort) |

### État Actif (Pressostat actif - boost ≥ 0,20 bar)

```
EV Position : Activée (Fermée)
├── P → A : CONNECTÉ (boost appliqué)
├── R → A : BLOQUÉ
└── Résultat : Valve OUVERTE (pression)
```

| Élément | État |
|---------|------|
| Pressostat | Fermé (détection boost) |
| Électrovanne | Alimentée |
| Circuit P-A | Connecté |
| Actionneur | Sous pression boost |
| Valve échappement | **OUVERTE** |

## Composants Requis

### Circuit Pneumatique

| Composant | Spécification | Quantité |
|-----------|---------------|----------|
| Électrovanne 3/2 NO | ≤15ms, orifice ≥2,5mm | 1 |
| Filtre évent | Protection poussières | 1 |
| Durite silicone renforcée | Ø adapté, résistante pression | ~2m |
| Raccords | T, droits selon montage | Selon besoin |
| Colliers | Haute pression | Selon besoin |

### Circuit Électrique

Voir [Schéma Électrique](schema-electrique.md)

## Avantages

✅ Circuit plus simple (pas de réservoir)  
✅ Moins de composants  
✅ Pression disponible proportionnelle au besoin  
✅ Ouverture forcée quand il y a du boost  

## Inconvénients

⚠️ Valve fermée si panne électrique (peut être un avantage)  
⚠️ Nécessite durites résistantes à la pression  
⚠️ Petite perte de charge sur le circuit boost  

## Installation

1. **Repérer** un point de piquage après l'intercooler
2. **Installer** un raccord en T sur la durite de boost
3. **Positionner** l'électrovanne à moins de 20 cm de l'actionneur
4. **Utiliser** des durites silicone renforcées haute pression
5. **Raccorder** le filtre évent dans un endroit protégé
6. **Connecter** le circuit électrique selon le schéma
7. **Serrer** tous les colliers correctement

## Points de Contrôle

- [ ] Étanchéité du circuit sous pression
- [ ] Durites haute pression utilisées
- [ ] Colliers serrés correctement
- [ ] Distance EV-actionneur < 20 cm
- [ ] Filtre évent protégé de l'eau
- [ ] Pas de fuite au raccord T

## Comparaison avec Variante A

| Critère | Variante A (Vide) | Variante B (Pression) |
|---------|-------------------|----------------------|
| Complexité | Plus complexe | Plus simple |
| Composants | Plus nombreux | Moins nombreux |
| Fiabilité | Dépend du vide moteur | Dépend du boost |
| Comportement panne | Valve s'ouvre | Valve reste fermée |
| Réactivité | Très rapide | Très rapide |
