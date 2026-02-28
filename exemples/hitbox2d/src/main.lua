utils.setLogLvl(3)
local idle     = { 9, 12 }
local walk     = { 25, 31 }
local jumpUp   = { 33, 35 }
local jumpDown = { 36, 37 }

camera.createCamera(90, 90, 2.0, 180, 180, -320, 320, -180, 180)

-- -------------------------------------------------------
-- Player
-- -------------------------------------------------------
local player = entite.EntityPlatformer(50.0 + 5.0, 50.0, 24, 24, -250, 500, 3, 500, 150, 3, 10.0, 2, true)
local entities = entite.EntitiesPlatformer()
entities:add(player)

local player_hp        = 100
local player_max_hp    = 100
local player_score     = 0
local player_deaths    = 0
local invincible_timer = 0.0

-- -------------------------------------------------------
-- Bots
-- -------------------------------------------------------
local BOT_COUNT        = 100
local bot_dirs         = {}
local bot_timers       = {}
local bot_hp           = {}
local bot_alive        = {}
local bot_refs         = {} -- cache des references, alloue une seule fois

for i = 1, BOT_COUNT do
    local bot = entite.EntityPlatformer(
        math.random(-200, 400), 50.0, 16, 16,
        -250, 500, 2, 500, 150, 3, 8.0, 1, false
    )
    entities:add(bot)
    bot_dirs[i]   = math.random(0, 1) == 0 and "left" or "right"
    bot_timers[i] = math.random(60, 180)
    bot_hp[i]     = 3
    bot_alive[i]  = true
end

-- cache des references apres que toutes les entites sont ajoutees
-- entities[i] ne realloue pas si on le stocke, on evite les appels repetes
local player_ref = entities[0]
for i = 1, BOT_COUNT do
    bot_refs[i] = entities[i]
end

-- -------------------------------------------------------
-- Blocks
-- -------------------------------------------------------
local blocks = block.Blocks()
for i = -30, 30 do blocks:add(block.Block(i * 20.0, 148.0, 20, 20, 1)) end
blocks:add(block.Block(100.0, 100.0, 20, 20, 1))
blocks:add(block.Block(120.0, 100.0, 20, 20, 1))
blocks:add(block.Block(180.0, 70.0, 20, 20, 3))
for i = -2, 10 do blocks:add(block.Block(i * 20.0, 148.0, 20, 20, 1)) end
for i = 0, 20 do blocks:add(block.Block(140.0, 88.0 - i * 20.0, 20, 20, 1)) end
for i = 0, 20 do blocks:add(block.Block(40.0, 88.0 - i * 20.0, 20, 20, 1)) end

-- -------------------------------------------------------
-- Particles
-- -------------------------------------------------------
local PARTICLE_COUNT = 1000
local parts          = image.Particles()
local part_vx        = {}
local part_vy        = {}
local part_life      = {}
-- pre-fill particle pool
for i = 1, PARTICLE_COUNT do
    parts:add(image.Particle(0, 0, 255, 255, 255, 0, 0))
    part_vx[i]   = 0
    part_vy[i]   = 0
    part_life[i] = 0
end
-- cache des references particules
local part_refs = {}
for i = 1, PARTICLE_COUNT do
    part_refs[i] = parts:get(i - 1)
end


local function spawn_particle(x, y, r, g, b)
    for i = 1, PARTICLE_COUNT do
        if part_life[i] <= 0 then
            local p      = part_refs[i]
            p.x          = x + math.random(-6, 6)
            p.y          = y + math.random(-4, 4)
            p.r          = r
            p.g          = g
            p.b          = b
            p.a          = 255
            part_vx[i]   = math.random(-80, 80) / 10.0
            part_vy[i]   = math.random(-120, -20) / 10.0
            part_life[i] = 0.6 + math.random(0, 40) / 100.0
            return
        end
    end
end

-- -------------------------------------------------------
-- Assets
-- -------------------------------------------------------
image.loadFolder("assets/")
local sprite = image.Sprite("assets/sprite.png", 32, 32)
window.setUniversSize(180, 180)

-- -------------------------------------------------------
-- Player state
-- -------------------------------------------------------
local frame           = 9
local animTimer       = 0.0
local animSpeed       = 0.15
local flip            = false
local ac              = false

local attack_timer    = 0.0
local ATTACK_COOLDOWN = 0.4
local is_attacking    = false
local attack_range    = 30.0

