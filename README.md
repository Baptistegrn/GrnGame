# GrnGame

<div align="center">

**Un moteur de jeu 2D Python puissant, conçu pour le pixel art**

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
from GrnGame import GrnGame
```

> ⚠️ **Important** : L'import doit être exactement `from GrnGame import GrnGame`. Toute autre forme peut empêcher la localisation des dépendances internes.

---

## 🚀 Démarrage rapide

### Premier programme

```python
from GrnGame import GrnGame

def update():
    # Dessiner un sprite
    GrnGame.dessiner_image("./assets/player.png", 10, 10, 32, 32)
    
    # Détecter une action
    if GrnGame.touche_juste_presser("space"):
        GrnGame.jouer_son("./assets/jump.wav")

# Initialiser le jeu
GrnGame.init(
    largeur=160,              # Résolution virtuelle (largeur)
    hauteur=90,               # Résolution virtuelle (hauteur)
    fps=60,                   # Images par seconde
    coeff=4,                  # Facteur d'échelle de la fenêtre
    chemin_image="./assets",  # Dossier des images
    chemin_son="./assets",    # Dossier des sons
    dessiner=True,            # Dessiner le fond
    bande_noir=True,          # Bandes noires si ratio différent
    r=0, g=0, b=0,           # Couleur de fond RGB
    update_func=update,       # Fonction appelée à chaque frame
    nom_fenetre="Mon Jeu"     # Titre de la fenêtre
)
```

---

## 📖 API Principale

### 🎯 Propriétés globales

| Propriété | Type | Description |
|-----------|------|-------------|
| `GrnGame.largeur` | `int` | Largeur de la résolution virtuelle |
| `GrnGame.hauteur` | `int` | Hauteur de la résolution virtuelle |
| `GrnGame.dt` | `float` | Delta time (temps entre frames) |
| `GrnGame.fps` | `float` | FPS actuel du jeu |
| `GrnGame.time` | `float` | Temps écoulé depuis le démarrage |
| `GrnGame.run` | `bool` | État d'exécution du jeu |
| `GrnGame.decalage_x` | `int` | Décalage horizontal en plein écran |
| `GrnGame.decalage_y` | `int` | Décalage vertical en plein écran |

---

### 🖱️ Entrée souris

```python
# Position de la souris
x = GrnGame.mouse_x
y = GrnGame.mouse_y

# Bouton gauche
if GrnGame.mouse_presse:            # Maintenu
    pass
if GrnGame.mouse_juste_presse:      # Vient d'être pressé
    pass

# Bouton droit
if GrnGame.mouse_droit_presse:      # Maintenu
    pass
if GrnGame.mouse_droit_juste_presse:  # Vient d'être pressé
    pass
```

---

### ⌨️ Entrée clavier
**LES ENTREES CLAVIER SONT LES TOUCHES PHYSIQUES DU CLAVIER ANGLAIS  :  W equivaut a Z pour un clavier francais**
```python
# Détecter une pression unique
if GrnGame.touche_juste_presser("space"):
    print("Espace pressé !")

# Détecter une touche maintenue
if GrnGame.touche_enfoncee("left"):
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
if GrnGame.init_mannette(0):  # 0 = première manette connectée
    print("Manette connectée !")

# Détecter les boutons
if GrnGame.touche_mannette_juste_presse("a"):
    print("Bouton A pressé !")

if GrnGame.touche_mannette_enfoncee("x"):
    print("Bouton X maintenu")

# Lire les joysticks et gâchettes
axes = GrnGame.renvoie_joysticks(dead_zone=0.15)
if axes:
    stick_g_x, stick_g_y, stick_d_x, stick_d_y, gachette_l, gachette_r = axes
    
    # Déplacement avec le stick gauche
    position_x += stick_g_x * vitesse
    position_y += stick_g_y * vitesse

# Fermer la manette (important avant de quitter)
GrnGame.fermer_controller()
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

#### Dessiner des images

```python
# Image unique
GrnGame.dessiner_image(
    lien="./assets/sprite.png",
    x=10, y=20,
    w=32, h=32,
    sens=0,        # 0=normal, 1=miroir horizontal
    rotation=0     # Rotation en degrés (0-360)
)

# Batch rendering (plus performant pour plusieurs sprites)
GrnGame.dessiner_image_batch(
    ids=["./assets/tile.png", "./assets/tile.png", "./assets/enemy.png"],
    xs=[0, 32, 64],
    ys=[0, 0, 16],
    ws=[32, 32, 48],
    hs=[32, 32, 48],
    sens=[0, 0, 1],        # Optionnel
    rotations=[0, 0, 90]   # Optionnel
)
```

#### Afficher du texte

```python
GrnGame.dessiner_mot(
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
GrnGame.ecrire_console("Message de debug")
```



---

### 🔊 Gestion audio

```python
# Jouer un son
GrnGame.jouer_son(
    lien="./assets/explosion.wav",
    boucle=0,    # 0=infini, 1+=nombre de répétitions
    canal=5      # 0-31 = canal spécifique
)

# Contrôle des canaux
GrnGame.arreter_canal(5)
GrnGame.pause_canal(5)
GrnGame.reprendre_canal(5)

# Contrôle des sons individuels
GrnGame.arreter_son("./assets/music.wav")
GrnGame.pause_son("./assets/music.wav")
GrnGame.reprendre_son("./assets/music.wav")
```

