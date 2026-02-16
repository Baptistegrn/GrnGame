-- Set log level: 0 for Debug/All
utils.setLogLvl(0)
local ANIM_IDLE      = { 9, 12 }
local ANIM_WALK      = { 25, 31 }
local ANIM_JUMP_UP   = { 33, 35 }
local ANIM_JUMP_DOWN = { 36, 37 }
-- We create the objects here so they are ready before the first frame
local camera         = game.Camera(160, 90, 2.0, 320, 180)
local player         = game.EntityPlatformer(50.0, 50.0, 24, 24, -200)
local blocks         = game.Blocks()
-- Map Construction
for i = 0, 16 do
    blocks:add(game.Block(i * 20.0, 148.0, 20, 20, 1))
end
blocks:add(game.Block(100.0, 100.0, 20, 20, 1))
blocks:add(game.Block(180.0, 70.0, 20, 20, 3)) -- Decorative block
-- we load sprite in update loop
local sprite    = nil
local init      = false
local frame     = 9
local animTimer = 0
local animSpeed = 0.15
local flip      = false
-- Helper function for animation
local function animate(rangeStart, rangeEnd, dt, loop)
    animTimer = animTimer + dt
    if animTimer >= animSpeed then
        animTimer = 0
        frame = frame + 1
        if frame > rangeEnd then
            frame = loop and rangeStart or rangeEnd
        end
    end
end
-- main loop
function update()
    local dt = var.delta()
    local moving = false
    if not init then
        image.loadFolder("assets/")
        sprite = image.Sprite("assets/sprite.png", 32, 32)
        init = true
    end
    -- Input Handling
    if input.keyPressed("q") then
        player.x = player.x - 100 * dt
        flip = true
        moving = true
    end
    if input.keyPressed("d") then
        player.x = player.x + 100 * dt
        flip = false
        moving = true
    end
    if input.keyJustPressed("space") then
        player.requestJump = true
    end

    -- Physics & Camera
    game.hitboxPlatformer(player, blocks, 300.0, 150.0, { 3 })
    camera:update(player.x, player.y, dt)

    -- Animation State Machine
    if player.inSky then
        local anim = player.speedY < 0 and ANIM_JUMP_UP or ANIM_JUMP_DOWN
        if frame < anim[1] or frame > anim[2] then frame = anim[1] end
        animate(anim[1], anim[2], dt, false)
    elseif moving then
        if frame < ANIM_WALK[1] or frame > ANIM_WALK[2] then frame = ANIM_WALK[1] end
        animate(ANIM_WALK[1], ANIM_WALK[2], dt, true)
    else
        if frame < ANIM_IDLE[1] or frame > ANIM_IDLE[2] then frame = ANIM_IDLE[1] end
        animate(ANIM_IDLE[1], ANIM_IDLE[2], dt, true)
    end
    -- Rendering
    utils.cls(170, 0, 170)
    -- Draw Player (Camera offset applied)
    image.drawSprite(sprite, frame, player.x - 4.0 - camera.x, player.y - 4.0 - camera.y, 1, flip, 0, 255)
    -- Draw Blocks (Camera offset applied)
    for block in blocks:pairs() do
        image.drawRectFilled(block.x - camera.x, block.y - camera.y, block.w, block.h, 100, 100, 100, 255)
        image.drawRect(block.x - camera.x, block.y - camera.y, block.w, block.h, 50, 50, 50, 255)
    end
end

utils.initialize(180, 320, 60.0, false, "Mega Man Platformer")
