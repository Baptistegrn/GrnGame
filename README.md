# GrnGame Documentation

---

## Getting Started

GrnGame runs on Windows 7+ 64bit, Ubuntu Linux 64bit, macOS 64bit M1+.
Everything is pre-compiled.

**Create a project** — clone the repo, move into `commandProject` and run the project creation script:

```
python create_project.py <name> [path] [--level debug|release]
```

The project contains:
- `GrnGameAppLinux` / `GrnGameAppWindows` / `GrnGameAppMacOs` — executables
- `src/main.lua` — entry point
- `src/grngame_lua.api` — API definitions
- `src/encrypt.py` — resource encryption tool

> ⚠️ The working directory is the **execution directory**, not the location of `main.lua`.

---

## Distribution

**Encrypt resources** (images, sounds, JSON) with `encrypt.py`:

```
python encrypt.py <key_hex_32chars> [path] <iv_hex_32chars>
```

Encrypted files must use the `.data` extension. Set the key/IV before loading — they are valid for one frame only.

**Obfuscate your code** by bundling all dependencies into `main.lua` and compiling to Lua 5.4.7 bytecode.

---

## utils

```lua
utils.setUpdateCallback(fn)     -- Set the main update function (called every frame)
utils.switchUpdateCallback(fn)  -- Switch the update function
utils.logMessage(level, msg)    -- Log a message (0=debug 1=info 2=warning 3=error)
utils.stopEngine()              -- Stop the engine
utils.setLogLvl(level)         -- Set minimum log level
utils.getInputText()            -- Get current text input → string
utils.deleteInputText()         -- Clear text input
utils.setWindowSize(w, h)      -- Set universe size
```

---

## window

```lua
window.fullscreen()             -- Exclusive fullscreen
window.windowedFullscreen()     -- Borderless fullscreen
window.windowed(w, h)          -- Windowed mode
window.windowedCoefficient(n)  -- Windowed mode with zoom coefficient
window.setFps(fps)             -- Set target FPS
window.setTitle(title)         -- Set window title
window.setBlackBars(bool)      -- Enable/disable black bars
window.setUniversSize(w, h)    -- Set universe size
```

---

## mouse

```lua
mouse.x()                   -- Mouse X in universe → number
mouse.y()                   -- Mouse Y in universe → number
mouse.justPressed(btn)      -- Just pressed this frame → boolean
mouse.pressed(btn)          -- Held down → boolean
mouse.scrollVertical()      -- Vertical scroll → integer
mouse.scrollHorizontal()    -- Horizontal scroll → integer
```

---

## input

```lua
input.keyJustPressed(key)           -- Key just pressed → boolean
input.keyPressed(key)               -- Key held down → boolean
input.showCursor(bool)              -- Show/hide cursor
input.buttonJustPressed(btn, idx)   -- Controller button just pressed → boolean
input.buttonPressed(btn, idx)       -- Controller button held → boolean
input.initController(idx)           -- Initialize controller
input.getJoysticks(deadzone, idx)   -- Get axes {lx,ly,rx,ry,tl,tr} → table
input.closeController(idx)
input.closeJoystick(idx)
input.closeTheController(idx)
```

---

## var

```lua
var.delta()           -- Delta time in seconds → number
var.fps()             -- Current FPS → number
var.frameCount()      -- Total frames since start → integer
var.offsetX()         -- Render X offset → number
var.offsetY()         -- Render Y offset → number
var.currentWidth()    -- Window width → integer
var.currentHeight()   -- Window height → integer
var.universeWidth()   -- Universe width → integer
var.universeHeight()  -- Universe height → integer
var.windowMinimised() -- Is window minimized? → boolean
var.getTextDrop()     -- Dropped file path → string
```

---

## image

