window.setUniversSize(150, 150)

local t = 0

utils.setUpdateCallback(function()
    local dt = var.delta()
    t = t + dt

    image.cls(10, 10, 15)

    local cx, cy = 75, 75
    local orbite = 45

    for i = 0, 7 do
        local angle = t + i * (math.pi / 4)
        local rayon = 10 + 7 * math.sin(t + i)

        local x = cx + orbite * math.cos(angle)
        local y = cy + orbite * math.sin(angle)

        local r = math.floor(127 + 128 * math.sin(t + i))
        local g = math.floor(127 + 128 * math.sin(t + i + 2))
        local b = math.floor(127 + 128 * math.sin(t + i + 4))

        if i % 2 == 0 then
            image.drawCircleFilled(x, y, rayon, r, g, b, 100)
        else
            image.drawCircle(x, y, rayon, r, g, b, 200)
        end
    end

    local rayon_central = 15 + 7 * math.sin(t * 2)
    image.drawCircle(cx, cy, rayon_central, 255, 255, 255, 255)
end)
