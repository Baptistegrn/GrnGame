window.setUniversSize(150, 150)

local t = 0

utils.setUpdateCallback(function()
    local dt = var.delta()
    t = t + dt

    image.cls(10, 10, 15)

    local cx, cy = 75, 75
    local longueur = 35

    for i = 0, 11 do
        local angle = t + i * (math.pi / 6)

        local x1 = cx
        local y1 = cy
        local x2 = cx + longueur * math.cos(angle)
        local y2 = cy + longueur * math.sin(angle)

        local r = math.floor(127 + 128 * math.sin(t + i))
        local g = math.floor(127 + 128 * math.sin(t + i + 2))
        local b = math.floor(127 + 128 * math.sin(t + i + 4))

        image.drawLine(x1, y1, x2, y2, r, g, b, 255)
    end

    image.drawCircleFilled(cx, cy, 3, 255, 255, 255, 255)
end)
