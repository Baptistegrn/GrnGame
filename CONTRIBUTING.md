# Guide de Contribution - GrnGame Engine

## Conventions de Code

### Code Interne du Moteur
- **Langue** : Français
- **Style de nommage** : snake_case (ex: `ma_fonction`, `ma_variable`)
- **Commentaires** : Chaque fonction doit être commentée
- **Structure** : Code bien divisé en plusieurs fichiers
- **Propriétés modifiables** : À définir dans `proprietes.h` (exemple : CLE 42)

### Code Public/Exposé aux Utilisateurs
- **Langue** : Anglais
- **Style de nommage** : camelCase (ex: `myFunction`, `myVariable`)
- **Commentaires** : Bien documentés en français sauf dans ce cas :
- **Cas spécial** : `GrnGame.h` dans `bindings_c/` utilise des commentaires anglais

### Structure des Fichiers et Dossiers
- **Dossiers** : snake_case
- **Fichiers** : snake_case

## Priorités de Développement

### 1. Refactorisation des Dépendances
- Chaque fichier `.c` doit dépendre uniquement de ses propres fichiers `.h`
- Supprimer la dépendance globale à `main.h`
- **Note** : Travail de longue haleine nécessitant une refactorisation complète

### 2. Documentation
- Recommenter l'ensemble du projet de manière propre et cohérente
- Uniformiser le style des commentaires selon les conventions établies

### 3. Bindings C++
- Développer les bindings C++ pour compléter l'API existante

### 4. Workflows CI/CD GitHub
- Configurer les workflows automatisés pour les différentes plateformes :
  - **Windows** : MSVC avec configurations debug/release/debug_allocation
  - **Linux** : GCC/Clang avec configurations debug/release/debug_allocation
  - **macOS** : Clang avec configurations debug/release/debug_allocation
- Tests automatisés pour chaque plateforme
- Publication automatique des releases
