local ffi = require("ffi")
local Engine = require("engine")


local function update()
    --update logic here
end


Engine.initialize(
    200,
    200,
    60.0,
    0,
    "my game",
    update
)
