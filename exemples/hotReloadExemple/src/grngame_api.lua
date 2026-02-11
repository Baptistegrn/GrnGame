---@meta
-- Definition file for GrnGame API autocompletion.
-- Do not execute, used only by editors (VS Code, IntelliJ, etc.).

--
-- utils: engine utility functions
--

---@class utils
utils = {}

--- Initializes the game engine
---@param height? integer universe height (default: 320)
---@param width? integer universe width (default: 180)
---@param fps? number frames per second (default: 60.0)
---@param black_bars? boolean black bars (default: false)
---@param window_title? string window title (default: "My Game")
---@param update_func? function update function called each frame
function utils.initialize(height, width, fps, black_bars, window_title, update_func) end

--- Logs a message
---@param level integer log level (0=allocation,1=debug, 2=info, 3=warning, 4=error)
---@param message string message to display
function utils.logMessage(level, message) end

--- Stops the engine
function utils.stop() end

--- Switches to fullscreen mode
function utils.fullscreen() end

--- Switches to windowed fullscreen mode
function utils.windowedFullscreen() end

--- Switches to windowed mode with given size
---@param width integer width
---@param height integer height
function utils.windowed(width, height) end

--- Switches to minimized windowed mode with coefficient
---@param coeff integer resize coefficient
function utils.windowedMinimised(coeff) end

--- Clears the screen with a color
---@param r integer red (0-255)
---@param g integer green (0-255)
---@param b integer blue (0-255)
function utils.cls(r, g, b) end

--- Changes the log level
---@param level integer log level (0=debug, 1=info, 2=warning, 3=error)
function utils.setLogLvl(level) end

--
-- var: engine constants and states (read-only)
--

---@class var
var = {}

--- Mouse X position
---@type integer
var.mouseX = 0

--- Mouse Y position
---@type integer
var.mouseY = 0

--- Left button just pressed
---@type boolean
var.mouseLeftJustPressed = false

--- Right button just pressed
---@type boolean
var.mouseRightJustPressed = false

--- Left button held
---@type boolean
var.mouseLeftPressed = false

--- Right button held
---@type boolean
var.mouseRightPressed = false

--- Vertical scroll delta
---@type integer
var.mouseScrollVertical = 0

--- Horizontal scroll delta
---@type integer
var.mouseScrollHorizontal = 0

--- Delta time in seconds
---@type number
var.delta = 0

--- Current FPS
---@type number
var.fps = 0

--- Elapsed frame count
---@type integer
var.frameCount = 0

--- Viewport X offset
---@type integer
var.offsetX = 0

--- Viewport Y offset
---@type integer
var.offsetY = 0

--- Current window width
---@type integer
var.currentWidth = 0

--- Current window height
---@type integer
var.currentHeight = 0

--- Universe width
---@type integer
var.universeWidth = 0

--- Universe height
---@type integer
var.universeHeight = 0

--- Window minimized
---@type boolean
var.windowMinimised = false

--
-- input: keyboard and controller handling
--

---@class input
input = {}

--- Checks if a key was just pressed
---@param key string key name (e.g., "a", "space", "escape")
---@return boolean
function input.keyJustPressed(key) end

--- Checks if a key is held down
---@param key string key name
---@return boolean
function input.keyPressed(key) end

--- Shows or hides the cursor
---@param visible boolean true to show, false to hide
function input.showCursor(visible) end

--- Checks if a controller button was just pressed
---@param button string button name (e.g., "a", "b", "x", "y")
---@param index integer controller index (0-3)
---@return boolean
function input.buttonJustPressed(button, index) end

--- Checks if a controller button is held
---@param button string button name
---@param index integer controller index (0-3)
---@return boolean
function input.buttonPressed(button, index) end

--- Initializes a controller
---@param index integer controller index (0-3)
function input.initController(index) end

--- Gets joystick values
---@param dead_zone number dead zone (0.0 - 1.0)
---@param index integer controller index (0-3)
---@return table {left_x, left_y, right_x, right_y, trigger_left, trigger_right}
function input.getJoysticks(dead_zone, index) end

--- Closes a controller
---@param index integer controller index (0-3)
function input.closeController(index) end

--- Closes a joystick
---@param index integer controller index (0-3)
function input.closeJoystick(index) end