```lua
image.cls(r, g, b)                                              -- Clear screen
image.draw(path, x, y, coeff, [flip], [rotP], [rot], [alpha])  -- Draw image
image.drawSprite(sprite, idx, x, y, coeff, [flip], [rot], [alpha]) -- Draw sprite frame
image.drawText(font, text, x, y, scale, [flip], [spacing], [rotP], [rot], [alpha])
image.drawRect(x, y, w, h, [r,g,b,a])                          -- Rectangle outline
image.drawRectFilled(x, y, w, h, [r,g,b,a])                    -- Filled rectangle
image.drawCircle(x, y, radius, [r,g,b,a])                      -- Circle outline
image.drawCircleFilled(x, y, radius, [r,g,b,a])                -- Filled circle
image.drawLine(x1, y1, x2, y2, [r,g,b,a])                      -- Line
image.drawParticles(x, y, rot, a, r, g, b)                     -- Particle batch
image.Sprite(id, w, h)       -- Create Sprite object → Sprite
image.loadFolder(folder)     -- Preload all images in folder
image.freeFolder()           -- Free loaded images
image.setIcon(path)          -- Set window icon
image.setKey(index, value)   -- Set decryption key byte
image.setIv(index, value)    -- Set decryption IV byte
```

**Sprite object**

```lua
local spr = image.Sprite("sheet.png", 16, 16)
-- spr.width, spr.height
image.drawSprite(spr, frame, x, y, coeff)
```

---

## song

```lua
song.play(path, [loop], [channel], [volume])  -- Play sound (loop: 1=loop 0=once, vol: 0-128)
song.stop(path)
song.pause(path)
song.resume(path)
song.stopChannel(channel)
song.pauseChannel(channel)
song.resumeChannel(channel)
song.loadFolder(path)
song.freeFolder()
song.setKey(index, value)
song.setIv(index, value)
```

---

## json

```lua
json.load(filename)
json.save(filename)
json.delete(filename)
json.exists(filename)               -- → boolean
json.writeVariable(filename, path, value)
json.readVariable(filename, path)   -- path="" reads root → value/table/nil
json.deleteVariable(filename, path)
json.setKey(index, value)
json.setIv(index, value)
```

---

## game

**Camera**

```lua
game.createCamera(x, y, smooth, w, h)
game.updateCamera(tx, ty, dt)
game.setCameraX(v) / game.getCameraX()
game.setCameraY(v) / game.getCameraY()
game.setCameraSmooth(v) / game.getCameraSmooth()
```

**Blocks**

```lua
local b = game.Block(x, y, w, h, type)     -- Create a block
local bs = game.Blocks()                   -- Create block container
bs:add(block)
bs:size()
bs:get(index)
bs:pairs()

local map = game.BlocksFromFile(path, step_x, step_y, separator, exclude)
map:size()
```

**Entities (Platformer)**

```lua
local e = game.EntityPlatformer(x, y, w, h, [jumpPower], [gravity], [speedMaxX],
          [speedMaxFall], [wallCorrection], [initialSpeed], [acceleration],
          [numJumps], [canJumpOnWall])
```

Fields: `x y w h speedX speedY inSky gravity jumpPower requestJump requestLeft requestRight leftLock rightLock initialSpeed acceleration numberOfJumps numberOfJumpsPossible jumpOnWall`

```lua
local es = game.EntitiesPlatformer()
es:add(entity)
es:size()
es:get(index)
es:pairs()
```

**Physics**

```lua
game.hitboxPlatformer(entities, blocks, [ignore_mask], [delta])
```

**Animation**

```lua
local frame, timer = game.animate(frame, timer, start, end, speed, dt, loop)
```

**Debug**

```lua
game.setDebugHitbox(bool)   -- Draw hitbox outlines
```

---

## Examples

### Hello World 

```lua
window.setTitle("Hello World")
window.windowed(320, 240)
window.setFps(60)
image.loadFolder("font/") --folder font contains all caracteres separeted and named by its code ascii see : ressources/font and /font2
local function update()
    image.cls(0, 0, 0)
    image.drawText("font/", "Hello World!", 90, 100, 2)
end

utils.setUpdateCallback(update)
```

---

### Draw & Quit on Escape

```lua
window.windowed(400, 300)
window.setFps(60)
utils.setWindowSize(400, 300)

local function update()
    image.cls(30, 30, 30)

    image.drawRectFilled(50, 50, 100, 60, 255, 0, 0, 255)
    image.drawCircle(200, 150, 40, 0, 255, 0, 255)
    image.drawLine(0, 0, 400, 300, 255, 255, 0, 200)

    if input.keyJustPressed("escape") then
        utils.stopEngine()
    end
end

utils.setUpdateCallback(update)
```

