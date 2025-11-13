<img src="https://raw.githubusercontent.com/Baptistegrn/GrnGame/main/GrnGame/xmake/logo_et_texte.png" >


**Un moteur de jeu 2D Python puissant, conçu pour le pixel art**

[![Contributions Welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](https://github.com/Baptistegrn/GrnGame/issues)

[![Python Version](https://img.shields.io/badge/python-3.8+-blue.svg)](https://www.python.org/downloads/)
[![License](https://img.shields.io/badge/license-MIT-green.svg)](LICENSE)
[![PyPI](https://img.shields.io/badge/pypi-v1.0.0-orange.svg)](https://pypi.org/project/GrnGame/)

[Fonctionnalités](#-fonctionnalités) • [Installation](#-installation) • [Démarrage rapide](#-démarrage-rapide) • [Documentation](#-api-principale) • [Exemples](#-exemples)

</div>

---

## ✨ Fonctionnalités

**GrnGame est une bibliotheque légere,performante et simple pour créer des jeux 2D en Python :**

* 🖼️ **Rendu optimisé** - Gestion de sprites avec batch rendering
* 🔊 **Audio multicanal** - 32 canaux audio simultanés
* ⌨️ **Entrées complètes** - Clavier, souris et manettes
* 🎮 **Support gamepad** - API intuitive pour les contrôleurs
* 🧮 **Outils mathématiques** - Bibliothèque de fonctions intégrée
* 🚀 **Haute performance** - Optimisé pour le pixel art et les jeux rétro
* 🪟 **Multi-plateforme** - Compatible Windows et Linux
* 🆑​ **police custom** - Police custom sans ttf
* ⚙️​ **Compilation** - possibilite de compiler
* ❌​ **Erreur** - Systeme d'erreur dans un .log

---

## 🎬 Exemples

<div align="center">

**Jeu d'exemple simple**

<img src="https://raw.githubusercontent.com/Baptistegrn/GrnGame/main/GrnGame/creations/example/example.gif" width="600">

**Démo Space Attacks**

<img src="https://raw.githubusercontent.com/Baptistegrn/GrnGame/main/GrnGame/creations/spaceattacks/space.gif" width="600">

</div>

---

## 📦 Installation

### Via PyPI

```bash
pip install GrnGame
```

### Import dans votre projet

```python
import GrnGame
```


---

## 🚀 Démarrage rapide

### Premier programme

```python
import GrnGame

def update():
    # Dessiner un sprite
    GrnGame.image.dessiner("./assets/player.png", 10, 10, 32, 32)
    
    # Détecter une action
    if GrnGame.clav.juste_presser("space"):
        GrnGame.son.jouer("./assets/jump.wav")

# Initialiser le jeu
GrnGame.utils.init(
    largeur=160,              # Résolution virtuelle (largeur)
    hauteur=90,               # Résolution virtuelle (hauteur)
    fps=60,                   # Images par seconde
    coeff=4,                  # Facteur d'échelle de la fenêtre
    chemin_image="./assets",  # Dossier des images
    chemin_son="./assets",    # Dossier des sons
    dessiner=True,            # Dessiner le fond
    bande_noir=True,          # Bandes noires si ratio différent
    update_func=update,       # Fonction appelée à chaque frame
    nom_fenetre="Mon Jeu",    # Titre de la fenêtre
    chemin_erreur="err.log"   # Chemin du fichier d'erreur
)
```

---

## 📖 API Principale

### 🎯 Propriétés globales

| Propriété | Type | Description |
|-----------|------|-------------|
| `GrnGame.const.largeur` | `int` | Largeur de la résolution virtuelle |
| `GrnGame.const.hauteur` | `int` | Hauteur de la résolution virtuelle |
| `GrnGame.const.dt` | `float` | Delta time (temps entre frames) |
| `GrnGame.const.fps` | `float` | FPS actuel du jeu |
| `GrnGame.const.time` | `float` | Temps écoulé depuis le démarrage |
| `GrnGame.const.run` | `bool` | État d'exécution du jeu |
| `GrnGame.const.decalage_x` | `int` | Décalage horizontal en plein écran |
| `GrnGame.const.decalage_y` | `int` | Décalage vertical en plein écran |

---

### 🖱️ Entrée souris

```python
# Position de la souris
x = GrnGame.const.mouse_x
y = GrnGame.const.mouse_y

# Bouton gauche
if GrnGame.const.mouse_presse:            # Maintenu
    pass
if GrnGame.const.mouse_juste_presse:      # Vient d'être pressé
    pass

# Bouton droit
if GrnGame.const.mouse_droit_presse:      # Maintenu
    pass
if GrnGame.const.mouse_droit_juste_presse:  # Vient d'être pressé
    pass
```

---

### ⌨️ Entrée clavier
**LES ENTREES CLAVIER SONT LES TOUCHES PHYSIQUES DU CLAVIER ANGLAIS  :  W equivaut a Z pour un clavier francais**
```python
# Détecter une pression unique
if GrnGame.clav.juste_presser("space"):
    print("Espace pressé !")

# Détecter une touche maintenue
if GrnGame.clav.enfoncee("left"):
    position_x -= vitesse
```

<details>
<summary><b>📋 Liste complète des touches supportées</b></summary>

**Lettres** : `a` à `z`

**Chiffres** : `0` à `9`

**Navigation** :
- Flèches : `up`, `down` (ou `dn`), `left` (ou `lt`), `right` (ou `rt`)
- Page : `home`, `end`, `pageup` (ou `pgup`), `pagedown` (ou `pgdn`)

**Système** :
- `space`, `enter` (ou `return`), `tab`, `backspace`
- `escape` (ou `esc`), `delete`, `insert`

**Modificateurs** :
- Shift : `shift`, `lshift`, `rshift`
- Ctrl : `ctrl` (ou `control`), `lctrl`, `rctrl`
- Alt : `alt`, `lalt`, `ralt`
- Caps : `caps`, `capslock`
- `numlock`, `scrolllock`

**Touches fonction** : `f1` à `f12`

**Spéciales** :
- `pause`, `break`, `print` (ou `prtscr`, `printscreen`)
- `sysreq`, `menu`, `application`

**GUI/Système** :
- Windows : `lwin`, `rwin`, `lgui`, `rgui`
- Super : `lsuper`, `rsuper`
- Mac : `lcmd`, `rcmd`

**Pavé numérique** :
- Chiffres : `kp0` à `kp9`
- Opérateurs : `kp+` (ou `kpplus`), `kp-` (ou `kpminus`), `kp*` (ou `kpmultiply`), `kp/` (ou `kpdivide`)
- Autres : `kp=` (ou `kpequals`), `kp.` (ou `kpperiod`), `kpenter` (ou `kpe`)

**Média** :
- Volume : `mute` (ou `audiomute`), `volumeup` (ou `volup`, `audioup`), `volumedown` (ou `voldown`, `audiodown`)
- Contrôle : `play` (ou `audioplay`, `mediaplay`, `playpause`), `stop` (ou `audiostop`, `mediastop`)
- Navigation : `next` (ou `audionext`, `medianext`), `previous` (ou `prev`, `audioprev`, `mediaprev`)

**Navigateur** :
- `browserback`, `browserfwd`, `browserstop`
- `browserhome`, `browserrefresh`, `browsersearch`

**Énergie** : `power`, `sleep`

**Caractères spéciaux** : `-`, `=`, `[`, `]`, `;`, `'`, `,`, `.`, `/`, `` ` ``, `\`

</details>

---

### 🎮 Manettes (Gamepads)

```python
# Initialiser une manette
if GrnGame.man.init(0):  # 0 = première manette connectée
    print("Manette connectée !")

# Détecter les boutons
if GrnGame.man.juste_presse("a"):
    print("Bouton A pressé !")

if GrnGame.man.enfoncee("x"):
    print("Bouton X maintenu")

# Lire les joysticks et gâchettes
axes = GrnGame.man.renvoie_joysticks(dead_zone=0.15)
if axes:
    stick_g_x, stick_g_y, stick_d_x, stick_d_y, gachette_l, gachette_r = axes
    
    # Déplacement avec le stick gauche
    position_x += stick_g_x * vitesse
    position_y += stick_g_y * vitesse

# Fermer la manette (important avant de quitter)
GrnGame.man.fermer()
```

<details>
<summary><b>🎮 Boutons de manette supportés</b></summary>

**Boutons faciaux** : `a`, `b`, `x`, `y`

**Système** : `start`, `back`, `select`, `guide`, `home`, `share`

**Bumpers** : `lb`, `rb`, `l1`, `r1`

**Sticks cliquables** : `l3`, `r3`

**D-Pad** : `up`, `down`, `left`, `right`

**Additionnels** : `paddle1`, `paddle2`, `paddle3`, `paddle4`, `touchpad`

**Valeurs des axes** :
- Retourne 6 valeurs flottantes entre -1.0 et 1.0
- `dead_zone` : seuil pour ignorer les petites déviations (défaut: 0.1)
- Ordre : stick gauche X/Y, stick droit X/Y, gâchette gauche, gâchette droite

</details>

---

### 🖼️ Rendu graphique

#### Dessiner des formes

```python
# Forme unique
GrnGame.image.dessiner_forme(
    x=10, y=20,
    w=32, h=32,
    sens=0,        # 0=normal, 1=miroir horizontal
    rotation=0,    # Rotation en degrés (0-360)
    transparence=255,  # 0-255
    r=255, g=255, b=255  # Couleur RGB
)

# Batch rendering (plus performant pour plusieurs formes)
GrnGame.image.dessiner_forme_batch(
    xs=[0, 32, 64],
    ys=[0, 0, 16],
    ws=[32, 32, 48],
    hs=[32, 32, 48],
    sens=[0, 0, 1],           # Optionnel (défaut: 0)
    rotations=[0, 0, 90],     # Optionnel (défaut: 0)
    transparences=[255, 255, 255],  # Optionnel (défaut: 255)
    rs=[255, 255, 100],       # Optionnel (défaut: 255)
    gs=[255, 100, 100],       # Optionnel (défaut: 255)
    bs=[255, 100, 100]        # Optionnel (défaut: 255)
)
```

#### Dessiner des images

```python
# Image unique
GrnGame.image.dessiner(
    lien="./assets/sprite.png",
    x=10, y=20,
    w=32, h=32,
    sens=0,        # 0=normal, 1=miroir horizontal
    rotation=0     # Rotation en degrés (0-360)
    transparence=255,  # 0-255
)

# Batch rendering (plus performant pour plusieurs sprites)
GrnGame.image.dessiner_batch(
    ids=["./assets/tile.png", "./assets/tile.png", "./assets/enemy.png"],
    xs=[0, 32, 64],
    ys=[0, 0, 16],
    ws=[32, 32, 48],
    hs=[32, 32, 48],
    sens=[0, 0, 1],        # Optionnel
    rotations=[0, 0, 90]   # Optionnel
    transparences=[255, 255, 255] # Optionnel
)
```

#### Afficher du texte

```python
GrnGame.image.dessiner_mot(
    lien="./assets/font",    # Chemin vers la police
    mot="Score: 1000",
    x=10, y=10,
    coeff=1,      # Échelle du texte
    ecart=1,      # Espacement entre caractères
    sens=0,       # 0=normal, 1=miroir
    rotation=0    # Rotation en degrés
)
```
> 💡 **Astuce** : Les éléments sont rendus dans l'ordre d'appel. Le dernier élément dessiné apparaît au premier plan.
#### Console de debug

```python
GrnGame.utils.ecrire_console("Message de debug")
```



---

### 🔊 Gestion audio

```python
# Jouer un son
GrnGame.son.jouer(
    lien="./assets/explosion.wav",
    boucle=0,    # -1=infini, 1+=nombre de répétitions a partir de 1
    canal=-1     # -1=auto, ou 0-31 = canal spécifique
)

# Contrôle des canaux
GrnGame.son.arreter_canal(5)
GrnGame.son.pause_canal(5)
GrnGame.son.reprendre_canal(5)

# Contrôle des sons individuels
GrnGame.son.arreter("./assets/music.wav")
GrnGame.son.pause("./assets/music.wav")
GrnGame.son.reprendre("./assets/music.wav")
```

**Caractéristiques** :
- 32 canaux audio simultanés (0-31)
- Format supporté : WAV
- `boucle=-1` : lecture en boucle infinie
- `boucle=n` : répète n fois puis s'arrête

---

### 🧮 Fonctions mathématiques

```python
# Opérations de base
GrnGame.maths.abs_val(-5)              # Valeur absolue → 5
GrnGame.maths.clamp(15, 0, 10)         # Limiter entre min/max → 10
GrnGame.maths.pow(2, 8)                # Puissance → 256
GrnGame.maths.sqrt(64)                 # Racine carrée → 8.0
GrnGame.maths.cbrt(27)                 # Racine cubique → 3.0
GrnGame.maths.random(10, 50)           # Nombre aléatoire entre 10 et 50

# Trigonométrie
angle = GrnGame.maths.atan2(dy, dx)    # Angle entre deux points
distance = GrnGame.maths.hypot(dx, dy) # Distance euclidienne

# Arrondis
GrnGame.maths.floor(3.7)    # → 3.0
GrnGame.maths.ceil(3.2)     # → 4.0
GrnGame.maths.round(3.5)    # → 4.0
GrnGame.maths.trunc(3.9)    # → 3.0

# Logarithmes et exponentielles
GrnGame.maths.exp(2)        # e^x
GrnGame.maths.log(10)       # ln(x)
GrnGame.maths.log10(100)    # log₁₀(x)
GrnGame.maths.log2(8)       # log₂(x)
```

<details>
<summary><b>📐 Fonctions complètes</b></summary>

**Trigonométrie** : `sin`, `cos`, `tan`, `asin`, `acos`, `atan`, `atan2`

**Hyperboliques** : `sinh`, `cosh`, `tanh`, `asinh`, `acosh`, `atanh`

**Exponentielles** : `exp`, `log`, `log10`, `log2`

**Arrondis** : `floor`, `ceil`, `round`, `trunc`

**Autres** : `fmod`, `hypot`, `abs_val`, `clamp`, `pow`, `sqrt`, `cbrt`, `random`

</details>

---

### 🎨 Gestion de la fenêtre

```python
# Basculer en plein écran / fenêtré
GrnGame.utils.redimensionner_fenetre()

# Arrêter le jeu proprement
GrnGame.utils.stopper_jeu()
```

---

## ⚙️ Physique 2D

### Plateforme 2D (Platformer)

Pour implémenter la physique d'un jeu de plateforme 2D avec gravity, sauts et collisions de murs :

```python
from GrnGame.hitbox import platformer_2d

# Dans la boucle de jeu
pos_x, pos_y, vitesse_y, est_en_air, collision_gauche, collision_droite = platformer_2d(
    dt=GrnGame.const.dt,           # Delta time
    pos_x=player_x,                # Position X du joueur
    pos_y=player_y,                # Position Y du joueur
    larg_joueur=16,                # Largeur du joueur
    haut_joueur=16,                # Hauteur du joueur
    vitesse_y=vitesse_y,           # Vitesse Y actuelle
    est_en_air=est_en_air,         # État aérien du joueur
    blocs=blocs,                   # Liste des blocs de collision [x, y, w, h]
    gravite=400.0,                 # Gravité (pixels/s²)
    force_saut=-200.0,             # Force du saut (négatif = vers le haut)
    vitesse_max_chute=500.0,       # Vitesse max de chute
    correction_mur=100.0,          # Bonus de vélocité quand on monte un mur
    touches_clavier_saut=["z"],    # Touches clavier pour sauter
    touches_manette_saut=["Y"],    # Boutons manette pour sauter
    joy_saut=False                 # Activer le saut via joystick
)

# Utiliser les résultats
player_x = pos_x
player_y = pos_y
est_en_air = est_en_air
if collision_gauche or collision_droite:
    print("Collision avec un mur !")
```

**Structure des blocs** :
- `blocs = [[x, y, width, height], ...]`
- Chaque bloc est un rectangle de collision statique

**Valeurs par défaut** :
- `gravite=400.0` : Ajuste la vitesse de chute
- `force_saut=-200.0` : Négatif pour sauter vers le haut
- `vitesse_max_chute=500.0` : Limite de vitesse en chute
- `correction_mur=100.0` : freine si frottage de mur

---

### 🎨 Gestion de la fenêtre

```python
# Basculer en plein écran / fenêtré
GrnGame.utils.redimensionner_fenetre()

# Arrêter le jeu proprement
GrnGame.utils.stopper_jeu()
```

---

## 🅰️ Créer une police personnalisée

GrnGame utilise des polices bitmap (images) plutôt que des fichiers TTF.

### Structure requise

```
assets/
└── font/
    ├── 32.png    # Espace (code ASCII 32)
    ├── 33.png    # !
    ├── 48.png    # 0
    ├── 49.png    # 1
    ├── 65.png    # A
    ├── 66.png    # B
    ├── 97.png    # a
    └── ...
```

### Règles importantes

1. **Nom de fichier** = code ASCII du caractère
   - `65.png` = lettre "A"
   - `97.png` = lettre "a"
   - `48.png` = chiffre "0"


2. **Caractères supportés** : ASCII standard (0-127)
   - `32.png` = espace
   - `48-57.png` = chiffres 0-9
   - `65-90.png` = majuscules A-Z
   - `97-122.png` = minuscules a-z

### Exemple d'utilisation

```python
GrnGame.image.dessiner_mot(
    lien="./assets/font",
    mot="Score: 1234",
    x=10, y=10,
    coeff=2,     # Taille ×2
    ecart=1,     # 1 pixel entre lettres
    sens=0,
    rotation=0
)
```

### Créer plusieurs styles

```
assets/
├── font_pixel/      # Police pixel art
├── font_outline/    # Police avec contour
└── font_neon/       # Police style néon
```

```python
# Changer de police dynamiquement
GrnGame.image.dessiner_mot("./assets/font_outline", "GAME OVER", 40, 40, coeff=3)
```

---

## 📚 Structure de projet recommandée

```
mon_jeu/
├── main.py
├── assets/
│   ├── sprites/
│   │   ├── player.png
│   │   ├── enemy.png
│   │   └── tiles.png
│   ├── fonts/
│   │   ├── main/
│   │   │   ├── 32.png
│   │   │   ├── 65.png
│   │   │   └── ...
│   │   └── title/
│   │       └── ...
│   └── sounds/
│       ├── jump.wav
│       ├── shoot.wav
│       └── music.wav
└── README.md
```

---

## 💻 Exemple complet

```python
import GrnGame

# Variables globales
player_x = 64
player_y = 40
player_speed = 50.0  # pixels par seconde
score = 0
m = False

def update():
    global player_x, player_y, score, m
    if not m:
        m = GrnGame.man.init(0)

    # Déplacement fluide avec delta time
    deplacement = player_speed * GrnGame.const.dt
    
    if GrnGame.clav.enfoncee("left"):
        player_x -= deplacement
    if GrnGame.clav.enfoncee("right"):
        player_x += deplacement
    if GrnGame.clav.enfoncee("up"):
        player_y -= deplacement
    if GrnGame.clav.enfoncee("down"):
        player_y += deplacement
    
    # Limiter aux bords de l'écran
    player_x = GrnGame.maths.clamp(player_x, 0, GrnGame.const.largeur - 16)
    player_y = GrnGame.maths.clamp(player_y, 0, GrnGame.const.hauteur - 16)
    
    # Action avec espace
    if GrnGame.clav.juste_presser("space"):
        GrnGame.utils.redimensionner_fenetre()
        GrnGame.son.jouer("./assets/shoot.wav", boucle=1, canal=1)
        score += 10
    
    # Support manette
    axes = GrnGame.man.renvoie_joysticks(dead_zone=0.2)
    if axes:
        stick_x, stick_y = axes[0], axes[1]
        player_x += stick_x * deplacement
        player_y += stick_y * deplacement
        
        if GrnGame.man.juste_presse("a"):
            GrnGame.son.jouer("./assets/shoot.wav", boucle=1, canal=1)
            score += 10
    
    # Rendu
    GrnGame.image.dessiner("./assets/1.png", player_x, player_y, 16, 16)
    GrnGame.image.dessiner_mot("./assets/police", f"Score: {score}", 5, 5, 1, 1)
    
    GrnGame.image.dessiner_mot("./assets/police", f"FPS: {int(1/GrnGame.const.dt)}", 5, 15, 1, 1)
    
    # Quitter
    if GrnGame.clav.juste_presser("escape"):
        GrnGame.utils.stopper_jeu()

# Configuration et lancement
GrnGame.utils.init(
    largeur=160,
    hauteur=90,
    fps=60,
    coeff=6,
    chemin_image="./assets",
    chemin_son="./assets",
    dessiner=True,
    bande_noir=True,
    update_func=update,
    nom_fenetre="Mon Jeu"
)


```

---

## 🔧 Créer un exécutable

### Compilation avec PyInstaller

```bash
# Compilation basique
GrnGame_app votre_fichier.py

# Sans console (Windows uniquement)
GrnGame_app votre_fichier.py --noconsole

# Avec icône personnalisée
GrnGame_app votre_fichier.py --icon chemin/vers/icone.ico

# Combinaison des options
GrnGame_app votre_fichier.py --noconsole --icon mon_icone.ico
```

**Options disponibles** :
- `--noconsole` : Cache la console Windows (fenêtre noire)
- `--icon` : Définit l'icône de l'exécutable (fichier .ico)

**Résultat** :
- L'exécutable est généré dans `./dist/`
- Sur Windows : fichier `.exe`
- Sur Linux : binaire exécutable

**Prérequis** :
- [PyInstaller](https://pyinstaller.org/) installé : `pip install pyinstaller`

**Notes** :
- L'exécutable est **spécifique à l'OS** de compilation
- Sur Linux post-compilation xmake, privilégiez une distribution standard (Ubuntu, Debian) pour la portabilité

---

## ⚠️ Systèmes d'exploitation moins courants

Si votre système n'est pas directement compatible, GrnGame compilera automatiquement la bibliothèque native via [xmake](https://xmake.io).

Voici les etapes pour ne pas rencontrer de problemes :

1. **Installer xmake automatiquement** :
   ```bash
   GrnGame_xmake
   ```

2. **Relancer votre console/terminal** (important pour recharger les variables d'environnement)

3. **Lancer votre projet** :
   ```bash
   python votre_fichier.py
   ```

**Important** :
- La commande `GrnGame_xmake` installe [xmake](https://xmake.io) qui est nécessaire pour compiler la bibliothèque native
- **Vous devez relancer la console** après l'installation pour que xmake soit reconnu
- Cette étape n'est nécessaire qu'une seule fois par système

---

## 🐛 Résolution de problèmes

### La bibliothèque ne se charge pas
- ✅ Vérifiez que la DLL/SO est dans `GrnGame/dist/`
- ✅ Installez `xmake` pour la compilation automatique

### Les images ne s'affichent pas
- ✅ Vérifiez les chemins (relatifs au script principal)
- ✅ Utilisez uniquement des fichiers PNG
- ✅ Assurez-vous que `chemin_image` pointe vers le bon dossier

### Les sons ne fonctionnent pas
- ✅ Utilisez uniquement des fichiers WAV
- ✅ Vérifiez les chemins des fichiers audio
- ✅ Ne dépassez pas 32 canaux simultanés

### Problèmes de performance
- ✅ Utilisez `dessiner_image_batch()` pour les sprites multiples
- ✅ Réduisez le nombre d'appels à `dessiner_image()` par frame
- ✅ Optimisez la taille des sprites

### La manette ne fonctionne pas
- ✅ Appelez `init_mannette()` **après** `GrnGame.init()`
- ✅ Appelez `fermer_controller()` avant de quitter
- ✅ Vérifiez que la manette est bien connectée avant le lancement

---

## 📧 Contact & Support

**Auteur** : Baptiste GUERIN  
**Email** : [baptiste.guerin34@gmail.com](mailto:baptiste.guerin34@gmail.com)

Pour signaler un bug ou proposer une amélioration, n'hésitez pas à me contacter !

---

## 📄 Licence

Ce projet est sous licence **MIT**.

---

<div align="center">

**GrnGame** - Un moteur de jeu 2D Python puissant, conçu pour le pixel art

[🌐 GitHub](https://github.com/Baptistegrn/GrnGame) • [📦 PyPI](https://pypi.org/project/GrnGame/) • [✉️ Contact](mailto:baptiste.guerin34@gmail.com)

</div>
