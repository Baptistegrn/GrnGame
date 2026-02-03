local ffi = require("ffi")
-- engine
local Engine = require("engine")


local function update()
    --update logic here
end


Engine.initialize(
    200,       --width
    200,       --height
    60.0,      --fps
    0,         --blacks bars
    "my game", --name
    update     --update function
)
