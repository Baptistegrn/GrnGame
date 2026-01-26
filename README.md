# GrnGame Engine

Moteur de jeu 2D multiplateforme utilisant Lua et SDL.

## 📋 Prérequis

- Python 3.x
- Git
- **Pour compiler le moteur :**
  - Windows : xmake, WSL, Docker Desktop
  - Linux : Docker

## 🚀 Installation

### Récupérer le projet

```bash
# Cloner le dépôt
git clone https://github.com/Baptistegrn/GrnGame.git

# Ou télécharger et extraire le ZIP
```

### Créer un projet

```bash
cd commandProject
python createProject.py <nom_projet> [chemin_destination] [--level <mode>]
```

**Arguments :**
- `name` : Nom du projet (obligatoire)
- `path` : Dossier de destination (défaut : répertoire courant)
- `--level` : Mode de build (défaut : debug)
  - `debug` : Mode développement avec logs
  - `release` : Mode production sans logs (utiliser avec précaution)
  - `debug_malloc` : Mode debug avec compteur malloc/free (contribution moteur uniquement)

### Structure du projet généré

```
projet/
├── src/
│   ├── main.lua          # Point d'entrée obligatoire
│   ├── log/
│   └── encrypt.py
├── windows/
│   ├── App.exe
│   ├── engine.lua
│   ├── GrnGame.dll
│   └── gamecontrollerdb.txt
└── linux/
    ├── App
    ├── engine.lua
    ├── libGrnGame.so
    └── gamecontrollerdb.txt
```

## 🎮 Utilisation

### Lancer le jeu

**Windows :**
```bash
cd windows
./App.exe
```

**Linux :**
```bash
cd linux
./App
```

⚠️ **Important :** Le fichier `src/main.lua` est le point d'entrée obligatoire. Ne pas le supprimer ou le renommer.

Vous pouvez inclure d'autres fichiers Lua depuis `main.lua`.

## 🔐 Encryption (optionnel)

> ⚠️ **En cours de développement - Sujet à modifications**

### Procédure

1. **Sauvegarder vos fichiers** avant toute manipulation
2. Obfusquer `main.lua` avec des outils externes (recommandé)
3. Utiliser `encrypt.py` pour chiffrer le fichier
4. Modifier la clé dans `encrypt.py` si nécessaire
5. **Si vous modifiez la clé :** recompiler le moteur avec la même clé

**Emplacement de la clé dans le moteur :**
```
src/bindings_lua/initialisation_lua.c
```

> ℹ️ Le décryptage reste possible pour des personnes expérimentées. L'encryption n'est pas une protection absolue.

## 🔧 Compilation du moteur

**Requis uniquement si vous :**
- Modifiez la clé d'encryption
- Contribuez au développement du moteur

### Windows (64 bits)

```bash
# Mode debug avec compteur malloc
xmake f --debug_mode=yes --malloc_mode=yes

# Mode debug standard
xmake f --debug_mode=yes --malloc_mode=no

# Mode release
xmake f --debug_mode=no --malloc_mode=no

xmake
```

### Linux (64 bits)

```bash
# Copier les fichiers Docker
cp dockerForLinux/* .

# Construire l'image Docker
./buildDocker.bat

# Entrer dans le conteneur
./enterdocker.ps1

# Lancer la compilation (modifier build.sh selon le mode souhaité)
./build.sh
```

### Vérification

Les binaires compilés se trouvent dans :
```
bin/
├── malloc_mode/
├── debug_mode/
└── release/
```

Vérifiez que les fichiers suivants ont été créés récemment :
- `GrnGame.dll` (Windows)
- `libGrnGame.so` (Linux)
- `App.exe` (Windows)
- `App` (Linux)

## 📚 API Lua

### Initialisation

```lua
local Engine = require("engine")

function update()
    -- Votre code de jeu
end

Engine.initialize(hauteur, largeur, fps, barres, "Titre", update)
```

### Core

```lua
Engine.stop()                                    -- Arrête le moteur
Engine.resize(w, h, fullscreen, window_fs)      -- Redimensionne la fenêtre
Engine.clear(r, g, b)                           -- Efface l'écran
Engine.delta()                                  -- Delta time
Engine.fps()                                    -- FPS actuels
```

### Logs

```lua
-- Niveaux : GRN_LOG_DEBUG=0, GRN_LOG_INFO=1, GRN_LOG_WARNING=2, GRN_LOG_ERROR=3
Engine.setLogLvl(niveau)
Engine.log(niveau, "message")
```