--- Closes the complete controller
---@param index integer controller index (0-3)
function input.closeTheController(index) end

--
-- image: graphics, drawing and sprites
--

---@class image
image = {}

--- Draws an image
---@param path string image path
---@param x number X position
---@param y number Y position
---@param coeff integer scale coefficient
---@param flip? boolean flip horizontally (default: false)
---@param rotationP? integer pixel perfect rotation in degrees (default: 0)
---@param rotation? integer rotation in degrees (default: 0)
---@param alpha? integer opacity 0-255 (default: 255)
function image.draw(path, x, y, coeff, flip, rotationP, rotation, alpha) end

--- Draws an empty rectangle
---@param x number X position
---@param y number Y position
---@param w integer width
---@param h integer height
---@param r? integer red (default: 0)
---@param g? integer green (default: 0)
---@param b? integer blue (default: 0)
---@param a? integer opacity (default: 255)
function image.drawRect(x, y, w, h, r, g, b, a) end

--- Draws a filled rectangle
---@param x number X position
---@param y number Y position
---@param w integer width
---@param h integer height
---@param r? integer red (default: 0)
---@param g? integer green (default: 0)
---@param b? integer blue (default: 0)
---@param a? integer opacity (default: 255)
function image.drawRectFilled(x, y, w, h, r, g, b, a) end

--- Draws a line
---@param x1 number start X
---@param y1 number start Y
---@param x2 number end X
---@param y2 number end Y
---@param r? integer red (default: 0)
---@param g? integer green (default: 0)
---@param b? integer blue (default: 0)
---@param a? integer opacity (default: 255)
function image.drawLine(x1, y1, x2, y2, r, g, b, a) end

--- Draws an empty circle
---@param x number center X
---@param y number center Y
---@param radius integer radius
---@param r? integer red (default: 0)
---@param g? integer green (default: 0)
---@param b? integer blue (default: 0)
---@param a? integer opacity (default: 255)
function image.drawCircle(x, y, radius, r, g, b, a) end

--- Draws a filled circle
---@param x number center X
---@param y number center Y
---@param radius integer radius
---@param r? integer red (default: 0)
---@param g? integer green (default: 0)
---@param b? integer blue (default: 0)
---@param a? integer opacity (default: 255)
function image.drawCircleFilled(x, y, radius, r, g, b, a) end

--- Draws text
---@param font_path string font path (spritesheet)
---@param text string text to display
---@param x number X position
---@param y number Y position
---@param scale integer scale
---@param flip? boolean flip (default: false)
---@param spacing? number spacing (default: 1)
---@param rotationP? integer pixel perfect rotation (default: 0)
---@param rotation? integer rotation (default: 0)
---@param alpha? integer opacity (default: 255)
function image.drawText(font_path, text, x, y, scale, flip, spacing, rotationP, rotation, alpha) end

--- Sets the window icon
---@param path string image path
function image.setIcon(path) end

--- Loads all images from a folder
---@param folder string folder path
function image.loadImageFolder(folder) end

--- Frees loaded images
function image.freeImageFolder() end

--- Draws particles
---@param x table X positions
---@param y table Y positions
---@param rotation table rotations
---@param a table opacities
---@param r table reds
---@param g table greens
---@param b table blues
function image.drawParticles(x, y, rotation, a, r, g, b) end

--- Draws a sprite
---@param sprite Sprite the sprite
---@param index integer frame index
---@param x number X position
---@param y number Y position
---@param coeff integer scale coefficient
---@param flip? boolean flip (default: false)
---@param rotation? integer rotation (default: 0)
---@param alpha? integer opacity (default: 255)
function image.drawSprite(sprite, index, x, y, coeff, flip, rotation, alpha) end

--- Creates a sprite from a spritesheet
---@param id string spritesheet path
---@param width integer frame width
---@param height integer frame height
---@return Sprite
function image.Sprite(id, width, height) end

---@class Sprite
---@field width integer frame width
---@field height integer frame height
local Sprite = {}

--
-- song: audio management
--

---@class song
song = {}

--- Plays a sound
---@param path string audio file path
---@param loop? integer loop count (default: 0)
---@param channel? integer audio channel (default: 1)
---@param volume? integer volume 0-128 (default: 124)
function song.playSound(path, loop, channel, volume) end

