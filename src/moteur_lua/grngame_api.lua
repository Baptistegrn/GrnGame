---@meta
-- Definition file for GrnGame API autocompletion.
-- Generated based on C++ bindings registration.

--
-- utils: General engine utilities (logging, callbacks, shutdown)
--
---@class utils
utils = {}

--- Sets the update function called every frame (trampoline).
---@param update_func? function The function to call (optional).
function utils.setUpdateCallback(update_func) end

--- Logs a message to the engine console.
---@param level integer Level (0=debug, 1=info, 2=warning, 3=error)
---@param message string The message content
function utils.logMessage(level, message) end

--- Stops the game engine.
function utils.stopEngine() end

--- Sets the minimum log level to display.
---@param level integer Level (0=debug, 1=info, 2=warning, 3=error)
function utils.setLogLvl(level) end

--- Retrieves the text entered by the user (text input).
---@return string
function utils.getInputText() end

--- Deletes the user's text input (e.g., after sending a message).
function utils.deleteInputText() end

--- Sets the internal size of the universe.
---@param width integer Width
---@param height integer Height
function utils.setWindowSize(width, height) end

--
-- window: Display window management
--
---@class window
window = {}

--- Switches the window to exclusive fullscreen mode.
function window.fullscreen() end

--- Switches the window to borderless windowed fullscreen mode.
function window.windowedFullscreen() end

--- Switches to windowed mode with a specific size.
---@param width integer Width
---@param height integer Height
function window.windowed(width, height) end

--- Switches to windowed mode with a zoom coefficient.
---@param coeff integer Coefficient
function window.windowedCoefficient(coeff) end

--- Sets the target Frames Per Second (FPS).
---@param fps integer
function window.setFps(fps) end

--- Sets the window title.
---@param title string
function window.setTitle(title) end

--- Enables or disables black bars to maintain aspect ratio.
---@param enabled boolean
function window.setBlackBars(enabled) end

--- Sets the universe size (alias for utils.setWindowSize).
---@param width integer
---@param height integer
function window.setUniversSize(width, height) end

--
-- mouse: Mouse management
--
---@class mouse
mouse = {}

--- Returns the mouse X position in the universe.
---@return number
function mouse.x() end

--- Returns the mouse Y position in the universe.
---@return number
function mouse.y() end

--- Checks if a button was just pressed in this frame.
---@param button integer|string
---@return boolean
function mouse.justPressed(button) end

--- Checks if a button is currently held down.
---@param button integer|string
---@return boolean
function mouse.pressed(button) end

--- Vertical scroll value.
---@return integer
function mouse.scrollVertical() end

--- Horizontal scroll value.
---@return integer
function mouse.scrollHorizontal() end

--
-- input: Keyboard and Controller inputs
--
---@class input
input = {}

--- Checks if a keyboard key was just pressed.
---@param key string Key name
---@return boolean
function input.keyJustPressed(key) end

--- Checks if a keyboard key is currently held down.
---@param key string Key name
---@return boolean
function input.keyPressed(key) end

--- Shows or hides the system cursor.
---@param visible boolean
function input.showCursor(visible) end

--- Checks if a controller button was just pressed.
---@param button string
---@param index integer Controller index
---@return boolean
function input.buttonJustPressed(button, index) end

--- Checks if a controller button is held down.
---@param button string
---@param index integer Controller index
---@return boolean
function input.buttonPressed(button, index) end

--- Initializes a controller.
---@param index integer
function input.initController(index) end

--- Gets Joysticks and triggers state {lx, ly, rx, ry, tl, tr}.
---@param dead_zone number
---@param index integer
---@return table
function input.getJoysticks(dead_zone, index) end

--- Closes a specific controller.
---@param index integer
function input.closeController(index) end

--- Closes a specific joystick.
---@param index integer
function input.closeJoystick(index) end

--- Closes the controller connection completely.
---@param index integer
function input.closeTheController(index) end

