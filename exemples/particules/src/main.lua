window.setBlackBars(true)
window.setUniversSize(320, 180)
utils.setLogLvl(3)

local particleCount = 1000

local particleSystem = image.Particles()

local vx = {}
local vy = {}

for i = 1, particleCount do
    local p = image.Particle(
        math.random() * 320, -- X
        math.random() * 180, -- Y
        255, 255, 255,       -- rgb
        math.random() * 360, -- Rotation
        255                  -- Alpha
    )
    particleSystem:add(p)
    vx[i] = (math.random() - 0.5) * 200
    vy[i] = (math.random() - 0.5) * 200
end

function update()
    local dt = var.delta()
    -- Clear screen with a background color
    image.cls(30, 200, 0)
    local index = 1
    for p in particleSystem:pairs() do
        p.x = p.x + vx[index] * dt
        p.y = p.y + vy[index] * dt
        -- Constant rotation (90 degrees per second)
        p.rotation = (p.rotation + 90 * dt) % 360
        -- Simple bounce logic on screen boundaries
        if p.x < 0 or p.x > 320 then
            vx[index] = -vx[index]
        end
        if p.y < 0 or p.y > 180 then
            vy[index] = -vy[index]
        end

        index = index + 1
    end
    image.drawParticles(particleSystem)
end

utils.setUpdateCallback(update)
