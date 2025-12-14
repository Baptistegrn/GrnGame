<div align="center">
<img src="https://raw.githubusercontent.com/Baptistegrn/GrnGame/main/GrnGame/icone/iconex8.png" width="150" alt="GrnGame Icon">

<h1>GrnGame</h1>

[![PyPI](https://img.shields.io/badge/pypi-v1.0.0-orange.svg)](https://pypi.org/project/GrnGame/)
[![Python Version](https://img.shields.io/badge/python-3.11+-blue.svg)](https://www.python.org/downloads/)
[![License](https://img.shields.io/badge/license-MIT-green.svg)](../LICENSE)
[![Contributions Welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](https://github.com/Baptistegrn/GrnGame/issues)

[ [Français](../README.md) | **English** ]

</div>

**GrnGame** is a high-performance 2D game engine for Python, specifically designed for pixel art.

**PYTHON SUPPORTED: 3.11, 3.12, 3.13, 3.14**

With optimized sprite management (batch rendering) and an intuitive API inspired by pyxel and pygame, you can easily create smooth and lightweight games.

<div align="center">
<img src="https://raw.githubusercontent.com/Baptistegrn/GrnGame/main/GrnGame/creations/gif/example.gif" width="45%">
<img src="https://raw.githubusercontent.com/Baptistegrn/GrnGame/main/GrnGame/creations/gif/space.gif" width="35%">
<img src="https://raw.githubusercontent.com/Baptistegrn/GrnGame/main/GrnGame/creations/gif/platformer.gif" width="35%">
</div>

## Specifications

- Works on Windows and Linux
- Programming in Python 3.11+
- Optimized rendering with Batch Rendering
- Multi-channel audio (32 simultaneous channels)
- Complete inputs (Keyboard, Mouse, Gamepads)
- Custom font support (Bitmap fonts)
- Graphics primitives (circle, rectangle, line, triangle)
- Customizable color shaders
- Integrated 2D physics (Platformer)
- Easy compilation to executable

## Installation

### Via PyPI

```bash
pip install GrnGame
```

### From Sources

If your system requires native compilation (e.g., specific Linux), GrnGame will automatically use `xmake`.

```bash
# 1. Install xmake if necessary
GrnGame_xmake

# 2. Restart your terminal, then run your script
python main.py
```

## Quick Start

In your Python script, import `GrnGame`, define an `update` function, and initialize the engine.

```python
import GrnGame

def update():
    # Draw a sprite
    GrnGame.image.draw("./assets/player.png", 10, 10, 32, 32)
    
    # Keyboard inputs
    if GrnGame.keyboard.just_pressed("space"):
        GrnGame.song.play("./assets/jump.wav")
        
    # Quit
    if GrnGame.keyboard.just_pressed("escape"):
        GrnGame.utils.stop_game()

# Initialize the game
GrnGame.utils.initialize(
    width=160,               # Virtual resolution
    height=90,               # Virtual resolution
    fps=60,                  # Target frames per second
    scale=4,                 # Window scale
    image_path="./assets",   # Root folder for images
    sound_path="./assets",   # Root folder for sounds
    update_func=update,      # Loop function
    window_name="My Game",
    error_path="errors/err.log" # Path to error logs
)
```

## Available Modules

GrnGame exposes the following modules:

- `GrnGame.utils` - System utilities
- `GrnGame.const` - Game constants
- `GrnGame.image` - Graphics rendering
- `GrnGame.keyboard` (or `GrnGame.clavier`) - Keyboard input
- `GrnGame.controller` (or `GrnGame.manette`) - Gamepad input
- `GrnGame.song` (or `GrnGame.son`) - Audio management
- `GrnGame.platformer_2d` - 2D physics for platformers

## API Reference

### System & Constants (`GrnGame.const`, `GrnGame.utils`)

#### Constants

- `width`, `height` (`largeur`, `hauteur`)  
  The width and height of the virtual resolution.

- `dt`  
  Time elapsed between two frames (Delta Time).

- `fps`  
  Current frames per second.

- `time`  
  Total number of frames elapsed since startup.

- `mouse_x`, `mouse_y` (`souris_x`, `souris_y`)  
  Mouse position relative to virtual screen.

- `mouse_pressed` (`mouse_presse`)  
  Returns `True` if left mouse button is pressed.

- `mouse_just_pressed` (`mouse_juste_presse`)  
  Returns `True` if left mouse button was just pressed this frame.

- `mouse_right_pressed` (`mouse_droit_presse`)  
  Returns `True` if right mouse button is pressed.

- `mouse_right_just_pressed` (`mouse_droit_juste_presse`)  
  Returns `True` if right mouse button was just pressed this frame.

- `offset_x`, `offset_y` (`decalage_x`, `decalage_y`)  
  Current camera offset (top-left corner coordinates).

- `running` (`en_marche`)  
  Returns `True` if the game is running.

#### Utilities

- `initialize(width, height, fps, scale, image_path, sound_path, letterbox, update_func, window_name, error_path)`  
  (`init(largeur, hauteur, fps, coeff, chemin_image, chemin_son, bande_noir, update_func, nom_fenetre, chemin_erreur)`)  
  Initialize the window and engine.

- `stop_game()` (`stopper_jeu()`)  
  Properly close the engine and window.

- `resize_window()` (`redimensionner_fenetre()`)  
  Toggle between windowed and fullscreen mode.

- `colorize(r, g, b)` (`colorier(r, g, b)`)  
  Apply an RGB color tint to all subsequent renders.

- `show_cursor(show=True)` (`afficher_curseur(afficher=True)`)  
  Show or hide the mouse cursor.

- `write_log(error_type, message)` (`ecrire_log(type_erreur, message)`)  
  Write a message to log files. Available types: `"info"`, `"debug"`, `"warning"` (`"avertissement"`), `"error"` (`"erreur"`).

- `change_log(level)` (`changer_log(niveau)`)  
  Change the logging level. Available levels: `"info"`, `"debug"`, `"warning"` (`"avertissement"`), `"error"` (`"erreur"`).

- `platformer_2d(dt, pos_x, pos_y, velocity_y, in_air, player_width, player_height, blocks, [gravity], [jump_force], [max_fall_speed], [wall_correction], [activate_jump])`  
  Integrated 2D physics for platform games. Handles gravity, jumps, and block collisions. Returns tuple `(new_pos_x, new_pos_y, new_velocity_y, new_in_air)`.
  - `dt`: Delta time (use `GrnGame.const.dt`)
  - `pos_x`, `pos_y`: Current player position
  - `velocity_y`: Current vertical velocity
  - `in_air`: `True` if player is airborne
  - `player_width`, `player_height`: Player dimensions
  - `blocks`: List of rectangles `[(x, y, w, h), ...]` representing collisions
  - `gravity`: Gravity force (default: 400.0)
  - `jump_force`: Jump force, negative value (default: -200.0)
  - `max_fall_speed`: Maximum fall speed (default: 500.0)
  - `wall_correction`: Wall collision correction distance (default: 100.0)
  - `activate_jump`: `True` to perform a jump this frame

### Graphics (`GrnGame.image`)

#### Sprites and Images

- `draw(path, x, y, w, h, [direction], [rotation], [transparency])`  
  (`dessiner(lien, x, y, w, h, [sens], [rotation], [transparence])`)  
  Draw an image at given position. `direction` allows flipping (0=normal, 1=horizontal mirror).

- `draw_batch(ids, xs, ys, ws, hs, [directions], [rotations], [transparency])`  
  (`dessiner_batch(ids, xs, ys, ws, hs, [sens], [rotations], [transparence])`)  
  Draw a list of images in a single GPU call for maximum performance.

#### Graphics Primitives

<div align="center">

| Primitive | Demo |
|-----------|------|
| **Circle** | <video src="https://raw.githubusercontent.com/Baptistegrn/GrnGame/main/GrnGame/demo/mp4/cercle.mp4" width="200" controls></video> |
| **Rectangle** | <video src="https://raw.githubusercontent.com/Baptistegrn/GrnGame/main/GrnGame/demo/mp4/rectangle.mp4" width="200" controls></video> |
| **Triangle** | <video src="https://raw.githubusercontent.com/Baptistegrn/GrnGame/main/GrnGame/demo/mp4/triangle.mp4" width="200" controls></video> |
| **Line** | <video src="https://raw.githubusercontent.com/Baptistegrn/GrnGame/main/GrnGame/demo/mp4/ligne.mp4" width="200" controls></video> |

</div>

- `draw_line(x1, y1, x2, y2, [r], [g], [b])`  
  (`dessiner_ligne(x1, y1, x2, y2, [r], [g], [b])`)  
  Draw a line between two points with RGB color (default: white).

- `draw_rectangle(x, y, w, h, [r], [g], [b], [filled])`  
  (`dessiner_rectangle(x, y, w, h, [r], [g], [b], [plein])`)  
  Draw a rectangle. `filled=True` for filled rectangle, `False` for outline.

- `draw_circle(x, y, radius, [r], [g], [b], [filled])`  
  (`dessiner_cercle(x, y, rayon, [r], [g], [b], [plein])`)  
  Draw a circle centered at (x, y). `filled=True` for disk, `False` for circle.

- `draw_triangle(x1, y1, x2, y2, [r], [g], [b], [filled])`  
  (`dessiner_triangle(x1, y1, x2, y2, [r], [g], [b], [plein])`)  
  Draw a triangle with three vertices. `filled=True` for filled triangle.

#### Text and Fonts

- `draw_word(path, word, x, y, scale, spacing, [direction], [rotation], [alpha])`  
  (`dessiner_mot(lien_police, mot, x, y, coeff, ecart, [sens], [rotation], [alpha])`)  
  Display text using bitmap font (folder of PNG images).
  - `path`: Path to folder containing characters (e.g., `"./fonts/my_font/"`)
  - `word`: Text to display
  - `x`, `y`: Text position
  - `scale`: Text scale
  - `spacing`: Character spacing
  - `direction`: Horizontal flip (0=normal, 1=mirror)
  - `rotation`: Rotation in degrees
  - `alpha`: Transparency (0-255)

#### Shaders and Effects

- `apply_shaders(xs, ys, [rs], [gs], [bs], [alphas])`  
  (`appliquer_shaders(xs, ys, [rs], [gs], [bs], [transparences])`)  
  Apply color shaders to multiple points at once.
  - `xs`, `ys`: Lists of positions
  - `rs`, `gs`, `bs`: Lists of RGB components (default: [155, 155, 20])
  - `alphas`: List of transparency values (default: [125])

#### Resource Management

- `load_all_textures(folder)` (`charger_toutes_les_textures(dossier)`)  
  Preload all images from a folder into memory for better performance.

- `load_all_sounds(folder)` (`charger_tous_les_sons(dossier)`)  
  Preload all sounds from a folder into memory.

- `free_image_manager()` (`liberer_gestionnaire_image()`)  
  Free all loaded textures from memory.

- `free_sound_manager()` (`liberer_gestionnaire_son()`)  
  Free all loaded sounds from memory.

#### Creating a Custom Bitmap Font

To create your own bitmap font:

1. **Create a folder** for your font (e.g., `fonts/my_font/`)

2. **Add your character images**:
   - One PNG file per character
   - Name each file with its **ASCII code** (e.g., `65.png` for 'A', `97.png` for 'a')
   - **All images should have the same size** for optimal rendering

3. **Example structure**:
   ```
   fonts/
   └── my_font/
       ├── 32.png   (space)
       ├── 33.png   (!)
       ├── 65.png   (A)
       ├── 97.png   (a)
       └── ...
   ```

4. **Usage**:
   ```python
   # The path is simply the folder name
   GrnGame.image.draw_word("fonts/my_font", "Hello!", 10, 10, 2, 1)
   ```

**Useful ASCII codes**:
- `32`: Space
- `33-47`: Punctuation (!, ", #, $, %, etc.)
- `48-57`: Numbers (0-9)
- `65-90`: Uppercase letters (A-Z)
- `97-122`: Lowercase letters (a-z)

### Audio (`GrnGame.song` or `GrnGame.son`)

- `play(path, [loop], [channel], [volume])`  
  (`jouer(lien, [boucle], [canal], [volume])`)  
  Play a `.wav` sound.
  - `loop`: Number of repetitions (-1 for infinite, 0 for single playback)
  - `channel`: Audio channel (0-31, or -1 for automatic choice)
  - `volume`: Playback volume (0-128, default: 64)

- `stop(path)` (`arreter(lien)`)  
  Stop playback of a specific sound.

- `stop_channel(channel)` (`arreter_canal(canal)`)  
  Stop all sounds on a specific channel (0-31).

- `pause_sound(path)` (`pause(lien)`)  
  Pause a specific sound.

- `pause_channel(channel)` (`pause_canal(canal)`)  
  Pause all sounds on a specific channel.

- `resume(path)` (`reprendre(lien)`)  
  Resume playback of a paused sound.

- `resume_channel(channel)` (`reprendre_canal(canal)`)  
  Resume playback of all paused sounds on a specific channel.

### Inputs (`GrnGame.keyboard` / `GrnGame.clavier`, `GrnGame.controller` / `GrnGame.manette`)

#### Keyboard

- `just_pressed(key)` (`juste_presse(touche)`)  
  Returns `True` if key was just pressed this frame. (Ex: "space", "a", "up")

- `pressed(key)` (`enfoncee(touche)`)  
  Returns `True` while key is held down.

<details>
<summary><b>Complete list of supported keys</b></summary>

**Letters**: `a` to `z`

**Numbers**: `0` to `9`

**Navigation**:
- Arrows: `up`, `down` (or `dn`), `left` (or `lt`), `right` (or `rt`)
- Page: `home`, `end`, `pageup` (or `pgup`), `pagedown` (or `pgdn`)

**System**:
- `space`, `enter` (or `return`), `tab`, `backspace`
- `escape` (or `esc`), `delete`, `insert`

**Modifiers**:
- Shift: `shift`, `lshift`, `rshift`
- Ctrl: `ctrl` (or `control`), `lctrl`, `rctrl`
- Alt: `alt`, `lalt`, `ralt`
- Caps: `caps`, `capslock`
- `numlock`, `scrolllock`

**Function keys**: `f1` to `f12`

**Special**:
- `pause`, `break`, `print` (or `prtscr`, `printscreen`)
- `sysreq`, `menu`, `application`

**GUI/System**:
- Windows: `lwin`, `rwin`, `lgui`, `rgui`
- Super: `lsuper`, `rsuper`
- Mac: `lcmd`, `rcmd`

**Numpad**:
- Numbers: `kp0` to `kp9`
- Operators: `kp+` (or `kpplus`), `kp-` (or `kpminus`), `kp*` (or `kpmultiply`), `kp/` (or `kpdivide`)
- Others: `kp=` (or `kpequals`), `kp.` (or `kpperiod`), `kpenter` (or `kpe`)

**Media**:
- Volume: `mute` (or `audiomute`), `volumeup` (or `volup`, `audioup`), `volumedown` (or `voldown`, `audiodown`)
- Control: `play` (or `audioplay`, `mediaplay`, `playpause`), `stop` (or `audiostop`, `mediastop`)
- Navigation: `next` (or `audionext`, `medianext`), `previous` (or `prev`, `audioprev`, `mediaprev`)

**Browser**:
- `browserback`, `browserfwd`, `browserstop`
- `browserhome`, `browserrefresh`, `browsersearch`

**Power**: `power`, `sleep`

**Special characters**: `-`, `=`, `[`, `]`, `;`, `'`, `,`, `.`, `/`, `` ` ``, `\`

</details>

#### Gamepad

- `initialize([index])` (`init([index])`)  
  Initialize gamepad. `index` specifies which gamepad to use (0 for first, default: 0).

- `pressed(button)` (`enfoncee(bouton)`)  
  Returns `True` if gamepad button is held down. (Ex: "a", "b", "start")

- `just_pressed(button)` (`juste_presse(bouton)`)  
  Returns `True` if gamepad button was just pressed this frame.

- `get_joysticks([dead_zone])` (`renvoie_joysticks([dead_zone])`)  
  Returns stick axes and triggers as list `[lx, ly, rx, ry, lt, rt]`.
  - Values between -1.0 and 1.0 for sticks
  - Values between 0.0 and 1.0 for triggers
  - `dead_zone`: Threshold to ignore small deviations (default: 0.1)

- `close()` (`fermer()`)  
  Close gamepad connection.

<details>
<summary><b>Supported gamepad buttons</b></summary>

**Face buttons**: `a`, `b`, `x`, `y`

**System**: `start`, `back`, `select`, `guide`, `home`, `share`

**Bumpers**: `lb`, `rb`, `l1`, `r1`

**Clickable sticks**: `l3`, `r3`

**D-Pad**: `up`, `down`, `left`, `right`

**Additional**: `paddle1`, `paddle2`, `paddle3`, `paddle4`, `touchpad`

**Axis values**:
- Returns 6 float values between -1.0 and 1.0
- `dead_zone`: threshold to ignore small deviations (default: 0.1)
- Order: left stick X/Y, right stick X/Y, left trigger, right trigger

</details>

## Code Examples

### Drawing Primitives

```python
import GrnGame

def update():
    # Red filled circle
    GrnGame.image.draw_circle(50, 50, 20, 255, 0, 0, True)
    
    # Blue empty rectangle (outline)
    GrnGame.image.draw_rectangle(100, 100, 40, 30, 0, 0, 255, False)
    
    # Green line
    GrnGame.image.draw_line(10, 10, 100, 100, 0, 255, 0)
    
    # Yellow triangle
    GrnGame.image.draw_triangle(150, 50, 180, 80, 255, 255, 0, True)

GrnGame.utils.initialize(width=200, height=150, fps=60, update_func=update)
```

### Batch Rendering for Performance

```python
import GrnGame

# Position and size of 1000 sprites
xs = [i * 10 for i in range(1000)]
ys = [50] * 1000
ws = [16] * 1000
hs = [16] * 1000
ids = ["./assets/particle.png"] * 1000

def update():
    # Draw 1000 sprites in a single GPU call
    GrnGame.image.draw_batch(ids, xs, ys, ws, hs)

GrnGame.utils.initialize(width=320, height=180, fps=60, update_func=update)
```

## Compilation

To create a standalone executable of your game:

```bash
# Create executable (Windows .exe or Linux binary)
GrnGame_app my_game.py --noconsole --icon my_icon.ico
```

The executable will be generated in the current folder.

## License

GrnGame is licensed under the [MIT License](../LICENSE).

<div align="center">
Developed by Baptiste GUERIN
</div>