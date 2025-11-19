# GrnGame

<div align="center">

<img src="https://raw.githubusercontent.com/Baptistegrn/GrnGame/main/GrnGame/icone/iconex8.png" width="120" alt="GrnGame Icon">

**A powerful 2D Python game engine designed for pixel art**

[![Contributions Welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](https://github.com/Baptistegrn/GrnGame/issues)

[![Python Version](https://img.shields.io/badge/python-3.8+-blue.svg)](https://www.python.org/downloads/)
[![License](https://img.shields.io/badge/license-MIT-green.svg)](LICENSE)
[![PyPI](https://img.shields.io/badge/pypi-v1.0.0-orange.svg)](https://pypi.org/project/GrnGame/)

[Features](#-features) • [Installation](#-installation) • [Quick Start](#-quick-start) • [Documentation](#-main-api) • [Examples](#-examples)

</div>

---

## ✨ Features

**GrnGame is a lightweight, performant, and simple library for creating 2D games in Python:**

* 🖼️ **Optimized rendering** - Sprite management with batch rendering
* 🔊 **Multi-channel audio** - 32 simultaneous audio channels
* ⌨️ **Complete input** - Keyboard, mouse, and gamepad support
* 🎮 **Gamepad support** - Intuitive API for controllers
* 🚀 **High performance** - Optimized for pixel art and retro games
* 🪟 **Cross-platform** - Compatible with Windows and Linux
* 🆑 **Custom fonts** - Custom fonts without TTF files
* ⚙️ **Compilation** - Ability to compile to executable
* ❌ **Error handling** - Error system with .log file

---

## 🎬 Examples

<div align="center">

**Simple example game**

<img src="https://raw.githubusercontent.com/Baptistegrn/GrnGame/main/GrnGame/creations/example/example.gif" width="600">

**Space Attacks Demo**

<img src="https://raw.githubusercontent.com/Baptistegrn/GrnGame/main/GrnGame/creations/spaceattacks/space.gif" width="600">

</div>

---

## 📦 Installation

### Via PyPI

```bash
pip install GrnGame
```

### Import in your project

```python
import GrnGame
```

---

## 🚀 Quick Start

### First Program

```python
import GrnGame

def update():
    # Draw a sprite
    GrnGame.image.draw("./assets/player.png", 10, 10, 32, 32)
    
    # Detect input
    if GrnGame.keyboard.just_pressed("space"):
        GrnGame.song.play("./assets/jump.wav")

# Initialize the game
GrnGame.utils.initialize(
    width=160,                # Virtual resolution (width)
    height=90,                # Virtual resolution (height)
    fps=60,                   # Frames per second
    scale=4,                  # Window scale factor
    image_path="./assets",    # Image folder
    sound_path="./assets",    # Sound folder
    letterbox=True,           # Black bars if different ratio
    update_func=update,       # Function called each frame
    window_name="My Game",    # Window title
    error_path="err.log"      # Error log file path
)
```

---

## 📖 Main API

### 🎯 Global Properties

| Property | Type | Description |
|----------|------|-------------|
| `GrnGame.const.width` | `int` | Virtual resolution width |
| `GrnGame.const.height` | `int` | Virtual resolution height |
| `GrnGame.const.dt` | `float` | Delta time (time between frames) |
| `GrnGame.const.fps` | `float` | Current game FPS |
| `GrnGame.const.time` | `float` | Time elapsed since startup |
| `GrnGame.const.running` | `bool` | Game running state |
| `GrnGame.const.offset_x` | `int` | Horizontal offset in fullscreen |
| `GrnGame.const.offset_y` | `int` | Vertical offset in fullscreen |

---

### 🖱️ Mouse Input

```python
# Mouse position
x = GrnGame.const.mouse_x
y = GrnGame.const.mouse_y

# Left button
if GrnGame.const.mouse_pressed:            # Held down
    pass
if GrnGame.const.mouse_just_pressed:       # Just pressed
    pass

# Right button
if GrnGame.const.mouse_right_pressed:      # Held down
    pass
if GrnGame.const.mouse_right_just_pressed: # Just pressed
    pass
```

---

### ⌨️ Keyboard Input
**KEYBOARD INPUTS ARE PHYSICAL KEYS FROM THE ENGLISH KEYBOARD: W equals Z for a French keyboard**

```python
# Detect a single press
if GrnGame.keyboard.just_pressed("space"):
    print("Space pressed!")

# Detect a held key
if GrnGame.keyboard.pressed("left"):
    position_x -= speed
```

<details>
<summary><b>📋 Complete list of supported keys</b></summary>

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

---

### 🎮 Gamepads (Controllers)

```python
# Initialize a gamepad
if GrnGame.controller.init(0):  # 0 = first connected gamepad
    print("Gamepad connected!")

# Detect buttons
if GrnGame.controller.just_pressed("a"):
    print("A button pressed!")

if GrnGame.controller.pressed("x"):
    print("X button held")

# Read joysticks and triggers
axes = GrnGame.controller.renvoie_joysticks(dead_zone=0.15)
if axes:
    stick_l_x, stick_l_y, stick_r_x, stick_r_y, trigger_l, trigger_r = axes
    
    # Movement with left stick
    position_x += stick_l_x * speed
    position_y += stick_l_y * speed

# Close the gamepad (important before quitting)
GrnGame.controller.fermer()
```

<details>
<summary><b>🎮 Supported gamepad buttons</b></summary>

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

---

### 🖼️ Graphics Rendering

#### Drawing Shapes

```python
# Single shape
GrnGame.image.draw_shape(
    x=10, y=20,
    w=32, h=32,
    direction=0,       # 0=normal, 1=horizontal flip
    rotation=0,        # Rotation in degrees (0-360)
    transparency=255,  # 0-255
    r=255, g=255, b=255  # RGB color
)

# Batch rendering (more efficient for multiple shapes)
GrnGame.image.draw_shape_batch(
    xs=[0, 32, 64],
    ys=[0, 0, 16],
    ws=[32, 32, 48],
    hs=[32, 32, 48],
    directions=[0, 0, 1],         # Optional (default: 0)
    rotations=[0, 0, 90],         # Optional (default: 0)
    transparencies=[255, 255, 255],  # Optional (default: 255)
    rs=[255, 255, 100],           # Optional (default: 255)
    gs=[255, 100, 100],           # Optional (default: 255)
    bs=[255, 100, 100]            # Optional (default: 255)
)
```

#### Drawing Images

```python
# Single image
GrnGame.image.draw(
    path="./assets/sprite.png",
    x=10, y=20,
    w=32, h=32,
    direction=0,       # 0=normal, 1=horizontal flip
    rotation=0,        # Rotation in degrees (0-360)
    transparency=255   # 0-255
)

# Batch rendering (more efficient for multiple sprites)
GrnGame.image.draw_batch(
    ids=["./assets/tile.png", "./assets/tile.png", "./assets/enemy.png"],
    xs=[0, 32, 64],
    ys=[0, 0, 16],
    ws=[32, 32, 48],
    hs=[32, 32, 48],
    directions=[0, 0, 1],        # Optional
    rotations=[0, 0, 90],        # Optional
    transparencies=[255, 255, 255]  # Optional
)
```

#### Displaying Text

```python
GrnGame.image.draw_word(
    path="./assets/font",    # Path to font
    word="Score: 1000",
    x=10, y=10,
    scale=1,      # Text scale
    spacing=1,    # Character spacing
    direction=0,  # 0=normal, 1=flip
    rotation=0    # Rotation in degrees
)
```
> 💡 **Tip**: Elements are rendered in call order. The last drawn element appears in the foreground.

#### Debug Console

```python
GrnGame.utils.write_console("Debug message")
```

---

### 🔊 Audio Management

```python
# Play a sound
GrnGame.song.play(
    path="./assets/explosion.wav",
    loop=0,    # -1=infinite, 1+=number of repetitions starting from 1
    channel=-1,  # -1=auto, or 0-31 = specific channel
    volume=64  # Volume level (0-128)
)

# Channel control
GrnGame.song.stop_channel(5)
GrnGame.song.pause_channel(5)
GrnGame.song.resume_channel(5)

# Individual sound control
GrnGame.song.stop("./assets/music.wav")
GrnGame.song.pause_sound("./assets/music.wav")
GrnGame.song.resume("./assets/music.wav")
```

**Features**:
- 32 simultaneous audio channels (0-31)
- Supported format: WAV
- `loop=-1`: infinite loop playback
- `loop=n`: repeats n times then stops

---

### 🎨 Window Management

```python
# Toggle fullscreen / windowed
GrnGame.utils.resize_window()

# Stop the game properly
GrnGame.utils.stop_game()
```

---

## ⚙️ 2D Physics

### 2D Platformer

To implement 2D platformer physics with gravity, jumping, and wall collisions:

```python
from GrnGame.hitbox import platformer_2d

# In the game loop
pos_x, pos_y, velocity_y, is_in_air, collision_left, collision_right = platformer_2d(
    dt=GrnGame.const.dt,           # Delta time
    pos_x=player_x,                # Player X position
    pos_y=player_y,                # Player Y position
    larg_joueur=16,                # Player width
    haut_joueur=16,                # Player height
    vitesse_y=velocity_y,          # Current Y velocity
    est_en_air=is_in_air,          # Player airborne state
    blocs=blocks,                  # List of collision blocks [x, y, w, h]
    gravite=400.0,                 # Gravity (pixels/s²)
    force_saut=-200.0,             # Jump force (negative = upward)
    vitesse_max_chute=500.0,       # Max fall speed
    correction_mur=100.0,          # Velocity bonus when climbing walls
    touches_clavier_saut=["z"],    # Keyboard keys for jumping
    touches_manette_saut=["Y"],    # Gamepad buttons for jumping
    joy_saut=False                 # Enable jump via joystick
)

# Use the results
player_x = pos_x
player_y = pos_y
is_in_air = is_in_air
if collision_left or collision_right:
    print("Wall collision!")
```

**Block structure**:
- `blocks = [[x, y, width, height], ...]`
- Each block is a static collision rectangle

**Default values**:
- `gravite=400.0`: Adjust fall speed
- `force_saut=-200.0`: Negative to jump upward
- `vitesse_max_chute=500.0`: Fall speed limit
- `correction_mur=100.0`: Slows down if wall friction

---

## 🅰️ Creating a Custom Font

GrnGame uses bitmap fonts (images) rather than TTF files.

### Required Structure

```
assets/
└── font/
    ├── 32.png    # Space (ASCII code 32)
    ├── 33.png    # !
    ├── 48.png    # 0
    ├── 49.png    # 1
    ├── 65.png    # A
    ├── 66.png    # B
    ├── 97.png    # a
    └── ...
```

### Important Rules

1. **Filename** = ASCII code of the character
   - `65.png` = letter "A"
   - `97.png` = letter "a"
   - `48.png` = digit "0"

2. **Supported characters**: Standard ASCII (0-127)
   - `32.png` = space
   - `48-57.png` = digits 0-9
   - `65-90.png` = uppercase A-Z
   - `97-122.png` = lowercase a-z

### Usage Example

```python
GrnGame.image.draw_word(
    path="./assets/font",
    word="Score: 1234",
    x=10, y=10,
    scale=2,     # Size ×2
    spacing=1,   # 1 pixel between letters
    direction=0,
    rotation=0
)
```

### Creating Multiple Styles

```
assets/
├── font_pixel/      # Pixel art font
├── font_outline/    # Outline font
└── font_neon/       # Neon style font
```

```python
# Switch fonts dynamically
GrnGame.image.draw_word("./assets/font_outline", "GAME OVER", 40, 40, scale=3)
```

---

## 📚 Recommended Project Structure

```
my_game/
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

## 💻 Complete Example

```python
import GrnGame

# Global variables
player_x = 64
player_y = 40
player_speed = 50.0  # pixels per second
score = 0
gamepad_initialized = False

def update():
    global player_x, player_y, score, gamepad_initialized
    
    if not gamepad_initialized:
        gamepad_initialized = GrnGame.controller.init(0)

    # Smooth movement with delta time
    movement = player_speed * GrnGame.const.dt
    
    if GrnGame.keyboard.pressed("left"):
        player_x -= movement
    if GrnGame.keyboard.pressed("right"):
        player_x += movement
    if GrnGame.keyboard.pressed("up"):
        player_y -= movement
    if GrnGame.keyboard.pressed("down"):
        player_y += movement
    
    # Clamp to screen edges
    player_x = max(0, min(player_x, GrnGame.const.width - 16))
    player_y = max(0, min(player_y, GrnGame.const.height - 16))
    
    # Action with space
    if GrnGame.keyboard.just_pressed("space"):
        GrnGame.utils.resize_window()
        GrnGame.song.play("./assets/shoot.wav", loop=1, channel=1)
        score += 10
    
    # Gamepad support
    axes = GrnGame.controller.renvoie_joysticks(dead_zone=0.2)
    if axes:
        stick_x, stick_y = axes[0], axes[1]
        player_x += stick_x * movement
        player_y += stick_y * movement
        
        if GrnGame.controller.just_pressed("a"):
            GrnGame.song.play("./assets/shoot.wav", loop=1, channel=1)
            score += 10
    
    # Rendering
    GrnGame.image.draw("./assets/player.png", player_x, player_y, 16, 16)
    GrnGame.image.draw_word("./assets/font", f"Score: {score}", 5, 5, 1, 1)
    dt = GrnGame.const.dt
    if dt != 0:
        fps = int(1/GrnGame.const.dt)
    else:
        fps = 0
    GrnGame.image.draw_word("./assets/font", f"FPS: {fps}", 5, 15, 1, 1)
    
    # Quit
    if GrnGame.keyboard.just_pressed("escape"):
        GrnGame.utils.stop_game()

# Configuration and launch
GrnGame.utils.initialize(
    width=160,
    height=90,
    fps=60,
    scale=6,
    image_path="./assets",
    sound_path="./assets",
    letterbox=True,
    update_func=update,
    window_name="My Game"
)
```

---

## 🔧 Creating an Executable

### Compilation with PyInstaller

```bash
# Basic compilation
GrnGame_app your_file.py

# Without console (Windows only)
GrnGame_app your_file.py --noconsole

# With custom icon
GrnGame_app your_file.py --icon path/to/icon.ico

# Combination of options
GrnGame_app your_file.py --noconsole --icon my_icon.ico
```

**Available options**:
- `--noconsole`: Hides the Windows console (black window)
- `--icon`: Sets the executable icon (.ico file)

**Result**:
- The executable is generated in `./dist/`
- On Windows: `.exe` file
- On Linux: executable binary

**Prerequisites**:
- [PyInstaller](https://pyinstaller.org/) installed: `pip install pyinstaller`

**Notes**:
- The executable is **OS-specific** to the compilation system
- On Linux post-xmake compilation, prefer a standard distribution (Ubuntu, Debian) for portability

---

## ⚠️ Less Common Operating Systems

If your system is not directly compatible, GrnGame will automatically compile the native library via [xmake](https://xmake.io).

Here are the steps to avoid problems:

1. **Install xmake automatically**:
   ```bash
   GrnGame_xmake
   ```

2. **Restart your console/terminal** (important to reload environment variables)

3. **Launch your project**:
   ```bash
   python your_file.py
   ```

**Important**:
- The `GrnGame_xmake` command installs [xmake](https://xmake.io) which is necessary to compile the native library
- **You must restart the console** after installation for xmake to be recognized
- This step is only necessary once per system

---

## 🐛 Troubleshooting

### Library won't load
- ✅ Check that the DLL/SO is in `GrnGame/dist/`
- ✅ Install `xmake` for automatic compilation

### Images don't display
- ✅ Check paths (relative to main script)
- ✅ Only use PNG files
- ✅ Ensure `image_path` points to the correct folder

### Sounds don't work
- ✅ Only use WAV files
- ✅ Check audio file paths
- ✅ Don't exceed 32 simultaneous channels

### Performance issues
- ✅ Use `draw_batch()` for multiple sprites
- ✅ Reduce the number of `draw()` calls per frame
- ✅ Optimize sprite sizes

### Gamepad doesn't work
- ✅ Call `init()` **after** `GrnGame.initialize()`
- ✅ Call `fermer()` before quitting
- ✅ Check that the gamepad is connected before launch

---

## 📧 Contact & Support

**Author**: Baptiste GUERIN  
**Email**: [baptiste.guerin34@gmail.com](mailto:baptiste.guerin34@gmail.com)

To report a bug or suggest an improvement, feel free to contact me!

---

## 📄 License

This project is under the **MIT** license.

---

<div align="center">

**GrnGame** - A powerful 2D Python game engine designed for pixel art

[🌐 GitHub](https://github.com/Baptistegrn/GrnGame) • [📦 PyPI](https://pypi.org/project/GrnGame/) • [✉️ Contact](mailto:baptiste.guerin34@gmail.com)

</div>