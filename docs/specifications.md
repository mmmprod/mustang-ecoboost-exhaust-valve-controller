# Spécifications Techniques

## Véhicule Cible

| Caractéristique | Valeur |
|-----------------|--------|
| Modèle | Ford Mustang |
| Motorisation | EcoBoost 2.3L Turbo |
| Année | 2016 |
| Configuration | Stage 4+ |
| Carburant | E85 |

## Équipement Existant

- **OPR V2** : Oil Pressure Regulator version 2 - conservé
- **Plunger** : en place et fonctionnel
- **Drain** : correct

## Pressions de Référence

### Mesures à Chaud

| Point de mesure | Valeur mesurée | Plage normale |
|-----------------|----------------|---------------|
| Pression huile moteur (ralenti) | 35 psi | 25 - 35 psi |
| Ligne turbo avant plunger | 28 psi | - |

### Cibles de Fonctionnement

| Paramètre | Idéal | Acceptable | Certitude |
|-----------|-------|------------|-----------|
| Contre-pression turbine-out (valve ouverte) | < 1,5 psi | 1,5 - 3 psi | 9/10 |
| Pression huile moteur (ralenti chaud) | 25 - 35 psi | - | 9/10 |
| Pression entrée turbo après plunger (ralenti chaud) | 15 - 30 psi | - | 9/10 |
| Pression entrée turbo après plunger (haut régime) | 40 - 45 psi | - | 9/10 |

> **Sources** : Mixtes atelier et documentation fabricants

## Spécifications Pneumatiques

### Pressostat Boost

| Paramètre | Valeur |
|-----------|--------|
| Type | Normalement Ouvert (NO) |
| Seuil d'activation | 0,20 - 0,25 bar |
| Hystérésis minimum | ≥ 0,05 bar |
| Point de raccordement | Ligne MAP |

### Électrovanne

| Paramètre | Valeur |
|-----------|--------|
| Type | 3/2 voies |
| Temps de réponse | ≤ 15 ms |
| Orifice minimum | ≥ 2,5 mm |
| Distance max actionneur | < 20 cm |

## Spécifications Électriques

### Alimentation

| Paramètre | Valeur |
|-----------|--------|
| Tension | +12V après contact |
| Protection fusible | 5A |
| Protection surtension | TVS 1.5KE18CA |

### Relais

| Paramètre | Valeur |
|-----------|--------|
| Tension bobine | 12V DC |
| Capacité contacts | 30A |
| Protection bobine | Diode 1N5819 |

### Câblage

| Type | Section |
|------|---------|
| Circuits puissance | 1,5 mm² |
| Circuits commande | 1,0 mm² |
| Masse | Châssis propre |

### Interrupteur

| Paramètre | Valeur |
|-----------|--------|
| Type | SPDT |
| Positions | 3 |
| Position 1 | Fermé |
| Position 2 | Auto |
| Position 3 | Ouvert |

## Conditions Environnementales

### Observations

- Fumée blanche à froid après 1 mois d'arrêt (condensation normale)
- Disparition complète une fois le moteur à température
- État général : RAS (Rien À Signaler)