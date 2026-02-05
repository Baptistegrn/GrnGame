-- path
package.path = package.path .. ";../src/?.lua"
-- luajit ffi
local ffi = require("ffi")
-- engine
local Engine = require("engine")

-- game class
local Game = {}
Game.__index = Game

function Game:new()
    local self = setmetatable({}, Game)
    return self
end

function Game:update()
    -- update logic here
end

function Game:run()
    Engine.initialize(
        200,       -- width
        200,       -- height
        60.0,      -- fps
        0,         -- black bars
        "my game", -- name
        function() self:update() end
    )
end

-- entry point
local game = Game:new()
game:run()
