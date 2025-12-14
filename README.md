<div align="center">
<img src="https://raw.githubusercontent.com/Baptistegrn/GrnGame/main/GrnGame/icone/iconex8.png" width="150" alt="GrnGame Icon">

<h1>GrnGame</h1>

[![PyPI](https://img.shields.io/badge/pypi-v1.0.0-orange.svg)](https://pypi.org/project/GrnGame/)
[![Python Version](https://img.shields.io/badge/python-3.11+-blue.svg)](https://www.python.org/downloads/)
[![License](https://img.shields.io/badge/license-MIT-green.svg)](LICENSE)
[![Contributions Welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](https://github.com/Baptistegrn/GrnGame/issues)

[ **Français** | [English](docs/README_EN.md) ]

</div>

**GrnGame** est un moteur de jeu 2D performant pour Python, conçu spécifiquement pour le pixel art.

**PYTHON SUPPORTÉ : 3.11, 3.12, 3.13, 3.14**

Avec une gestion optimisée des sprites (batch rendering) et une API intuitive inspirée de pyxel et pygame, vous pouvez créer des jeux fluides et légers facilement.

<div align="center">
<img src="https://raw.githubusercontent.com/Baptistegrn/GrnGame/main/GrnGame/creations/gif/example.gif" width="45%">
<img src="https://raw.githubusercontent.com/Baptistegrn/GrnGame/main/GrnGame/creations/gif/space.gif" width="35%">
<img src="https://raw.githubusercontent.com/Baptistegrn/GrnGame/main/GrnGame/creations/gif/platformer.gif" width="35%">
</div>

## Spécifications

- Fonctionne sur Windows et Linux
- Programmation en Python 3.11+
- Rendu optimisé avec Batch Rendering
- Audio multicanal (32 canaux simultanés)
- Entrées complètes (Clavier, Souris, Manettes)
- Support des polices personnalisées (Bitmap fonts)
- Primitives graphiques (cercle, rectangle, ligne, triangle)
- Shaders de couleur personnalisables
- Physique 2D intégrée (Platformer)
- Compilation facile en exécutable

## Installation

### Via PyPI

```bash
pip install GrnGame
```

### Depuis les sources

Si votre système nécessite une compilation native (ex: Linux spécifique), GrnGame utilisera automatiquement `xmake`.

```bash
# 1. Installe xmake si nécessaire
GrnGame_xmake

# 2. Relancez votre terminal, puis lancez votre script
python main.py
```

## Utilisation Rapide

Dans votre script Python, importez `GrnGame`, définissez une fonction `update`, et initialisez le moteur.

```python
import GrnGame

def update():
    # Dessiner un sprite
    GrnGame.image.dessiner("./assets/player.png", 10, 10, 32, 32)
    
    # Entrées clavier (Noms des touches anglaises: 'space', 'enter', etc.)
    if GrnGame.clavier.juste_presse("space"):
        GrnGame.son.jouer("./assets/jump.wav")
        
    # Quitter
    if GrnGame.clavier.juste_presse("escape"):
        GrnGame.utils.stopper_jeu()

# Initialiser le jeu
GrnGame.utils.init(
    largeur=160,              # Résolution virtuelle
    hauteur=90,               # Résolution virtuelle
    fps=60,                   # Images par seconde cible
    coeff=4,                  # Échelle de la fenêtre
    chemin_image="./assets",  # Dossier racine des images
    chemin_son="./assets",    # Dossier racine des sons
    update_func=update,       # Fonction de boucle
    nom_fenetre="Mon Jeu",
    chemin_erreur="erreurs/err.log" # Chemin vers les logs d'erreurs
)
```

## Modules Disponibles

GrnGame expose les modules suivants :

- `GrnGame.utils` - Utilitaires système
- `GrnGame.const` - Constantes du jeu
- `GrnGame.image` - Rendu graphique
- `GrnGame.clavier` ou `GrnGame.keyboard` - Entrées clavier
- `GrnGame.manette` ou `GrnGame.controller` - Entrées manette
- `GrnGame.son` ou `GrnGame.song` - Gestion audio
- `GrnGame.platformer_2d` - Physique 2D pour plateformes

## Référence API

### Système & Constantes (`GrnGame.const`, `GrnGame.utils`)

#### Constantes

- `largeur`, `hauteur` (`width`, `height`)  
  La largeur et la hauteur de la résolution virtuelle.

- `dt`  
  Le temps écoulé entre deux frames (Delta Time).

- `fps`  
  Le nombre d'images par seconde actuel.

- `time`  
  Le nombre total de frames écoulées depuis le démarrage.

- `souris_x`, `souris_y` (`mouse_x`, `mouse_y`)  
  Position de la souris relative à l'écran virtuel.

- `mouse_presse` (`mouse_pressed`)  
  Renvoie `True` si le bouton gauche de la souris est enfoncé.

- `mouse_juste_presse` (`mouse_just_pressed`)  
  Renvoie `True` si le bouton gauche de la souris vient d'être pressé ce frame.

- `mouse_droit_presse` (`mouse_right_pressed`)  
  Renvoie `True` si le bouton droit de la souris est enfoncé.

- `mouse_droit_juste_presse` (`mouse_right_just_pressed`)  
  Renvoie `True` si le bouton droit de la souris vient d'être pressé ce frame.

- `decalage_x`, `decalage_y` (`offset_x`, `offset_y`)  
  Le décalage actuel de la caméra (coordonnées du coin supérieur gauche).

- `en_marche` (`running`)  
  Renvoie `True` si le jeu est en cours d'exécution.

#### Utilitaires

- `init(largeur, hauteur, fps, coeff, chemin_image, chemin_son, bande_noir, update_func, nom_fenetre, chemin_erreur)`  
  (`initialize(width, height, fps, scale, image_path, sound_path, letterbox, update_func, window_name, error_path)`)  
  Initialise la fenêtre et le moteur.

- `stopper_jeu()` (`stop_game()`)  
  Ferme proprement le moteur et la fenêtre.

- `redimensionner_fenetre()` (`resize_window()`)  
  Bascule entre le mode fenêtré et le plein écran.

- `colorier(r, g, b)` (`colorize(r, g, b)`)  
  Applique une teinte de couleur RGB à tous les rendus suivants.

- `afficher_curseur(afficher=True)` (`show_cursor(show=True)`)  
  Affiche ou masque le curseur de la souris.

- `ecrire_log(type_erreur, message)` (`write_log(type_error, message)`)  
  Écrit un message dans les fichiers de logs. Types disponibles : `"info"`, `"debug"`, `"avertissement"` (`"warning"`), `"erreur"` (`"error"`).

- `changer_log(niveau)` (`change_log(level)`)  
  Change le niveau de journalisation. Niveaux disponibles : `"info"`, `"debug"`, `"avertissement"` (`"warning"`), `"erreur"` (`"error"`).

- `platformer_2d(dt, pos_x, pos_y, vitesse_y, en_air, larg_joueur, haut_joueur, blocs, [gravite], [force_saut], [vitesse_max_chute], [correction_mur], [activer_saut])`  
  Physique 2D intégrée pour les jeux de plateforme. Gère la gravité, les sauts, et les collisions avec les blocs. Renvoie un tuple `(nouvelle_pos_x, nouvelle_pos_y, nouvelle_vitesse_y, nouveau_en_air)`.
  - `dt` : Delta time (utiliser `GrnGame.const.dt`)
  - `pos_x`, `pos_y` : Position actuelle du joueur
  - `vitesse_y` : Vitesse verticale actuelle
  - `en_air` : `True` si le joueur est en l'air
  - `larg_joueur`, `haut_joueur` : Dimensions du joueur
  - `blocs` : Liste de rectangles `[(x, y, w, h), ...]` représentant les collisions
  - `gravite` : Force de gravité (défaut: 400.0)
  - `force_saut` : Force du saut, valeur négative (défaut: -200.0)
  - `vitesse_max_chute` : Vitesse maximale de chute (défaut: 500.0)
  - `correction_mur` : Distance de correction lors d'une collision murale (défaut: 100.0)
  - `activer_saut` : `True` pour effectuer un saut ce frame

### Graphismes (`GrnGame.image`)

#### Sprites et Images

- `dessiner(lien, x, y, w, h, [sens], [rotation], [transparence])`  
  (`draw(path, x, y, w, h, [direction], [rotation], [transparency])`)  
  Dessine une image à la position donnée. `sens` permet d'inverser l'image (0=normal, 1=miroir horizontal).

- `dessiner_batch(ids, xs, ys, ws, hs, [sens], [rotations], [transparence])`  
  (`draw_batch(ids, xs, ys, ws, hs, [directions], [rotations], [transparency])`)  
  Dessine une liste d'images en un seul appel GPU pour des performances maximales.

#### Primitives Graphiques

<div align="center">

| Primitive | Démo |
|-----------|------|
| **Cercle** | <video src="https://raw.githubusercontent.com/Baptistegrn/GrnGame/main/GrnGame/demo/mp4/cercle.mp4" width="200" controls></video> |
| **Rectangle** | <video src="https://raw.githubusercontent.com/Baptistegrn/GrnGame/main/GrnGame/demo/mp4/rectangle.mp4" width="200" controls></video> |
| **Triangle** | <video src="https://raw.githubusercontent.com/Baptistegrn/GrnGame/main/GrnGame/demo/mp4/triangle.mp4" width="200" controls></video> |
| **Ligne** | <video src="https://raw.githubusercontent.com/Baptistegrn/GrnGame/main/GrnGame/demo/mp4/ligne.mp4" width="200" controls></video> |

</div>

- `dessiner_ligne(x1, y1, x2, y2, [r], [g], [b])`  
  (`draw_line(x1, y1, x2, y2, [r], [g], [b])`)  
  Dessine une ligne entre deux points avec une couleur RGB (défaut: blanc).

- `dessiner_rectangle(x, y, w, h, [r], [g], [b], [plein])`  
  (`draw_rectangle(x, y, w, h, [r], [g], [b], [filled])`)  
  Dessine un rectangle. `plein=True` pour un rectangle rempli, `False` pour un contour.

- `dessiner_cercle(x, y, rayon, [r], [g], [b], [plein])`  
  (`draw_circle(x, y, radius, [r], [g], [b], [filled])`)  
  Dessine un cercle centré à (x, y). `plein=True` pour un disque, `False` pour un cercle.

- `dessiner_triangle(x1, y1, x2, y2, [r], [g], [b], [plein])`  
  (`draw_triangle(x1, y1, x2, y2, [r], [g], [b], [filled])`)  
  Dessine un triangle avec trois sommets. `plein=True` pour un triangle rempli.

#### Texte et Polices

- `dessiner_mot(lien_police, mot, x, y, coeff, ecart, [sens], [rotation], [alpha])`  
  (`draw_word(path, word, x, y, scale, spacing, [direction], [rotation], [alpha])`)  
  Affiche du texte en utilisant une police bitmap (dossier d'images PNG).
  - `lien_police` : Chemin vers le dossier contenant les caractères (ex: `"./fonts/ma_police/"`)
  - `mot` : Texte à afficher
  - `x`, `y` : Position du texte
  - `coeff` : Échelle du texte
  - `ecart` : Espacement entre les caractères
  - `sens` : Inversion horizontale (0=normal, 1=miroir)
  - `rotation` : Rotation en degrés
  - `alpha` : Transparence (0-255)

#### Shaders et Effets

- `appliquer_shaders(xs, ys, [rs], [gs], [bs], [transparences])`  
  (`apply_shaders(xs, ys, [rs], [gs], [bs], [alphas])`)  
  Applique des shaders de couleur sur plusieurs points à la fois.
  - `xs`, `ys` : Listes des positions
  - `rs`, `gs`, `bs` : Listes des composantes RGB (défaut: [155, 155, 20])
  - `transparences` : Liste des valeurs de transparence (défaut: [125])

#### Gestion de Ressources

- `charger_toutes_les_textures(dossier)` (`load_all_textures(folder)`)  
  Précharge toutes les images d'un dossier en mémoire pour de meilleures performances.

- `charger_tous_les_sons(dossier)` (`load_all_sounds(folder)`)  
  Précharge tous les sons d'un dossier en mémoire.

- `liberer_gestionnaire_image()` (`free_image_manager()`)  
  Libère toutes les textures chargées de la mémoire.

- `liberer_gestionnaire_son()` (`free_sound_manager()`)  
  Libère tous les sons chargés de la mémoire.

#### Créer une Police Bitmap Personnalisée

Pour créer votre propre police bitmap :

1. **Créez un dossier** pour votre police (ex: `fonts/ma_police/`)

2. **Ajoutez vos images de caractères** :
   - Un fichier PNG par caractère
   - Nommez chaque fichier avec son **code ASCII** (ex: `65.png` pour 'A', `97.png` pour 'a')
   - **Toutes les images doivent avoir la même taille** pour un rendu optimal

3. **Structure d'exemple** :
   ```
   fonts/
   └── ma_police/
       ├── 32.png   (espace)
       ├── 33.png   (!)
       ├── 65.png   (A)
       ├── 97.png   (a)
       └── ...
   ```

4. **Utilisation** :
   ```python
   # Le chemin est simplement le nom du dossier
   GrnGame.image.dessiner_mot("fonts/ma_police", "Hello!", 10, 10, 2, 1)
   ```

**Codes ASCII utiles** :
- `32` : Espace
- `33-47` : Ponctuation (!, ", #, $, %, etc.)
- `48-57` : Chiffres (0-9)
- `65-90` : Lettres majuscules (A-Z)
- `97-122` : Lettres minuscules (a-z)

### Audio (`GrnGame.son` ou `GrnGame.song`)

- `jouer(lien, [boucle], [canal], [volume])`  
  (`play(path, [loop], [channel], [volume])`)  
  Joue un son `.wav`. 
  - `boucle` : Nombre de répétitions (-1 pour infini, 0 pour une seule lecture)
  - `canal` : Canal audio (0-31, ou -1 pour choix automatique)
  - `volume` : Volume de lecture (0-128, défaut: 64)

- `arreter(lien)` (`stop(path)`)  
  Arrête la lecture d'un son spécifique.

- `arreter_canal(canal)` (`stop_channel(channel)`)  
  Stoppe tous les sons sur un canal spécifique (0-31).

- `pause(lien)` (`pause_sound(path)`)  
  Met en pause un son spécifique.

- `pause_canal(canal)` (`pause_channel(channel)`)  
  Met en pause tous les sons sur un canal spécifique.

- `reprendre(lien)` (`resume(path)`)  
  Reprend la lecture d'un son mis en pause.

- `reprendre_canal(canal)` (`resume_channel(channel)`)  
  Reprend la lecture de tous les sons mis en pause sur un canal spécifique.

### Entrées (`GrnGame.clavier` / `GrnGame.keyboard`, `GrnGame.manette` / `GrnGame.controller`)

#### Clavier

- `juste_presse(touche)` (`just_pressed(touch_name)`)  
  Renvoie `True` si la touche vient d'être pressée ce frame. (Ex: "space", "a", "up")

- `enfoncee(touche)` (`pressed(touch_name)`)  
  Renvoie `True` tant que la touche est maintenue.

<details>
<summary><b>Liste complète des touches supportées</b></summary>

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

#### Manette

- `init([index])` (`initialize([index])`)  
  Initialise la manette. `index` spécifie quelle manette utiliser (0 pour la première, défaut: 0).

- `enfoncee(bouton)` (`pressed(button_name)`)  
  Renvoie `True` si le bouton de la manette est maintenu. (Ex: "a", "b", "start")

- `juste_presse(bouton)` (`just_pressed(button_name)`)  
  Renvoie `True` si le bouton de la manette vient d'être pressé ce frame.

- `renvoie_joysticks([dead_zone])` (`get_joysticks([dead_zone])`)  
  Renvoie les axes des sticks et gâchettes sous forme de liste `[lx, ly, rx, ry, lt, rt]`.
  - Valeurs entre -1.0 et 1.0 pour les sticks
  - Valeurs entre 0.0 et 1.0 pour les gâchettes
  - `dead_zone` : Seuil pour ignorer les petites déviations (défaut: 0.1)

- `fermer()` (`close()`)  
  Ferme la connexion avec la manette.

<details>
<summary><b>Boutons de manette supportés</b></summary>

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

## Exemples de Code

### Dessiner des Primitives

```python
import GrnGame

def update():
    # Cercle rouge rempli
    GrnGame.image.dessiner_cercle(50, 50, 20, 255, 0, 0, True)
    
    # Rectangle bleu vide (contour)
    GrnGame.image.dessiner_rectangle(100, 100, 40, 30, 0, 0, 255, False)
    
    # Ligne verte
    GrnGame.image.dessiner_ligne(10, 10, 100, 100, 0, 255, 0)
    
    # Triangle jaune
    GrnGame.image.dessiner_triangle(150, 50, 180, 80, 255, 255, 0, True)

GrnGame.utils.init(largeur=200, hauteur=150, fps=60, update_func=update)
```

### Batch Rendering pour Performance

```python
import GrnGame

# Position et taille de 1000 sprites
xs = [i * 10 for i in range(1000)]
ys = [50] * 1000
ws = [16] * 1000
hs = [16] * 1000
ids = ["./assets/particle.png"] * 1000

def update():
    # Dessine 1000 sprites en un seul appel GPU
    GrnGame.image.dessiner_batch(ids, xs, ys, ws, hs)

GrnGame.utils.init(largeur=320, hauteur=180, fps=60, update_func=update)
```

## Compilation

Pour créer un exécutable autonome de votre jeu :

```bash
# Créer un exécutable (Windows .exe ou Linux binaire)
GrnGame_app mon_jeu.py --noconsole --icon mon_icone.ico
```

L'exécutable sera généré dans le dossier courant.

## Licence

GrnGame est sous licence [MIT License](LICENSE).

<div align="center">
Développé par Baptiste GUERIN
</div>