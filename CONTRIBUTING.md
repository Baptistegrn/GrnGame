# Guide de contribution — GrnGame Engine

Ce document décrit les conventions et la procédure pour contribuer au projet.
Merci pour ton aide — respecte les conventions ci‑dessous pour faciliter les revues.

---

## 1. Principes généraux
- Ouvre une issue pour les grosses modifications.
- Travaille sur une branche dédiée et nommée clairement (ex. `feature/ma-fonction`).
- Fais des PR petites et ciblées.
- Ajoute des tests et mets à jour le `README` correspondant.

---

## 2. Conventions de code
### Langue
- Code interne du moteur : **français** (noms, commentaires internes).
- Code public / API exposée : **anglais** (noms et documentation publique).

### Style & nommage
- Style interne : **snake_case** (ex. `ma_fonction`, `ma_variable`).
- API publique : **camelCase** (ex. `myFunction`, `myVariable`).
- Fichiers et dossiers : **snake_case**.

### Commentaires
- Chaque fonction publique/privée doit être commentée.
- Utilise commentaires clairs et concis ; préférence pour le français dans le moteur.

### Exposition API moteur
- Si tu implémentes une fonctionnalité du moteur, expose‑la en C dans `GrnGame.h` (nommage et documentation en anglais) **et** fournis le binding Lua correspondant.

### Propreté
- Respecte les règles de formatage du projet (indentation, includes explicites).

---

## 3. Procédure Pull Request (PR)
- Branche depuis `main` : `git checkout -b feature/ma-fonction`.
- Commit fréquents, messages clairs (voir format ci‑dessous).
- Push et ouvre une PR vers `main` en décrivant : but, changements, breaking changes, tests ajoutés.
- Assigne reviewers et attends la revue ; corrige les retours avant merge.

Format de message de commit recommandé :
```
type(scope): courte description

Description détaillée (si nécessaire)

Refs: #<issue-number>
```
Types usuels : `feat`, `fix`, `chore`, `docs`, `test`, `refactor`.

---

## 4. Tests et Qualité
- Ajoute des tests unitaires pour tout comportement modifié ou ajouté.
- Exécute les tests localement avant de créer la PR.
- Les PR critiques doivent passer la CI (lint, builds, tests) avant merge.

---

## 5. Priorités de développement
1. Corrections de bugs et stabilité
2. Documentation (uniformisation et exemples)
3. Bindings C++ (compléter l'API exposée)
4. CI / CD : workflows windows 64 bits , linux (docker windows) 64 bits

---

## 6. Autres recommandations
- Respecte la rétrocompatibilité pour l'API publique quand possible.
- Documente tout changement d'API dans le README.
- Pour les grosses modifications, ouvre d'abord une RFC (issue) pour discussion.

Merci — bonne contribution ! 🙏