---

### Sprite Animation

```lua
window.windowed(320, 240)
window.setFps(60)
utils.setWindowSize(320, 240)
image.loadFolder("src/")
local spr = image.Sprite("src/player.png", 16, 16)
local frame = 0
local timer = 0

local function update()
    image.cls(0, 0, 0)

    frame, timer = game.animate(frame, timer, 0, 5, 0.1, var.delta(), true)
    image.drawSprite(spr, frame, 100, 100, 3)
end

utils.setUpdateCallback(update)
```

---

### Platformer

```lua
window.windowed(320, 240)
window.setFps(60)
utils.setWindowSize(320, 240)
game.setDebugHitbox(true)

local blocks = game.Blocks()
blocks:add(game.Block(0, 200, 320, 40, 1))
blocks:add(game.Block(80, 160, 60, 16, 1))

local entities = game.EntitiesPlatformer()
local player = game.EntityPlatformer(100, 100, 16, 24, 5.0, 0.3)
entities:add(player)

game.createCamera(0, 0, 0.1, 320, 240)

local function update()
    image.cls(50, 50, 80)

    player.requestLeft  = input.keyPressed("left")
    player.requestRight = input.keyPressed("right")
    player.requestJump  = input.keyJustPressed("space")

    game.hitboxPlatformer(entities, blocks)
    game.updateCamera(player.x, player.y, var.delta())

    image.drawRectFilled(player.x, player.y, player.w, player.h, 0, 200, 255, 255)

    for _, b in blocks:pairs() do
        image.drawRectFilled(b.x, b.y, b.w, b.h, 150, 100, 50, 255)
    end
end

utils.setUpdateCallback(update)
```

---

### Load Map from File

```lua
-- map.txt example:
-- 1,1,1,1
-- 1,0,0,1
-- 1,0,0,1
-- 1,1,1,1

window.windowed(320, 240)
window.setFps(60)
utils.setWindowSize(320, 240)

local map = game.BlocksFromFile("src/map.txt", 16, 16, ",", 0)

local function update()
    image.cls(0, 0, 0)
    utils.logMessage(0, "blocks loaded: " .. map:size())
end

utils.setUpdateCallback(update)
```

---

### Save & Load JSON

```lua
window.windowed(320, 240)
window.setFps(60)
utils.setWindowSize(320, 240)

local score = 0
json.load("save.json")
if json.exists("save.json") then
    score = json.readVariable("save.json", "score") or 0
end

local function update()
    image.cls(0, 0, 0)
    image.drawText("src/font.png", "Score: " .. score, 10, 10, 2)

    if input.keyJustPressed("space") then
        score = score + 10
        json.writeVariable("save.json", "score", score)
        json.save("save.json")
    end

    if input.keyJustPressed("escape") then
        utils.stopEngine()
    end
end

utils.setUpdateCallback(update)
```

---

### Play Sound

```lua
window.windowed(320, 240)
window.setFps(60)
utils.setWindowSize(320, 240)

local played = false

local function update()
    image.cls(0, 0, 0)

    if input.keyJustPressed("space") and not played then
        song.play("src/music.wav", 1, 0, 80)
        played = true
    end

    if input.keyJustPressed("escape") then
        song.stopChannel(0)
        utils.stopEngine()
    end
end

utils.setUpdateCallback(update)
```

---

### Load Encrypted Image

```lua
-- Key and IV must be set the frame before loading

window.windowed(320, 240)
window.setFps(60)
utils.setWindowSize(320, 240)

local KEY = {0x2b,0x7e,0x15,0x16,0x28,0xae,0xd2,0xa6,0xab,0xf7,0x15,0x88,0x09,0xcf,0x4f,0x3c}
local IV  = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f}

local ready = false

local function update()
    if not ready then
        for i, v in ipairs(KEY) do image.setKey(i - 1, v) end
        for i, v in ipairs(IV)  do image.setIv(i - 1, v)  end
        ready = true
        return
    end

    image.cls(0, 0, 0)
    image.draw("src/player.data", 100, 100, 2)
end

utils.setUpdateCallback(update)
```