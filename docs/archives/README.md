# Archives - Variantes Pneumatiques

Ce dossier contient des variantes de conception pneumatique qui n'ont **pas été retenues** pour la version finale du circuit.

## Variantes archivées

### Variante A - Actionneur fermé par vide
Système utilisant le vide du collecteur d'admission + réservoir pour maintenir la valve fermée au repos.

**Statut :** ❌ Non retenue

### Variante B - Actionneur ouvert par pression boost
Système utilisant directement la pression de boost (après intercooler) pour ouvrir la valve.

**Statut :** ❌ Non retenue

## Solution finale retenue

Le circuit **v4.5** utilise un **compresseur d'air externe** (90-120 PSI) avec :
- Régulateur SMC IR1000-01BG (réduction à 1 bar/15 PSI)
- Électrovanne MAC 35A 3/2
- Détection boost via pressostat SMC ISE30A sur ligne MAP

Voir [`hardware/VALVE_v4_5.docx`](../../hardware/VALVE_v4_5.docx) pour le circuit complet.
