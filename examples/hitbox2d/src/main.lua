-- modules
local physics = require("physic")
local animation = require("animation")

-- load and create sprite
image.loadFolder("assets/")
local sprite = image.Sprite("assets/sprite.png", 32, 32)
window.setFps(64)
-- window and camera
window.setUniversSize(320, 180)
-- x,y,smooth factor,sizex,sizey,limitleft,limitright,limitup,limitdown
camera.createCamera(90, 90, 2.5, 180, 180, -500, 500, -500, 500)


local player = {
    x = 50,                    --x
    y = 50,                    --y
    w = 24,                    --w
    h = 24,                    --h
    gravity = 500,             --gravity power
    jumpPower = -250,          --jump power (negative)
    speedMaxX = 3,             --speed x
    initialSpeed = 0.1,        --initial speed when player start to move
    acceleration = 3,          -- factor acceleration
    jumpOnWall = true,         -- can jump on wall ?
    numberOfJumpsPossible = 2, -- number of jumps
}
-- need to create a table
local entities = { player }
--
local map_blocks = {}
-- structure : {x,y,w,h,type}
for i = 1, 5 do table.insert(map_blocks, { x = i * 20, y = 150, w = 20, h = 20, type = 1 }) end
table.insert(map_blocks, { x = 100, y = 100, w = 40, h = 20, type = 1 })
table.insert(map_blocks, { x = 180, y = 80, w = 20, h = 20, type = 3 }) -- we going to ingore type 3

-- animation
local anims = { idle = { 9, 12 }, walk = { 25, 31 }, jumpUp = { 33, 35 }, jumpDown = { 36, 37 } }
local frame, animTimer, flip = 9, 0, false

utils.setUpdateCallback(function()
    local dt = var.delta()

    if input.keyPressed("q") then
        player.requestLeft = true; flip = true
    end
    if input.keyPressed("d") then
        player.requestRight = true; flip = false
    end
    if input.keyJustPressed("space") then player.requestJump = true end

    -- entites , map , ignoreMask : its 0<<3 = 8 so we ignore block type 3 , delta
    physics.update(entities, map_blocks, 8, dt)
    camera.updateCamera(player.x, player.y, dt)

    image.cls(170, 0, 170)
    local state = anims.idle
    if player.inSky then
        state = player.speedY < 0 and anims.jumpUp or anims.jumpDown
    elseif (input.keyPressed("q") or input.keyPressed("d")) then
        state = anims.walk
    end
    image.drawSprite(sprite, frame, player.x - 4, player.y - 4, 1, flip, 0, 255)
    for _, b in ipairs(map_blocks) do
        image.drawRectFilled(b.x, b.y, b.w, b.h, 100, 100, 100, 255)
        image.drawRect(b.x, b.y, b.w, b.h, 60, 60, 60, 255)
    end
    frame, animTimer = animation.update(frame, animTimer, state[1], state[2], 0.15, dt, not player.inSky)
end)
