local physics = require("physic")
local player = {x = 50,y = 50,w = 24,h = 24,gravity = 500,jumpPower = -250, -- negative = upwardspeedMaxX = 3,initialSpeed = 0.1,acceleration = 3,jumpOnWall = true,numberOfJumpsPossible = 2,}
local map_blocks = {}
table.insert(map_blocks, { x = 0, y = 150, w = 320, h = 20, type = 1 })
local entities = { player }
function update()
    local dt            = var.delta()
    player.requestLeft  = input.keyPressed("q")
    player.requestRight = input.keyPressed("d")
    player.requestJump  = input.keyJustPressed("space")
    physics.update(entities, map_blocks, 0, dt)
    camera.updateCamera(player.x, player.y, dt)
    image.drawRect(player.x, player.y, player.w, player.h, 0, 255, 255)
    local block = map_blocks[1]
    image.drawRectFilled(block.x, block.y, block.w, block.h, 40)
end
utils.setUpdateCallback(update)