**Caractéristiques** :
- 32 canaux audio simultanés (0-31)
- Format supporté : WAV
- `boucle=0` : lecture en boucle infinie
- `boucle=n` : répète n fois puis s'arrête

---

### 🧮 Fonctions mathématiques

```python
# Opérations de base
GrnGame.abs_val(-5)              # Valeur absolue → 5
GrnGame.clamp(15, 0, 10)         # Limiter entre min/max → 10
GrnGame.pow(2, 8)                # Puissance → 256
GrnGame.sqrt(64)                 # Racine carrée → 8.0
GrnGame.cbrt(27)                 # Racine cubique → 3.0
GrnGame.random(10, 50)           # Nombre aléatoire entre 10 et 50

# Trigonométrie
angle = GrnGame.atan2(dy, dx)    # Angle entre deux points
distance = GrnGame.hypot(dx, dy) # Distance euclidienne

# Arrondis
GrnGame.floor(3.7)    # → 3.0
GrnGame.ceil(3.2)     # → 4.0
GrnGame.round(3.5)    # → 4.0
GrnGame.trunc(3.9)    # → 3.0

# Logarithmes et exponentielles
GrnGame.exp(2)        # e^x
GrnGame.log(10)       # ln(x)
GrnGame.log10(100)    # log₁₀(x)
GrnGame.log2(8)       # log₂(x)
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
GrnGame.redimensionner_fenetre()

# Changer la couleur de fond
GrnGame.colorier(r, g, b)  # RGB (0-255)

# Arrêter le jeu proprement
GrnGame.stopper_jeu()
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

2. **Taille uniforme** : toutes les images doivent avoir les mêmes dimensions (ex: 8×8, 16×16)

3. **Caractères supportés** : ASCII standard (0-127)
   - `32.png` = espace
   - `48-57.png` = chiffres 0-9
   - `65-90.png` = majuscules A-Z
   - `97-122.png` = minuscules a-z

### Exemple d'utilisation

```python
GrnGame.dessiner_mot(
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
GrnGame.dessiner_mot("./assets/font_outline", "GAME OVER", 40, 40, coeff=3)
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
from GrnGame import GrnGame

# Variables globales
player_x = 64
player_y = 40
player_speed = 50.0  # pixels par seconde
score = 0
m = False

def update():
    global player_x, player_y, score,m
    if not m:
       m= GrnGame.init_mannette(0)

    # Déplacement fluide avec delta time
    deplacement = player_speed * GrnGame.dt
    
    if GrnGame.touche_enfoncee("left"):
        player_x -= deplacement
    if GrnGame.touche_enfoncee("right"):
        player_x += deplacement
    if GrnGame.touche_enfoncee("up"):
        player_y -= deplacement
    if GrnGame.touche_enfoncee("down"):
        player_y += deplacement
    
    # Limiter aux bords de l'écran
    player_x = GrnGame.clamp(player_x, 0, GrnGame.largeur - 16)
    player_y = GrnGame.clamp(player_y, 0, GrnGame.hauteur - 16)
    
    # Action avec espace
    if GrnGame.touche_juste_presser("space"):
        GrnGame.jouer_son("./assets/shoot.wav", boucle=1, canal=1)
        score += 10
    
    # Support manette
    axes = GrnGame.renvoie_joysticks(dead_zone=0.2)
    if axes:
        stick_x, stick_y = axes[0], axes[1]
        player_x += stick_x * deplacement
        player_y += stick_y * deplacement
        
        if GrnGame.touche_mannette_juste_presse("a"):
            GrnGame.jouer_son("./assets/shoot.wav", boucle=1, canal=1)
            score += 10
    
    # Rendu
    GrnGame.dessiner_image("./assets/player.png", player_x, player_y, 16, 16)
    GrnGame.dessiner_mot("./assets/font", f"Score: {score}", 5, 5, 1, 1)
    GrnGame.dessiner_mot("./assets/font", f"FPS: {int(GrnGame.fps)}", 5, 15, 1, 1)
    
    # Quitter
    if GrnGame.touche_juste_presser("escape"):
        GrnGame.stopper_jeu()

# Configuration et lancement
GrnGame.init(
    largeur=160,
    hauteur=90,
    fps=60,
    coeff=6,
    chemin_image="./assets",
    chemin_son="./assets",
    dessiner=True,
    bande_noir=True,
    r=15, g=15, b=35,
    update_func=update,
    nom_fenetre="Mon Jeu"
)


```

---

## 🔧 Créer un exécutable

### Compilation avec PyInstaller

```bash
GrnGame_app votre_fichier.py
```

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

## ⚙️ Compilation automatique

Si votre système n'est pas directement compatible, GrnGame compilera automatiquement la bibliothèque native via [xmake](https://xmake.io).

**Vérifications** :
- La bibliothèque compilée doit se trouver dans `GrnGame/dist/`
- `xmake` doit être installé et accessible
- Un compilateur compatible (GCC, Clang, MSVC) doit être présent

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
