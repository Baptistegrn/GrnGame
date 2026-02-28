window.setUniversSize(150, 150)

local t = 0

utils.setUpdateCallback(function()
    local dt = var.delta()
    t = t + dt
    image.cls(10, 10, 15)

    local cx, cy = 75, 75
    for i = 0, 7 do
        local size = 20 + i * 10 + 8 * math.sin(t * 2 + i * 0.5)
        local x = cx - size / 2
        local y = cy - size / 2
        local r = math.floor(127 + 128 * math.sin(t + i * 0.7))
        local g = math.floor(127 + 128 * math.sin(t + i * 0.7 + 2))
        local b = math.floor(127 + 128 * math.sin(t + i * 0.7 + 4))

        if i % 2 == 0 then
            image.drawRectFilled(x, y, size, size, r, g, b, 200)
        else
            image.drawRect(x, y, size, size, r, g, b, 150)
        end
    end
    image.drawRect(cx - 5, cy - 5, 10, 10, 255, 255, 255, 90)
end)
