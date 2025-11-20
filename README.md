<div align="center">
<img src="https://raw.githubusercontent.com/Baptistegrn/GrnGame/main/GrnGame/icone/iconex8.png" width="150" alt="GrnGame Icon">

<h1>GrnGame</h1>

[![PyPI](https://img.shields.io/badge/pypi-v1.0.0-orange.svg)](https://pypi.org/project/GrnGame/)
[![Python Version](https://img.shields.io/badge/python-3.8+-blue.svg)](https://www.python.org/downloads/)
[![License](https://img.shields.io/badge/license-MIT-green.svg)](LICENSE)
[![Contributions Welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](https://github.com/Baptistegrn/GrnGame/issues)

[ **Français** | [English](docs/README.en.md) ]

</div>

**GrnGame** est un moteur de jeu 2D performant pour Python, conçu spécifiquement pour le pixel art.

**PyPi pas encore mis a jour 2.0.0**

Avec une gestion optimisée des sprites (batch rendering) et une API intuitive inspirée de pyxel et pygame, vous pouvez créer des jeux fluides et légers facilement.

<div align="center">
<img src="https://raw.githubusercontent.com/Baptistegrn/GrnGame/main/GrnGame/creations/example/example.gif" width="45%">
<img src="https://raw.githubusercontent.com/Baptistegrn/GrnGame/main/GrnGame/creations/spaceattacks/space.gif" width="35%">
<img src="https://raw.githubusercontent.com/Baptistegrn/GrnGame/main/GrnGame/creations/examplebis/platformer.gif" width="35%">
</div>

## Spécifications

- Fonctionne sur Windows et Linux
- Programmation en Python 3.8+
- Rendu optimisé avec Batch Rendering
- Audio multicanal (32 canaux simultanés)
- Entrées complètes (Clavier, Souris, Manettes)
- Support des polices personnalisées (Bitmap fonts)
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

## Référence API

### Système & Constantes (`GrnGame.const`, `GrnGame.utils`)

#### Constantes

- `largeur`, `hauteur`  
  La largeur et la hauteur de la résolution virtuelle.

- `dt`  
  Le temps écoulé entre deux frames (Delta Time).

- `fps`  
  Le nombre d'images par seconde actuel.

- `time`  
  Le nombre total de frames écoulées depuis le démarrage.

- `souris_x`, `souris_y`  
  Position de la souris relative à l'écran virtuel.

- `mouse_presse`  
  Renvoie `True` si le bouton gauche de la souris est enfoncé.

- `mouse_juste_presse`  
  Renvoie `True` si le bouton gauche de la souris vient d'être pressé ce frame.

- `mouse_droit_presse`  
  Renvoie `True` si le bouton droit de la souris est enfoncé.

- `mouse_droit_juste_presse`  
  Renvoie `True` si le bouton droit de la souris vient d'être pressé ce frame.

- `decalage_x`, `decalage_y`  
  Le décalage actuel de la caméra (coordonnées du coin supérieur gauche).

- `en_marche`  
  Renvoie `True` si le jeu est en cours d'exécution.

#### Utilitaires

- `init(largeur, hauteur, fps, coeff, chemin_image, chemin_son, bande_noir, update_func, nom_fenetre, chemin_erreur)`  
  Initialise la fenêtre et le moteur.

- `stopper_jeu()`  
  Ferme proprement le moteur et la fenêtre.

- `redimensionner_fenetre()`  
  Bascule entre le mode fenêtré et le plein écran.

- `colorier(r, g, b)`  
  Applique une teinte de couleur RGB à tous les rendus suivants.

- `ecrire_log(type_erreur, message)`  
  Écrit un message dans les fichiers de logs. Types disponibles : `"info"`, `"debug"`, `"avertissement"`, `"erreur"`.

- `changer_log(niveau)`  
  Change le niveau de journalisation. Niveaux disponibles : `"info"`, `"debug"`, `"avertissement"`, `"erreur"`.

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

- `dessiner(lien, x, y, w, h, [sens], [rotation], [transparence])`  
  Dessine une image à la position donnée. `sens` permet d'inverser l'image (0=normal, 1=miroir horizontal).

- `dessiner_batch(ids, xs, ys, ws, hs, [sens], [rotations], [transparence])`  
  Dessine une liste d'images en un seul appel GPU pour des performances maximales.

- `dessiner_forme(x, y, w, h, [sens], [rotation], [transparence], [r], [g], [b])`  
  Dessine un rectangle coloré (uni ou avec transparence).

- `dessiner_forme_batch(xs, ys, ws, hs, [sens], [rotations], [transparences], [rs], [gs], [bs])`  
  Dessine une liste de rectangles colorés en un seul appel GPU.

- `dessiner_mot(lien_police, mot, x, y, coeff, ecart, [sens], [rotation], [alpha])`  
  Affiche du texte en utilisant une police bitmap (dossier d'images PNG).
  - `lien_police` : Chemin vers le dossier contenant les caractères (ex: `"./fonts/ma_police/"`)
  - `mot` : Texte à afficher
  - `x`, `y` : Position du texte
  - `coeff` : Échelle du texte
  - `ecart` : Espacement entre les caractères
  - `sens` : Inversion horizontale (0=normal, 1=miroir)
  - `rotation` : Rotation en degrés
  - `alpha` : Transparence (0-255)

- `charger_toutes_les_textures(dossier)`  
  Précharge toutes les images d'un dossier en mémoire pour de meilleures performances.

- `charger_tous_les_sons(dossier)`  
  Précharge tous les sons d'un dossier en mémoire.

- `liberer_gestionnaire_image()`  
  Libère toutes les textures chargées de la mémoire.

- `liberer_gestionnaire_son()`  
  Libère tous les sons chargés de la mémoire.

### Audio (`GrnGame.son`)

- `jouer(lien, [boucle], [canal], [volume])`  
  Joue un son `.wav`. 
  - `boucle` : Nombre de répétitions (-1 pour infini, 0 pour une seule lecture)
  - `canal` : Canal audio (0-31, ou -1 pour choix automatique)
  - `volume` : Volume de lecture (0-128, défaut: 64)

- `arreter(lien)`  
  Arrête la lecture d'un son spécifique.

- `arreter_canal(canal)`  
  Stoppe tous les sons sur un canal spécifique (0-31).

- `pause(lien)`  
  Met en pause un son spécifique.

- `pause_canal(canal)`  
  Met en pause tous les sons sur un canal spécifique.

- `reprendre(lien)`  
  Reprend la lecture d'un son mis en pause.

- `reprendre_canal(canal)`  
  Reprend la lecture de tous les sons mis en pause sur un canal spécifique.

### Entrées (`GrnGame.clavier`, `GrnGame.manette`)

#### Clavier

- `juste_presse(touche)`  
  Renvoie `True` si la touche vient d'être pressée ce frame. (Ex: "space", "a", "up")

- `enfoncee(touche)`  
  Renvoie `True` tant que la touche est maintenue.

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

#### Manette

- `init([index])`  
  Initialise la manette. `index` spécifie quelle manette utiliser (0 pour la première, défaut: 0).

- `enfoncee(bouton)`  
  Renvoie `True` si le bouton de la manette est maintenu. (Ex: "a", "b", "start")

- `juste_presse(bouton)`  
  Renvoie `True` si le bouton de la manette vient d'être pressé ce frame.

- `renvoie_joysticks([dead_zone])`  
  Renvoie les axes des sticks et gâchettes sous forme de liste `[lx, ly, rx, ry, lt, rt]`.
  - Valeurs entre -1.0 et 1.0 pour les sticks
  - Valeurs entre 0.0 et 1.0 pour les gâchettes
  - `dead_zone` : Seuil pour ignorer les petites déviations (défaut: 0.1)

- `fermer()`  
  Ferme la connexion avec la manette.

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

## Compilation

Pour créer un exécutable autonome de votre jeu :

```bash
# Créer un exécutable (Windows .exe ou Linux binaire)
GrnGame_app mon_jeu.py --noconsole --icon mon_icone.ico
```

L'exécutable sera généré dans le dossier courant

## Licence

GrnGame est sous licence [MIT License](LICENSE).

<div align="center">
Développé par Baptiste GUERIN
</div>