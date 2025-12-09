# Valve d'Échappement Automatique - Mustang EcoBoost 2016

Circuit pneumatique et électrique pour contrôle automatique de valve d'échappement.

## Véhicule

- **Modèle** : Ford Mustang EcoBoost 2016
- **Configuration** : Stage 4+ E85
- **Équipement existant** : OPR V2, Plunger, Drain correct

## Contexte

Redémarrage après 1 mois. Fumée blanche à froid due à la condensation. Disparition à chaud.

**Pressions mesurées à chaud :**
- Moteur : 35 psi au ralenti
- Ligne turbo : 28 psi avant plunger
- État : RAS

## Objectif

Ouvrir automatiquement la valve d'échappement en charge pour éviter toute contre-pression, avec un mode manuel "toujours ouvert".

## Cibles Techniques

| Paramètre | Valeur cible | Acceptable |
|-----------|--------------|------------|
| Contre-pression turbine-out (valve ouverte) | < 1,5 psi | 1,5 - 3 psi |
| Pression huile moteur (ralenti chaud) | 25 - 35 psi | - |
| Pression entrée turbo après plunger (ralenti) | 15 - 30 psi | - |
| Pression entrée turbo après plunger (haut régime) | 40 - 45 psi | - |

> **Certitude : 9/10** - Sources mixtes atelier et docs fabricants

## Solution Pneumatique

### Détection
- **Pressostat boost NO** réglé à 0,20 - 0,25 bar sur ligne MAP
- **Hystérésis** ≥ 0,05 bar

### Action
- **Électrovanne 3/2** : temps de réponse ≤ 15 ms, orifice ≥ 2,5 mm
- **Placement** : < 20 cm de l'actionneur

### Variantes Disponibles

| Variante | Principe | Source | Documentation |
|----------|----------|--------|---------------|
| **A** | Actionneur fermé par vide | Collecteur admission | [Détails](docs/variante-A-vide.md) |
| **B** | Actionneur ouvert par pression | Boost après intercooler | [Détails](docs/variante-B-pression.md) |

## Commande Électrique

```
+12V après contact
    │
    ├── Fusible 5A
    │
    ├── TVS 1.5KE18CA (protection)
    │
    ├── Relais 12V 30A + diode 1N5819
    │
    └── Inter SPDT 3 positions
        ├── Fermé
        ├── Auto
        └── Ouvert
```

### Câblage
- **Puissance** : 1,5 mm²
- **Commande** : 1,0 mm²
- **Masse** : châssis propre

📄 [Schéma électrique détaillé](docs/schema-electrique.md)

## Documentation

- 📋 [Spécifications techniques](docs/specifications.md)
- 🔧 [Variante A - Vide](docs/variante-A-vide.md)
- 🔧 [Variante B - Pression](docs/variante-B-pression.md)
- ⚡ [Schéma électrique](docs/schema-electrique.md)
- ✅ [Procédures de validation](docs/validation.md)
- 🛒 [Liste des composants (BOM)](bom/bill-of-materials.md)

## Validation

- [ ] Seuil pressostat 0,20 - 0,25 bar vérifié
- [ ] Test statique pompe à main OK
- [ ] Mesure contre-pression en charge (Auto) < 1,5 psi
- [ ] (Optionnel) Pression après plunger @ 2000/3000 tr/min

## Modes d'Utilisation

| Mode | Position Inter | Usage |
|------|----------------|-------|
| **Fermé** | Position 1 | Ville, autoroute (silencieux) |
| **Auto** | Position 2 | Ouverture automatique dès 0,20-0,25 bar MAP |
| **Ouvert** | Position 3 | Bypass manuel permanent |

## License

MIT License - Voir [LICENSE](LICENSE)

---

**Projet** : Circuit valve échappement automatique  
**Véhicule** : Mustang EcoBoost 2016 Stage 4+ E85  
**Auteur** : @mmmprod