### Dimensions

```lua
Engine.offsetX()          -- Offset X
Engine.offsetY()          -- Offset Y
Engine.currentWidth()     -- Largeur fenêtre
Engine.currentHeight()    -- Hauteur fenêtre
Engine.universeWidth()    -- Largeur univers
Engine.universeHeight()   -- Hauteur univers
```

### Rendu

```lua
Engine.draw(path, x, y, w, h, flip, rotation, alpha)
Engine.drawText(font, text, x, y, scale, flip, spacing, rotation, alpha)
Engine.drawRect(x, y, w, h, r, g, b, alpha, filled)
Engine.drawCircle(x, y, radius, r, g, b, filled)
Engine.drawTriangle(x, y, w, h, r, g, b, filled)
Engine.drawLine(x1, y1, x2, y2, r, g, b)

Engine.loadImageFolder(folder)   -- Précharge les images
Engine.freeImageFolder()          -- Libère la mémoire
Engine.setIcon(path)              -- Définit l'icône
```

### Audio

```lua
Engine.playSound(path, loop, channel, volume)
Engine.stopSound(path)
Engine.pauseSound(path)
Engine.resumeSound(path)
Engine.stopChannel(channel)
Engine.pauseChannel(channel)
Engine.resumeChannel(channel)

Engine.loadSongFolder(folder)    -- Précharge les sons
Engine.freeSongFolder()           -- Libère la mémoire
```

### Input - Clavier/Souris

```lua
Engine.keyPressed(key)            -- Touche maintenue
Engine.keyJustPressed(key)        -- Touche vient d'être pressée
Engine.showCursor(visible)        -- Affiche/cache le curseur

Engine.mouseX()
Engine.mouseY()
Engine.mouseLeftPressed()
Engine.mouseRightPressed()
Engine.mouseLeftJustPressed()
Engine.mouseRightJustPressed()
Engine.mouseScrollVertical()
Engine.mouseScrollHorizontal()
```

### Input - Manette

```lua
Engine.initController(idx)                    -- Initialise une manette
Engine.buttonPressed(button, idx)             -- Bouton maintenu
Engine.buttonJustPressed(button, idx)         -- Bouton vient d'être pressé
Engine.getJoysticks(dead_zone, idx)          -- Joysticks (retourne tableau)
Engine.closeController(idx)
Engine.closeJoystick(idx)
Engine.closeTheController(idx)
```

### Entités et Physique

#### Entité Platformer

```lua
local player = Engine.newEntityPlatformer(x, y, width, height, gravity, powerJump)

player.x, player.y                -- Position
player.speedY                     -- Vitesse verticale
player.inSky                      -- En l'air ?
player.requestJump                -- Demande de saut
player.leftLock, player.rightLock -- Collision murs

player:updatePhysics(blocks, max_fall_speed, wall_correction, type_lock)
```

#### Entité Top-down

```lua
local entity = Engine.newEntityTopdown(x, y, width, height)

entity.x, entity.y                -- Position

entity:updatePhysics(blocks, type_lock)
```

#### Blocs de collision

```lua
local blocks = Engine.Blocks.new(path, grid_x, grid_y, separator, exclusion)

#blocks  -- Nombre de blocs
```

**Format du fichier de blocs :**
- Chaque ligne représente un bloc
- Séparateur personnalisable
- Valeur d'exclusion pour ignorer certains types

### Caméra

```lua
local camera = Engine.newCamera(width, height, smooth_factor)

camera.x, camera.y                -- Position
camera.w, camera.h                -- Dimensions
camera.smooth_factor              -- Facteur de lissage

camera:update(target_x, target_y, dt)
```

## 🖥️ Plateformes supportées

- Windows 7+ (64 bits)
- Ubuntu 18.04+ et équivalents (64 bits)
- SteamOS 2.0, 3.0, 4.0 (64 bits) (non teste)
- macOS (nouvelles puces, 64 bits) - À venir

## 🤝 Contribution

Les contributions sont les bienvenues ! Utilisez le mode `debug_malloc` pour détecter les fuites mémoire et n'hésitez pas à créer des pull requests.

## 📄 Licence

Voir le dépôt GitHub pour plus d'informations.

---

**Dépôt :** https://github.com/Baptistegrn/GrnGame.git
