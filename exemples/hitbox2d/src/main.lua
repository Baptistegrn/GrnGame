-- logs info
utils.setLogLvl(1)

-- sprite.png animations frames
local ANIM_IDLE = { 9, 12 }
local ANIM_WALK = { 25, 31 }
local ANIM_JUMP_UP = { 33, 35 }
local ANIM_JUMP_DOWN = { 36, 37 }

-- camera
local camera = nil

-- first init of the game
local init = false

-- player entity
local player = nil

-- blocks map
local blocks = nil

-- sprite to draw
local sprite = nil

local frame = 9
local animTimer = 0
local animSpeed = 0.15
local flip = false
local dt = 0

-- local function for animation
local function animate(rangeStart, rangeEnd, loop)
    animTimer = animTimer + dt
    if animTimer >= animSpeed then
        animTimer = 0
        frame = frame + 1
        if frame > rangeEnd then
            if loop then
                frame = rangeStart
            else
                frame = rangeEnd
            end
        end
    end
end

function update()
    -- delta time
    dt = var.delta()

    if not init then
        -- load all images in assets folder
        image.loadFolder("assets/")

        -- you need to give the full path
        sprite = image.Sprite("assets/sprite.png", 32, 32)

        -- create blocks container
        -- you can also do blocks = {} and add blocks manually
        blocks = game.Blocks()

        for i = 0, 10 do
            -- add a block to blocks
            -- to create a block you can do : block = game.Block(x, y, w, h, type)
            blocks:add(game.Block(i * 32.0, 148.0, 32, 32, 1))
        end

        blocks:add(game.Block(100.0, 100.0, 32, 32, 1))

        -- this block is decoration
        -- we set type = 3 and ignore it in hitbox update
        blocks:add(game.Block(180.0, 70.0, 32, 32, 3))

        -- create player : x, y, w, h, jump power (must be negative)
        player = game.EntityPlatformer(50.0, 50.0, 24, 24, -200)

        -- create camera : x, y, smooth factor, size : w, h
        camera = game.Camera(320 / 2, 180 / 2, 2.0, 320, 180)

        -- end of init
        init = true
    end

    local moving = false

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
        -- to activate jump you need to do this
        player.requestJump = true
    end

    -- 300.0 = max fall speed
    -- 150.0 = friction if you hit a wall during jump
    -- we ignore block type 3
    game.hitboxPlatformer(player, blocks, 300.0, 150.0, { 3 })

    -- camera update
    camera:update(player.x, player.y, dt)

    -- inSky : is the player in the air ?
    -- speedY < 0 = going up
    -- speedY > 0 = going down
    if player.inSky then
        if player.speedY < 0 then
            if frame < ANIM_JUMP_UP[1] or frame > ANIM_JUMP_UP[2] then
                frame = ANIM_JUMP_UP[1]
            end
            animate(ANIM_JUMP_UP[1], ANIM_JUMP_UP[2], false)
        else
            if frame < ANIM_JUMP_DOWN[1] or frame > ANIM_JUMP_DOWN[2] then
                frame = ANIM_JUMP_DOWN[1]
            end
            animate(ANIM_JUMP_DOWN[1], ANIM_JUMP_DOWN[2], false)
        end
    elseif moving then
        if frame < ANIM_WALK[1] or frame > ANIM_WALK[2] then
            frame = ANIM_WALK[1]
        end
        animate(ANIM_WALK[1], ANIM_WALK[2], true)
    else
        if frame < ANIM_IDLE[1] or frame > ANIM_IDLE[2] then
            frame = ANIM_IDLE[1]
        end
        animate(ANIM_IDLE[1], ANIM_IDLE[2], true)
    end

    -- draw screen background
    utils.cls(170, 0, 170)

    -- draw player
    -- you need to subtract camera.x and camera.y to apply camera
    image.drawSprite(
        sprite,
        frame,
        player.x - 4.0 - camera.x,
        player.y - 4.0 - camera.y,
        1,
        flip,
        0,
        255
    )

    -- draw blocks
    for block in blocks:pairs() do
        image.drawRectFilled(
            block.x - camera.x,
            block.y - camera.y,
            block.w,
            block.h,
            100, 100, 100, 255
        )
        image.drawRect(
            block.x - camera.x,
            block.y - camera.y,
            block.w,
            block.h,
            50, 50, 50, 255
        )
    end
end

-- initialize game
utils.initialize(180, 320, 60.0, false, "Mega man Platformer")
