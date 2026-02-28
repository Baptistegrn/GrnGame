package.path = package.path .. ";./src/?.lua"
local test = require("test")
utils.setLogLvl(3)

utils.setUpdateCallback(function()
    if input.keyJustPressed("f3") then
        window.windowed(500, 500)
    end
    test.draw()
end)