local COIN_COUNT      = 20
local coins           = {}
for i = 1, COIN_COUNT do
    coins[i] = {
        x     = math.random(-200, 400),
        y     = math.random(60, 130),
        alive = true,
        timer = 0.0
    }
end

-- -------------------------------------------------------
-- Helpers
-- -------------------------------------------------------
local function dist(ax, ay, bx, by)
    local dx = ax - bx
    local dy = ay - by
    return math.sqrt(dx * dx + dy * dy)
end

local function respawn_player()
    player_ref.x      = 50.0
    player_ref.y      = 50.0
    player_ref.speedX = 0
    player_ref.speedY = 0
    player_hp         = player_max_hp
    player_deaths     = player_deaths + 1
    invincible_timer  = 2.0
    for i = 1, 12 do spawn_particle(50, 50, 255, 100, 100) end
end

-- -------------------------------------------------------
-- Main loop
-- -------------------------------------------------------
utils.setUpdateCallback(function()
    local dt     = var.delta()
    local moving = false

    -- ---- input ----
    if input.keyPressed("q") then
        player_ref.requestLeft = true
        flip                   = true
        moving                 = true
    end
    if input.keyPressed("d") then
        player_ref.requestRight = true
        flip                    = false
        moving                  = true
    end
    if input.keyJustPressed("space") then
        camera.shakeCamera(0.6, 0.08, 0.15)
        player_ref.requestJump = true
        ac = false
    end
    if input.keyJustPressed("f") and attack_timer <= 0 then
        is_attacking = true
        attack_timer = ATTACK_COOLDOWN
        camera.shakeCamera(0.8, 0.05, 0.2)
    end

    -- ---- timers ----
    if attack_timer > 0 then attack_timer = attack_timer - dt end
    if invincible_timer > 0 then invincible_timer = invincible_timer - dt end

    -- ---- bot AI (avant hitbox) ----
    for i = 1, BOT_COUNT do
        if bot_alive[i] then
            local bot = bot_refs[i]
            bot_timers[i] = bot_timers[i] - 1
            if bot_timers[i] <= 0 then
                local r       = math.random(0, 2)
                bot_dirs[i]   = r == 0 and "left" or r == 1 and "right" or "none"
                bot_timers[i] = math.random(40, 200)
            end
            if bot_dirs[i] == "left" then bot.requestLeft = true end
            if bot_dirs[i] == "right" then bot.requestRight = true end
            if not bot.inSky and math.random(0, 120) == 0 then bot.requestJump = true end
            if bot.leftLock then
                bot_dirs[i] = "right"; bot_timers[i] = math.random(40, 120)
            end
            if bot.rightLock then
                bot_dirs[i] = "left"; bot_timers[i] = math.random(40, 120)
            end
        end
    end

    -- ---- physics ----
    entite.hitboxPlatformer(entities, blocks, 8, dt)

    -- ---- positions a jour apres hitbox ----
    local px = player_ref.x
    local py = player_ref.y

    -- ---- landing shake ----
    if player_ref.inSky then ac = true end
    if not player_ref.inSky and ac then
        camera.shakeCamera(1.0, 0.3, 0.2)
        for i = 1, 8 do spawn_particle(px, py + 20, 180, 180, 180) end
        ac = false
    end

    -- ---- attack particles ----
    if is_attacking then
        for i = 1, 6 do spawn_particle(px + (flip and -12 or 12), py, 255, 200, 50) end
    end

    -- ---- coin collection ----
    for i = 1, COIN_COUNT do
        local c = coins[i]
        if c.alive then
            c.timer = c.timer + dt
            if dist(px, py, c.x, c.y) < 16 then
                c.alive      = false
                player_score = player_score + 10
                for j = 1, 8 do spawn_particle(c.x, c.y, 255, 220, 0) end
            end
        end
    end

    -- ---- bot interactions (apres hitbox) ----
    for i = 1, BOT_COUNT do
        if bot_alive[i] then
            local bot = bot_refs[i]
            local d   = dist(bot.x, bot.y, px, py)

            if d < 60 then
                bot.requestLeft  = bot.x > px
                bot.requestRight = bot.x < px
                if not bot.inSky and math.random(0, 60) == 0 then bot.requestJump = true end
            end

            if invincible_timer <= 0 and d < 20 then
                player_hp        = player_hp - 1
                invincible_timer = 0.8
                camera.shakeCamera(2.0, 0.2, 0.3)
                for j = 1, 6 do spawn_particle(px, py, 255, 50, 50) end
                if player_hp <= 0 then respawn_player() end
            end

            if is_attacking and dist(bot.x, bot.y, px + (flip and -attack_range or attack_range), py) < 24 then
                bot_hp[i] = bot_hp[i] - 1
                for j = 1, 5 do spawn_particle(bot.x, bot.y, 255, 100, 50) end
                if bot_hp[i] <= 0 then
                    bot_alive[i] = false
                    player_score = player_score + 5
                    for j = 1, 12 do spawn_particle(bot.x, bot.y, 220, 60, 60) end
                end
            end
        end
    end
    is_attacking = false

    -- ---- particles update (via cache, pas de get() chaque frame) ----
    local GRAVITY_P = 200.0
    for i = 1, PARTICLE_COUNT do
        if part_life[i] > 0 then
            part_life[i] = part_life[i] - dt
            local p = part_refs[i]
            part_vy[i] = part_vy[i] + GRAVITY_P * dt
            p.x = p.x + part_vx[i] * dt * 60
            p.y = p.y + part_vy[i] * dt * 60
            p.a = math.max(0, math.floor((part_life[i] / 0.6) * 255))
            if part_life[i] <= 0 then p.a = 0 end
        end
    end

    -- ---- draw ----
    image.cls(30, 10, 50)
    camera.updateCamera(px, py, dt)

    for b in blocks:pairs() do
        image.drawRectFilled(b.x, b.y, b.w, b.h, 100, 100, 100, 255)
        image.drawRect(b.x, b.y, b.w, b.h, 60, 60, 60, 255)
    end

    for i = 1, COIN_COUNT do
        local c = coins[i]
        if c.alive then
            local bob = math.sin(c.timer * 4.0) * 2.0
            image.drawCircleFilled(c.x + 4, c.y + bob, 5, 255, 215, 0, 255)
            image.drawCircle(c.x + 4, c.y + bob, 5, 200, 160, 0, 255)
        end
    end

    for i = 1, BOT_COUNT do
        if bot_alive[i] then
            local bot = bot_refs[i]
            local r   = math.floor(80 + (bot_hp[i] / 3.0) * 140)
            image.drawRectFilled(bot.x, bot.y, bot.w, bot.h, r, 60, 60, 255)
            image.drawRect(bot.x, bot.y, bot.w, bot.h, 180, 40, 40, 255)
            local bar_w = math.floor((bot_hp[i] / 3.0) * bot.w)
            image.drawRectFilled(bot.x, bot.y - 4, bot.w, 2, 60, 60, 60, 200)
            image.drawRectFilled(bot.x, bot.y - 4, bar_w, 2, 80, 220, 80, 220)
        end
    end


    local draw_player = invincible_timer <= 0 or math.floor(invincible_timer * 10) % 2 == 0
    if draw_player then
        local currentAnim = idle
        local loopAnim    = true
        if player_ref.inSky then
            currentAnim = player_ref.speedY < 0 and jumpUp or jumpDown
            loopAnim    = false
        elseif moving then
            currentAnim = walk
        end
        if frame < currentAnim[1] or frame > currentAnim[2] then frame = currentAnim[1] end
        frame, animTimer = entite.animate(frame, animTimer,
            currentAnim[1], currentAnim[2], animSpeed, dt, loopAnim)
        image.drawSprite(sprite, frame, px - 4.0, py - 4.0, 1, flip, 0, 255)
    end

    if attack_timer > ATTACK_COOLDOWN - 0.08 then
        local ax = flip and (px - attack_range) or (px + attack_range)
        image.drawCircleFilled(ax, py + 8, 8, 255, 220, 50, 180)
    end

    local hud_x = camera.getCameraX() + 4
    local hud_y = camera.getCameraY() + 4
    image.drawRectFilled(hud_x, hud_y, 50, 5, 60, 20, 20, 200)
    local hp_w = math.floor((player_hp / player_max_hp) * 50)
    image.drawRectFilled(hud_x, hud_y, hp_w, 5, 220, 60, 60, 220)
    image.drawRect(hud_x, hud_y, 50, 5, 255, 255, 255, 120)
    image.drawRectFilled(hud_x, hud_y + 8, 60, 5, 20, 20, 60, 180)
    image.drawRectFilled(hud_x, hud_y + 8, math.min(60, player_score), 5, 80, 180, 255, 200)
    image.drawRect(hud_x, hud_y + 8, 60, 5, 200, 200, 255, 120)
    image.drawParticles(parts)
end)