--
-- var: Engine constants and states (Read Only)
--
---@class var
var = {}

--- Delta Time in seconds.
---@return number
function var.delta() end

--- Current FPS.
---@return number
function var.fps() end

--- Total frame count since start.
---@return integer
function var.frameCount() end

--- Rendering X offset (due to black bars/camera).
---@return number
function var.offsetX() end

--- Rendering Y offset (due to black bars/camera).
---@return number
function var.offsetY() end

--- Current window width.
---@return integer
function var.currentWidth() end

--- Current window height.
---@return integer
function var.currentHeight() end

--- Universe width.
---@return integer
function var.universeWidth() end

--- Universe height.
---@return integer
function var.universeHeight() end

--- Is the window minimized?
---@return boolean
function var.windowMinimised() end

--- Path of a dropped file (if any).
---@return string
function var.getTextDrop() end

--
-- json: Encrypted JSON file management
--
---@class json
json = {}

--- Loads a JSON file.
---@param filename string
function json.load(filename) end

--- Saves a JSON file.
---@param filename string
function json.save(filename) end

--- Deletes a JSON file.
---@param filename string
function json.delete(filename) end

--- Writes a variable to a JSON file, automatically detecting its type.
--- Supports writing via a specific path or writing a full data object directly.
---
--- @overload fun(filename: string, data: boolean|string|number|table|nil)
--- @param filename string The name of the JSON file.
--- @param varName string|boolean|number|table|nil The path of the variable (e.g., "player.health") OR the data itself.
--- @param value? any The value to write (optional if varName is the data).
function json.writeVariable(filename, varName, value) end

--- Reads a variable from a JSON file, automatically detecting its type.
--- Returns a specific value (string, number, boolean) or a nested table.
--- Use an empty string "" for `varName` to read the entire root object.
---
--- @param filename string The name of the JSON file.
--- @param varName string The path of the variable (e.g., "player.health") or "" for the root.
--- @return boolean|string|number|table|nil value The requested value, or nil if not found.
function json.readVariable(filename, varName) end

--- Deletes a variable from a loaded JSON structure.
---@param filename string
---@param varName string
function json.deleteVariable(filename, varName) end

--- Sets the encryption Key (index specific).
---@param index integer
---@param value integer
function json.setKey(index, value) end

--- Sets the encryption IV (index specific).
---@param index integer
---@param value integer
function json.setIv(index, value) end

--- Checks if a file exists.
---@param filename string
---@return boolean
function json.exists(filename) end

--
-- image: Graphics and Sprites
--
---@class image
image = {}

--- Creates a Sprite object.
---@param id string Texture ID/Path
---@param width integer
---@param height integer
---@return Sprite
function image.Sprite(id, width, height) end

--- Draws a rectangle outline.
---@param x number
---@param y number
---@param w integer
---@param h integer
---@param r? integer Red (0-255)
---@param g? integer Green (0-255)
---@param b? integer Blue (0-255)
---@param a? integer Alpha (0-255)
function image.drawRect(x, y, w, h, r, g, b, a) end

--- Draws a filled rectangle.
---@param x number
---@param y number
---@param w integer
---@param h integer
---@param r? integer Red (0-255)
---@param g? integer Green (0-255)
---@param b? integer Blue (0-255)
---@param a? integer Alpha (0-255)
function image.drawRectFilled(x, y, w, h, r, g, b, a) end

--- Draws an image/texture.
---@param path string
---@param x number
---@param y number
---@param coeff integer Scale coefficient
---@param flip? boolean Flip horizontally
---@param rotationP? integer Pivot point for rotation
---@param rotation? integer Rotation in degrees
---@param alpha? integer Alpha transparency (0-255)
function image.draw(path, x, y, coeff, flip, rotationP, rotation, alpha) end