--- Stops a sound
---@param path string audio file path
function song.stopSound(path) end

--- Pauses a sound
---@param path string audio file path
function song.pauseSound(path) end

--- Resumes a paused sound
---@param path string audio file path
function song.resumeSound(path) end

--- Loads all sounds from a folder
---@param path string folder path
function song.loadSongFolder(path) end

--- Frees loaded sounds
function song.freeSongFolder() end

--- Stops a channel
---@param channel integer channel number
function song.stopChannel(channel) end

--- Pauses a channel
---@param channel integer channel number
function song.pauseChannel(channel) end

--- Resumes a paused channel
---@param channel integer channel number
function song.resumeChannel(channel) end

--
-- game: physics, blocks, entities and camera
--

---@class game
game = {}

--- Creates a block
---@param x number X position
---@param y number Y position
---@param w number width
---@param h number height
---@param type integer block type
---@return Block
function game.Block(x, y, w, h, type) end

--- Creates an empty blocks container
---@return Blocks
function game.Blocks() end

--- Creates a topdown entity
---@param x number X position
---@param y number Y position
---@param w number width
---@param h number height
---@return EntityTopdown
function game.EntityTopdown(x, y, w, h) end

--- Creates a platformer entity
---@param x number X position
---@param y number Y position
---@param w number width
---@param h number height
---@param jumpPower? number jump force (default: -150)
---@param gravity? number gravity (default: 300)
---@return EntityPlatformer
function game.EntityPlatformer(x, y, w, h, jumpPower, gravity) end

--- Creates a camera
---@param x number X position
---@param y number Y position
---@param smooth number smoothing factor
---@param w integer view width
---@param h integer view height
---@return Camera
function game.Camera(x, y, smooth, w, h) end

--- Loads blocks from a map file
---@param path string file path
---@param step_x integer X step (pixels)
---@param step_y integer Y step (pixels)
---@param separator string separator character
---@param exclude integer type to exclude
---@return BlocksFromFile
function game.BlocksFromFile(path, step_x, step_y, separator, exclude) end

--- Computes collisions for a platformer game
---@param entity EntityPlatformer entity
---@param blocks Blocks|table collision blocks
---@param max_fall_speed? number max fall speed (default: 300)
---@param wall_correction? number wall correction (default: 150)
---@param ignore? table block types to ignore
---@return EntityPlatformer
function game.hitboxPlatformer(entity, blocks, max_fall_speed, wall_correction, ignore) end

--- Computes collisions for a top-down game
---@param entity EntityTopdown entity
---@param blocks Blocks|table collision blocks
---@param ignore? table block types to ignore
---@return EntityTopdown
function game.hitboxTopdown(entity, blocks, ignore) end

---@class Block
---@field x number X position
---@field y number Y position
---@field w number width
---@field h number height
---@field type integer block type
local Block = {}

---@class Blocks
local Blocks = {}

--- Adds a block
---@param block Block block to add
function Blocks:add(block) end

--- Returns the number of blocks
---@return integer
function Blocks:size() end

--- Gets a block by index
---@param index integer
---@return Block
function Blocks:get(index) end

--- Iterator over blocks
---@return function
function Blocks:pairs() end

---@class EntityTopdown
---@field x number X position
---@field y number Y position
---@field w number width
---@field h number height
local EntityTopdown = {}

---@class EntityPlatformer
---@field x number X position
---@field y number Y position
---@field w number width
---@field h number height
---@field gravity number gravity force
---@field jumpPower number jump force
---@field requestJump boolean jump request
---@field leftLock boolean blocked left
---@field rightLock boolean blocked right
---@field inSky boolean in the air
---@field speedY number vertical speed
local EntityPlatformer = {}

---@class Camera
---@field x number X position
---@field y number Y position
---@field smoothFactor number smoothing factor
---@field w integer width
---@field h integer height
local Camera = {}

--- Updates the camera towards a target
---@param target_x number target X
---@param target_y number target Y
---@param dt number delta time
function Camera:update(target_x, target_y, dt) end

---@class BlocksFromFile
local BlocksFromFile = {}

--- Returns the number of blocks
---@return integer
function BlocksFromFile:size() end
