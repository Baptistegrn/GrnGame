utils.setLogLvl(0)
local idle     = { 9, 12 }
local walk     = { 25, 31 }
local jumpUp   = { 33, 35 }
local jumpDown = { 36, 37 }
-- arg0,arg1 are the center of the screen, arg2 is the smooth factor and arg3,arg4 are the size of the screen view
game.createCamera(90, 90, 2.0, 180, 180)
--arg0,arg1 are the position of player,arg2,arg3 are the hitbox,arg4 are the powerjump of player
local player = game.EntityPlatformer(50.0, 50.0, 24, 24, -200)
-- create a list of blocks you can also do : {game.block(...),game.block(...)....}
local blocks = game.Blocks()
--add somes blocks
for i = -20, 20 do
    blocks:add(game.Block(i * 20.0, 148.0, 20, 20, 1))
end
blocks:add(game.Block(100.0, 100.0, 20, 20, 1)) -- Platform
blocks:add(game.Block(120.0, 100.0, 20, 20, 1)) -- Platform
blocks:add(game.Block(180.0, 70.0, 20, 20, 3))  -- Decorative block (Type 3)

local frame     = 9
local animTimer = 0.0
local animSpeed = 0.15
local flip      = false

-- Load Assets
image.loadFolder("assets/")
local sprite = image.Sprite("assets/sprite.png", 32, 32)

-- Set Universe Size
window.setUniversSize(180, 180)

-- Main Game Loop
utils.setUpdateCallback(function()
    local dt = var.delta()
    local moving = false
    --leftLock is for prevent bugs
    if input.keyPressed("q") and not player.leftLock then
        -- use int value for moving the player (better camera) you can also using delta
        player.x = player.x - 2
        flip = true
        moving = true
    end
    --rightLock is for prevent bugs
    if input.keyPressed("d") and not player.rightLock then
        -- use int value for moving the player (better camera) you can also using delta
        player.x = player.x + 2
        flip = false
        moving = true
    end
    if input.keyJustPressed("space") then
        -- send 1 requestJump
        player.requestJump = true
    end
    -- block type 3 is ignored
    game.hitboxPlatformer(player, blocks, 300.0, 150.0, { 3 })
    -- updateCamera is important
    game.updateCamera(player.x, player.y, dt)

    local currentAnim = idle
    local loopAnim = true
    -- player in sky ?
    if player.inSky then
        -- speedY <0 player is going up , speed >0 player is going down
        currentAnim = player.speedY < 0 and jumpUp or jumpDown
        loopAnim = false
    elseif moving then
        currentAnim = walk
    else
        currentAnim = idle
    end
    if frame < currentAnim[1] or frame > currentAnim[2] then
        frame = currentAnim[1]
    end
    frame, animTimer = game.animate(
        frame,
        animTimer,
        currentAnim[1], -- Start
        currentAnim[2], -- End
        animSpeed,
        dt,
        loopAnim
    )
    --draw
    image.cls(170, 0, 170)
    image.drawSprite(sprite, frame, player.x - 4.0, player.y - 4.0, 1, flip, 0, 255)
    for block in blocks:pairs() do
        image.drawRectFilled(block.x, block.y, block.w, block.h, 100, 100, 100, 255)
        image.drawRect(block.x, block.y, block.w, block.h, 50, 50, 50, 255)
    end
end)