--- Draws text using a font.
---@param font_path string
---@param text string
---@param x number
---@param y number
---@param scale integer
---@param flip? boolean
---@param spacing? number Letter spacing
---@param rotationP? integer Pivot point
---@param rotation? integer Rotation in degrees
---@param alpha? integer Alpha (0-255)
function image.drawText(font_path, text, x, y, scale, flip, spacing, rotationP, rotation, alpha) end

--- Sets the window icon.
---@param path string
function image.setIcon(path) end

--- Draws a specific sprite from a spritesheet/texture.
---@param sprite Sprite
---@param index integer Sprite index
---@param x number
---@param y number
---@param coeff integer
---@param flip? boolean
---@param rotation? integer
---@param alpha? integer
function image.drawSprite(sprite, index, x, y, coeff, flip, rotation, alpha) end

--- Loads all images in a folder into memory.
---@param folder string
function image.loadFolder(folder) end

--- Draws particle effects.
---@param x table Table of X positions
---@param y table Table of Y positions
---@param rotation table Table of rotations
---@param a table Table of alpha values
---@param r table Table of red values
---@param g table Table of green values
---@param b table Table of blue values
function image.drawParticles(x, y, rotation, a, r, g, b) end

--- Frees images loaded from a folder.
function image.freeFolder() end

--- Draws a line.
---@param x1 number
---@param y1 number
---@param x2 number
---@param y2 number
---@param r? integer
---@param g? integer
---@param b? integer
---@param a? integer
function image.drawLine(x1, y1, x2, y2, r, g, b, a) end

--- Draws a circle outline.
---@param x number
---@param y number
---@param radius integer
---@param r? integer
---@param g? integer
---@param b? integer
---@param a? integer
function image.drawCircle(x, y, radius, r, g, b, a) end

--- Draws a filled circle.
---@param x number
---@param y number
---@param radius integer
---@param r? integer
---@param g? integer
---@param b? integer
---@param a? integer
function image.drawCircleFilled(x, y, radius, r, g, b, a) end

--- Clears the screen with a specific color.
---@param r integer
---@param g integer
---@param b integer
function image.cls(r, g, b) end

--- Sets encryption Key for image loading.
---@param index integer
---@param value integer
function image.setKey(index, value) end

--- Sets encryption IV for image loading.
---@param index integer
---@param value integer
function image.setIv(index, value) end

--
-- song: Audio management
--
---@class song
song = {}

--- Plays a sound file.
---@param path string
---@param loop? integer (1=loop, 0=once)
---@param channel? integer
---@param volume? integer (0-128)
function song.play(path, loop, channel, volume) end

--- Stops a sound file.
---@param path string
function song.stop(path) end

--- Pauses a sound file.
---@param path string
function song.pause(path) end

--- Resumes a sound file.
---@param path string
function song.resume(path) end

--- Loads audio files from a folder.
---@param path string
function song.loadFolder(path) end

--- Stops a specific audio channel.
---@param channel integer
function song.stopChannel(channel) end

--- Pauses a specific audio channel.
---@param channel integer
function song.pauseChannel(channel) end

--- Resumes a specific audio channel.
---@param channel integer
function song.resumeChannel(channel) end

--- Frees audio resources.
function song.freeFolder() end

--- Sets encryption Key for audio loading.
---@param index integer
---@param value integer
function song.setKey(index, value) end

--- Sets encryption IV for audio loading.
---@param index integer
---@param value integer
function song.setIv(index, value) end

--
-- game: Physics, Blocks, Entities, and Global Camera
--
---@class game
game = {}

--- Creates a Block object.
---@param x number
---@param y number
---@param w number
---@param h number
---@param type integer
---@return Block
function game.Block(x, y, w, h, type) end

--- Creates a Blocks container.
---@return Blocks
function game.Blocks() end

