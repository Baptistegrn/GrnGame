#  GrnGame

<div align="center">

**Un moteur de jeu Python 2D puissant conçu pour les jeux en pixel art**

[![Python Version](https://img.shields.io/badge/python-3.8+-blue.svg)](https://www.python.org/downloads/)
[![License](https://img.shields.io/badge/license-MIT-green.svg)](LICENSE)
[![PyPI](https://img.shields.io/badge/pypi-v1.0.0-orange.svg)](https://pypi.org/project/GrnGame/)

</div>

---

## ✨ Fonctionnalités

**GrnGame** est un module Python léger pour créer facilement des jeux 2D :

* 🖼️ **Gestion des images** - Rendu de sprites optimisé avec batch
* 🔊 **Système audio** - Gestion multi-canaux (32 canaux)
* ⌨️ **Gestion des entrées** - Clavier et souris
* 🎮 **Support des manettes** - Intégration complète des gamepads
* 🧮 **Utilitaires mathématiques** - Fonctions intégrées
* 🚀 **Haute performance** - Optimisé pour le pixel art

👉 Compatible avec **Windows** et **Linux**.

---

## 🎬 Exemples

<div align="center">

**Exemple de jeu simple 2D**

<img src="https://raw.githubusercontent.com/Baptistegrn/GrnGame/main/GrnGame/creations/example/example.gif" width="600">

**Démo Space Attacks**

<img src="https://raw.githubusercontent.com/Baptistegrn/GrnGame/main/GrnGame/creations/spaceattacks/space.gif" width="600">

</div>

---

## 📦 Installation

### Installation depuis PyPI

```bash
pip install GrnGame
```

### Importation dans votre projet

```python
from GrnGame import GrnGame
```

> ⚠️ **Important :**  
> L'import **doit être exactement** :
> ```python
> from GrnGame import GrnGame
> ```
> Toute autre forme d'import peut empêcher la bibliothèque de localiser correctement ses dépendances internes.

---

## 🚀 Démarrage rapide

### Initialiser un jeu

```python
from GrnGame import GrnGame

def update():
    # Dessiner une image à chaque frame
    GrnGame.dessiner_image("./assets/joueur.png", 10, 10, 32, 32)
    
    if GrnGame.touche_juste_presser("space"):
        print("Espace pressée !")

GrnGame.init(
    largeur=160,            # largeur virtuelle
    hauteur=90,             # hauteur virtuelle
    fps=60,                 # images par seconde
    coeff=3,                # facteur d'échelle
    chemin_image="./assets", # dossier images
    chemin_son="./assets",   # dossier sons
    dessiner=True,          # dessiner le fond ?
    bande_noir=True,        # bandes noires si ratio différent ?
    r=0, g=0, b=0,         # couleur de fond (RVB)
    update_func=update,     # fonction de mise à jour
    nom_fenetre="MonJeu"    # nom de la fenêtre
)
```

---

## 🎮 API Principale

### 📊 Propriétés globales

| Propriété | Description |
|-----------|-------------|
| `GrnGame.largeur` | Largeur virtuelle |
| `GrnGame.hauteur` | Hauteur virtuelle |
| `GrnGame.dt` | Delta time entre frames |
| `GrnGame.fps` | FPS actuel |
| `GrnGame.time` | Temps écoulé |
| `GrnGame.run` | Le jeu tourne ? |
| `GrnGame.decalage_x` | Décalage X en plein écran |
| `GrnGame.decalage_y` | Décalage Y en plein écran |

---

### 🖱️ Entrée souris

```python
GrnGame.mouse_x                      # Position X de la souris
GrnGame.mouse_y                      # Position Y de la souris
GrnGame.mouse_presse                 # Bouton gauche pressé
GrnGame.mouse_juste_presse           # Bouton gauche vient d'être pressé
GrnGame.mouse_droit_presse           # Bouton droit pressé
GrnGame.mouse_droit_juste_presse     # Bouton droit vient d'être pressé
```

---

### ⌨️ Entrée clavier

```python
GrnGame.touche_juste_presser("a")    # Touche pressée cette frame
GrnGame.touche_enfoncee("a")         # Touche maintenue
```

#### Touches supportées

**Lettres :** `a` à `z`

**Chiffres :** `0` à `9`

**Caractères spéciaux :** `-` `=` `[` `]` `;` `'` `,` `.` `/` `` ` `` `\`

**Touches fonction :** `f1` à `f12`

**Navigation :**
- `up`, `dn` (bas), `lt` (gauche), `rt` (droite)
- `down`, `left`, `right`
- `home`, `end`, `pgup` (page haut), `pgdn` (page bas)
- `pageup`, `pagedown`

**Touches système :**
- `space`, `enter`, `return`
- `esc`, `escape`
- `tab`, `backspace`, `delete`, `insert`

**Modificateurs :**
- `shift`, `lshift`, `rshift`
- `ctrl`, `control`, `lctrl`, `rctrl`
- `alt`, `lalt`, `ralt`
- `caps`, `capslock`
- `numlock`
- `scrolllock`

**Touches spéciales :**
- `pause`, `break`
- `print`, `prtscr`, `printscreen`
- `sysreq`

**Touches Windows/GUI :**
- `lwin`, `rwin`
- `lgui`, `rgui`
- `lsuper`, `rsuper`
- `lcmd`, `rcmd` (Mac)
- `menu`, `application`

**Pavé numérique :**
- `kp0` à `kp9`
- `kp+`, `kp-`, `kp*`, `kp/`, `kp=`, `kp.`
- `kpe`, `kpenter` (entrée)
- `kpplus`, `kpminus`, `kpmultiply`, `kpdivide`, `kpequals`, `kpperiod`

**Touches média :**
- `mute`, `audiomute`
- `volup`, `volumeup`, `audioup`
- `voldown`, `volumedown`, `audiodown`
- `play`, `audioplay`, `mediaplay`, `playpause`
- `stop`, `audiostop`, `mediastop`
- `next`, `audionext`, `medianext`
- `prev`, `audioprev`, `mediaprev`, `previous`

**Système :**
- `power`, `sleep`

**Navigateur :**
- `browserback`, `browserfwd`, `browserstop`
- `browserhome`, `browserrefresh`, `browsersearch`

---

### 🎮 Entrée manette

```python
# Initialiser une manette (index 0 = première manette)
cond = GrnGame.init_mannette(0) # cond vaut true ou false si la mannette est initialise ou non

# Vérifier si un bouton vient d'être pressé
if GrnGame.touche_mannette_juste_presse("x"):
    print("X pressé !")

# Vérifier si un bouton est maintenu
if GrnGame.touche_mannette_enfoncee("a"):
    print("A maintenu !")

# Lire les sticks analogiques
axes = GrnGame.renvoie_joysticks(dead_zone=0.1)
if axes:
    gauche_x, gauche_y, droit_x, droit_y, gachette_g, gachette_d = axes
    print(f"Stick gauche: x={gauche_x}, y={gauche_y}")
    print(f"Stick droit: x={droit_x}, y={droit_y}")
    print(f"Gâchettes: L={gachette_g}, R={gachette_d}")

# Fermer la manette
GrnGame.fermer_controller()
```

#### Boutons manette supportés

**Boutons faciaux :** `a`, `b`, `x`, `y`

**Système :** `start`, `back`, `select`, `guide`, `home`, `share`

**Gâchettes supérieures (bumpers) :** `lb`, `rb`, `l1`, `r1`

**Sticks cliquables :** `l3`, `r3`

**Croix directionnelle (D-Pad) :** `up`, `down`, `left`, `right`

**Additionnels :** `paddle1`, `paddle2`, `paddle3`, `paddle4`, `touchpad`

**Valeurs des joysticks :**
- Les 6 valeurs retournées sont des flottants entre -1.0 et 1.0
- `dead_zone` : zone morte pour ignorer les petites variations (défaut 0.1)

---

### 🖼️ Graphismes

```python
# Dessiner une seule image
GrnGame.dessiner_image(
    lien="./assets/sprite.png",
    x=10, y=20,
    w=32, h=32,
    sens=0,        # 0=normal, 1=miroir horizontal
    rotation=0     # rotation en degrés (0-360)
)

# Dessiner plusieurs images en batch (plus performant)
GrnGame.dessiner_image_batch(
    ids=["./assets/tuile.png", "./assets/tuile.png", "./assets/ennemi.png"],
    xs=[0, 32, 64],
    ys=[0, 0, 16],
    ws=[32, 32, 48],
    hs=[32, 32, 48],
    sens=[0, 0, 1],      # optionnel (défaut: tous à 0)
    rotations=[0, 0, 90] # optionnel (défaut: tous à 0)
)

# Dessiner du texte
GrnGame.dessiner_mot(
    lien="./assets/police",  # chemin vers la police de caractères
    mot="Bonjour le monde",
    x=50, y=50,
    coeff=1,     # facteur d'échelle du texte
    ecart=1,     # espacement entre les caractères
    sens=0,      # 0=normal, 1=miroir horizontal
    rotation=0   # rotation en degrés
)

# Écrire dans la console de debug
GrnGame.ecrire_console("Message de debug")
```

**Notes sur le rendu :**
- Les éléments sont dessinés dans l'ordre où ils sont ajoutés
- Le dernier élément dessiné apparaît au premier plan
- Utilisez `dessiner_image_batch()` pour optimiser le rendu de multiples sprites similaires

---

### 🔊 Audio

```python
# Jouer un son
GrnGame.jouer_son(
    lien="./assets/son.wav",
    boucle=0,    # 0=infini, 1+=nombre de répétitions
    canal=23    # 0-31=canal spécifique
)

# Arrêter un son spécifique
GrnGame.arreter_son("./assets/son.wav")

# Arrêter tous les sons d'un canal
GrnGame.arreter_canal(3)

# Mettre en pause un canal
GrnGame.pause_canal(3)

# Reprendre la lecture d'un canal
GrnGame.reprendre_canal(3)

# Mettre en pause un son spécifique
GrnGame.pause_son("./assets/son.wav")

# Reprendre la lecture d'un son spécifique
GrnGame.reprendre_son("./assets/son.wav")
```

**Paramètres audio :**
- **boucle** : `0` = joue a linfini,  `1+` = nombre de répétitions
- **canal** : `0-31` = canal spécifique
- Le moteur supporte **32 canaux audio simultanés**

---

### 🧮 Fonctions mathématiques

```python
# Mathématiques de base
GrnGame.abs_val(-5)              # Valeur absolue
GrnGame.clamp(10, 0, 5)          # Limiter une valeur entre min et max
GrnGame.pow(2, 3)                # Puissance (2³ = 8)
GrnGame.sqrt(16)                 # Racine carrée
GrnGame.cbrt(27)                 # Racine cubique
GrnGame.random(0, 100)           # Nombre aléatoire entre min et max

# Trigonométrie
GrnGame.sin(3.14)                # Sinus
GrnGame.cos(1.57)                # Cosinus
GrnGame.tan(0.78)                # Tangente
GrnGame.asin(0.5)                # Arc sinus
GrnGame.acos(0.5)                # Arc cosinus
GrnGame.atan(1.0)                # Arc tangente
GrnGame.atan2(y, x)              # Arc tangente à 2 arguments

# Fonctions hyperboliques
GrnGame.sinh(1.0)                # Sinus hyperbolique
GrnGame.cosh(1.0)                # Cosinus hyperbolique
GrnGame.tanh(1.0)                # Tangente hyperbolique
GrnGame.asinh(1.0)               # Arc sinus hyperbolique
GrnGame.acosh(1.5)               # Arc cosinus hyperbolique (x >= 1)
GrnGame.atanh(0.5)               # Arc tangente hyperbolique

# Logarithmes et exponentielles
GrnGame.exp(2.0)                 # Exponentielle (e^x)
GrnGame.log(10.0)                # Logarithme naturel (ln)
GrnGame.log10(100.0)             # Logarithme base 10
GrnGame.log2(8.0)                # Logarithme base 2

# Arrondis
GrnGame.floor(3.7)               # Arrondi inférieur (3.0)
GrnGame.ceil(3.2)                # Arrondi supérieur (4.0)
GrnGame.round(3.5)               # Arrondi au plus proche (4.0)
GrnGame.trunc(3.9)               # Troncature (3.0)

# Autres fonctions
GrnGame.fmod(10.5, 3.0)          # Modulo flottant
GrnGame.hypot(3, 4)              # Hypoténuse (√(x²+y²))
```

---

### 🎨 Gestion de la fenêtre

```python
# Redimensionner la fenêtre (bascule plein écran)
GrnGame.redimensionner_fenetre()

# Changer la couleur de fond
GrnGame.colorier(r, g, b)  # r, g, b entre 0 et 255

# Arrêter le jeu
GrnGame.stopper_jeu()
```

---

## 🔧 Génération d'exécutable

Pour compiler votre projet en exécutable autonome :

```bash
GrnGame_app votre_fichier.py
```

**Détails :**
- L'exécutable sera généré dans le dossier **`/dist`**
- **[PyInstaller](https://pyinstaller.org/)** est requis pour cette étape
- L'exécutable est **spécifique à l'OS** sur lequel la compilation a été faite
- Sur Windows, génère un fichier `.exe`
- Sur Linux, génère un binaire exécutable

> 💡 **Astuce :**  
> Si vous compilez sur Linux après avoir recompilé la librairie via `xmake`, l'exécutable pourrait ne pas être portable entre différentes distributions. Compilez sur une distribution largement utilisée (Ubuntu, Debian, Fedora) pour maximiser la compatibilité.

---

## 📝 Exemple complet

```python
from GrnGame import GrnGame

# Variables du jeu
position_x = 80
position_y = 45
vitesse = 2

def update():
    global position_x, position_y
    
    # Déplacements avec les flèches
    if GrnGame.touche_enfoncee("left"):
        position_x -= vitesse
    if GrnGame.touche_enfoncee("right"):
        position_x += vitesse
    if GrnGame.touche_enfoncee("up"):
        position_y -= vitesse
    if GrnGame.touche_enfoncee("down"):
        position_y += vitesse
    
    # Limiter la position à l'écran
    position_x = GrnGame.clamp(position_x, 0, GrnGame.largeur - 32)
    position_y = GrnGame.clamp(position_y, 0, GrnGame.hauteur - 32)
    
    # Jouer un son quand on appuie sur espace
    if GrnGame.touche_juste_presser("space"):
        GrnGame.jouer_son("./assets/laser.wav")
    
    # Dessiner le joueur
    GrnGame.dessiner_image("./assets/player.png", position_x, position_y, 32, 32)
    
    # Afficher les FPS
    GrnGame.dessiner_mot("./assets/font", f"FPS: {int(GrnGame.fps)}", 5, 5, 1, 1)
    
    # Quitter avec Échap
    if GrnGame.touche_juste_presser("esc"):
        GrnGame.stopper_jeu()

# Initialisation et lancement du jeu
GrnGame.init(
    largeur=160,
    hauteur=90,
    fps=60,
    coeff=5,
    chemin_image="./assets",
    chemin_son="./assets",
    dessiner=True,
    bande_noir=True,
    r=20, g=20, b=50,
    update_func=update,
    nom_fenetre="Mon Jeu"
)
```
🅰️ Créer sa propre police personnalisée

Pour faire sa propre police ce n'est pas avec des .ttf :

📁 Structure requise

Crée un dossier (par exemple font/) qui contiendra une image par caractère :

assets/
└── font/
    ├── 32.png   # Espace (code ASCII 32)
    ├── 33.png   # !
    ├── 65.png   # A
    ├── 66.png   # B
    ├── 67.png   # C
    ├── 97.png   # a
    ├── 98.png   # b
    └── ...

⚙️ Règles importantes

Chaque fichier représente une seule lettre ou symbole

Le nom du fichier doit être le code ASCII du caractère (ex. 65.png = A)

Tous les fichiers doivent être dans le même dossier

Les images doivent avoir la même taille (ex. 8×8, 16×16, 32×32, etc.)

les inclusions sont les caracteres ascii non etendu sur un char .

💡 Exemple :
32.png = espace
48.png à 57.png = chiffres 0–9
65.png à 90.png = lettres majuscules A–Z
97.png à 122.png = lettres minuscules a–z

🧩 Exemple d’utilisation
# Dessiner du texte avec une police personnalisée
GrnGame.dessiner_mot(
    lien="./assets/font",    # chemin vers ton dossier de police
    mot="Bonjour !",
    x=10, y=10,
    coeff=1,                 # taille du texte
    ecart=1,                 # espacement entre lettres
    sens=0,                  # 0=normal, 1=miroir horizontal
    rotation=0               # rotation du texte
)

✅ Astuce

Tu peux créer plusieurs styles :

assets/
├── font_pixel/
├── font_outline/
└── font_shadow/


Et changer dynamiquement la police utilisée :

GrnGame.dessiner_mot("./assets/font_outline", "Game Over", 40, 40, coeff=2)
---

## 📚 Structure de projet recommandée

```
mon_jeu/
├── main.py              # Fichier principal
├── assets/              # Dossier des ressources
│   ├── player.png       # Sprites
│   ├── enemy.png
│   ├── tile.png
│   ├── font/            # Police de caractères
│   │   ├── 35.png
│   │   ├── 89.png
│   │   └── ...
│   ├── laser.wav        # Sons
│   ├── music.wav
│   └── explosion.wav
```

---

## ⚙️ Compilation automatique

Si votre système d'exploitation **n'est pas directement compatible**, la compilation de la bibliothèque native sera **automatiquement effectuée via [xmake](https://xmake.io)**.

Assurez-vous que :
- La librairie compilée se trouve bien dans **`GrnGame/dist/`**
- `xmake` est correctement installé et accessible dans votre terminal
- Un compilateur compatible est installé (GCC, Clang ou MSVC)

---

## ✅ Notes importantes

* Les chemins des fichiers sont **relatifs au script principal**
* Les images sont dessinées **dans l'ordre d'ajout** - l'ordre définit la profondeur de rendu
* `update_func` doit être une **fonction callable**
* Utilisez `dessiner_image_batch()` pour optimiser le rendu de multiples images similaires
* Pour les manettes : toujours appeler `init_mannette()` après `init()` et fermer avec `fermer_controller()` avant de quitter
* Les coordonnées utilisent un système de **résolution virtuelle** qui est mise à l'échelle automatiquement

---

## 🐛 Résolution de problèmes

**La librairie ne se charge pas :**
- Vérifiez que la DLL/SO est bien dans le dossier `dist/`
- Essayez d'installer `xmake` pour la compilation automatique
- Vérifiez les logs dans le fichier `erreurs.log`

**Les images ne s'affichent pas :**
- Vérifiez que le chemin vers les images est correct
- Les chemins sont relatifs au script principal
- Les formats supportés sont PNG

**Les sons ne fonctionnent pas :**
- Vérifiez le format des fichiers audio WAV
- Assurez-vous que le chemin est correct
- Vérifiez que le nombre de canaux n'est pas dépassé (max 32)

**Problèmes de performance :**
- Utilisez `dessiner_image_batch()` pour dessiner plusieurs sprites
- Limitez le nombre d'appels à `dessiner_image()` par frame

---

## 📧 Support

Pour signaler un bug ou proposer une amélioration :  
📧 **[baptiste.guerin34@gmail.com](mailto:baptiste.guerin34@gmail.com)**

---


---

<div align="center">

**Fait par Baptiste GUERIN**

[GitHub](https://github.com/Baptistegrn/GrnGame) • [PyPI](https://pypi.org/project/GrnGame/) • [Email](mailto:baptiste.guerin34@gmail.com)

</div>