--- Creates a Platformer Entity.
---@param x number
---@param y number
---@param w number
---@param h number
---@param jumpPower? number (Optional)
---@param gravity? number (Optional)
---@param speedMaxX? number (Optional) Maximum horizontal speed
---@param speedMaxFall? number (Optional) Maximum falling speed
---@param wallCorrection? number (Optional) Wall friction/correction
---@param initialSpeed? number (Optional) initial speed of x
---@param acceleration? number (Optional) acceleration speed
---@return EntityPlatformer
function game.EntityPlatformer(x, y, w, h, jumpPower, gravity, speedMaxX, speedMaxFall, wallCorrection, initialSpeed,
                               acceleration)
end

--- Initializes the global camera.
---@param x number
---@param y number
---@param smooth number Smoothing factor
---@param w integer Width
---@param h integer Height
function game.createCamera(x, y, smooth, w, h) end

--- Updates the camera position towards a target.
---@param tx number Target X
---@param ty number Target Y
---@param dt number Delta time
function game.updateCamera(tx, ty, dt) end

--- Sets the Camera X position directly.
---@param v number
function game.setCameraX(v) end

--- Gets the Camera X position.
---@return number
function game.getCameraX() end

--- Sets the Camera Y position directly.
---@param v number
function game.setCameraY(v) end

--- Gets the Camera Y position.
---@return number
function game.getCameraY() end

--- Sets the Camera smoothing factor.
---@param v number
function game.setCameraSmooth(v) end

--- Gets the Camera smoothing factor.
---@return number
function game.getCameraSmooth() end

--- Loads a map from a text/data file.
---@param path string
---@param step_x integer Block width step
---@param step_y integer Block height step
---@param separator string Character separator
---@param exclude integer Value to exclude
---@return BlocksFromFile
function game.BlocksFromFile(path, step_x, step_y, separator, exclude) end

--- Manages platformer collisions (Entity vs Blocks).
---@param entity EntityPlatformer
---@param blocks Blocks|table
---@param ignore_mask? integer Bitmask of block types to ignore (Optional, defaults to 0)
---@param delta? number Delta time (Optional, defaults to engine dt)
function game.hitboxPlatformer(entity, blocks, ignore_mask, delta) end

--- Calculates the next frame and timer for an animation.
--- Useful for managing sprite sheets logic.
---
--- @param frame integer Current frame index
--- @param timer number Current timer accumulator
--- @param rangeStart integer First frame index of the animation
--- @param rangeEnd integer Last frame index of the animation
--- @param speed number Duration of a single frame in seconds
--- @param dt number Delta time (elapsed time since last frame)
--- @param loop boolean If true, animation restarts from rangeStart; otherwise it stays at rangeEnd
--- @return integer newFrame The updated frame index
--- @return number newTimer The updated timer value
function game.animate(frame, timer, rangeStart, rangeEnd, speed, dt, loop) end

-- Internal Classes

---@class Sprite
---@field width integer
---@field height integer
local Sprite = {}

---@class Block
---@field x number
---@field y number
---@field w number
---@field h number
---@field type integer
local Block = {}

---@class Blocks
local Blocks = {}
--- Adds a block to the container.
---@param block Block
function Blocks:add(block) end

--- Returns the size of the container.
---@return integer
function Blocks:size() end

--- Gets a block at a specific index.
---@param index integer
---@return Block
function Blocks:get(index) end

--- Returns an iterator for the blocks.
function Blocks:pairs() end

---@class EntityPlatformer
---@field x number
---@field y number
---@field w number
---@field h number
---@field speedX number
---@field speedY number
---@field inSky boolean
---@field gravity number
---@field jumpPower number
---@field requestJump boolean
---@field requestLeft boolean
---@field requestRight boolean
---@field leftLock boolean
---@field rightLock boolean
---@field initialSpeed number
---@field acceleration number
local EntityPlatformer = {}

---@class BlocksFromFile
local BlocksFromFile = {}

--- Returns the number of blocks loaded.
---@return integer
function BlocksFromFile:size() end
