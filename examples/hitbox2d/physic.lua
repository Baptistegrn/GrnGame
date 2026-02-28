local physics = {}
-- Constants for Coyote Time and Wall Jump mechanics
local WALL_TOLERANCE = 0.15
local GROUND_TOLERANCE = 0.10
local WALL_JUMP_LOCK_DURATION = 0.20

--- Calculates the hitbox and physics for a list of entities and blocks
-- @param entities Table containing game entities
-- @param blocks Table containing blocks {x, y, w, h, type}
-- @param ignoreMask Bitmask to filter block types
-- @param dt Frame delta time
function physics.update(entities, blocks, ignoreMask, dt)
    for _, entity in ipairs(entities) do
        local x, y = entity.x, entity.y
        local vy = entity.speedY or 0
        local vx = entity.speedX or 0
        local w, h = entity.w or 32, entity.h or 32

        local gravity = entity.gravity or 300
        local jumpPower = entity.jumpPower or -150
        local speedMaxX = entity.speedMaxX or 2
        local speedMaxFall = entity.speedMaxFall or 300
        local wallCorrection = entity.wallCorrection or 150
        local acceleration = entity.acceleration or 8
        local initialSpeed = entity.initialSpeed or 0.4
        local maxJumpsPossible = entity.numberOfJumpsPossible or 1

        -- State and Timers
        local inAir = entity.inSky
        local wallTimerLeft = (entity.wallTimerLeft or 0) - dt
        local wallTimerRight = (entity.wallTimerRight or 0) - dt
        local groundTimer = entity.groundTimer or 0
        local lockTimer = entity.wallJumpLockTimer or 0

        -- Coyote Time Logic
        if inAir then
            groundTimer = groundTimer - dt
        else
            groundTimer = GROUND_TOLERANCE
        end

        -- Directional Input Lock Logic (Wall Jump propulsion)
        local reqLeft = entity.requestLeft
        local reqRight = entity.requestRight

        if lockTimer > 0 then
            lockTimer = math.max(0, lockTimer - dt)
            reqRight, reqLeft = true, false -- Force right movement
        elseif lockTimer < 0 then
            lockTimer = math.min(0, lockTimer + dt)
            reqLeft, reqRight = true, false -- Force left movement
        end

        -- Horizontal Movement logic
        if reqLeft and not entity.leftLock then
            if vx <= 0 then vx = speedMaxX * initialSpeed end
            vx = math.min(speedMaxX, vx + (speedMaxX * acceleration) * dt)
            x = x - vx
        elseif reqRight and not entity.rightLock then
            if vx <= 0 then vx = speedMaxX * initialSpeed end
            vx = math.min(speedMaxX, vx + (speedMaxX * acceleration) * dt)
            x = x + vx
        else
            -- Friction/Deceleration
            vx = math.max(0, vx - (speedMaxX * acceleration * 1.5) * dt)
            x = math.floor(x + 0.5)
            y = math.floor(y + 0.5)
        end

        -- Jump Logic (Wall and Ground)
        if entity.requestJump then
            -- Wall Jumping
            if entity.jumpOnWall and inAir and
                ((wallTimerLeft > 0 and reqRight) or (wallTimerRight > 0 and reqLeft)) then
                vy = jumpPower
                if wallTimerLeft > 0 then
                    x = x + 4      -- Push away from wall
                    vx = speedMaxX -- Restore momentum
                    wallTimerLeft = 0
                    lockTimer = WALL_JUMP_LOCK_DURATION
                else
                    x = x - 4
                    vx = speedMaxX
                    wallTimerRight = 0
                    lockTimer = -WALL_JUMP_LOCK_DURATION
                end
                -- Ground Jumping or Coyote Time
            elseif not inAir or groundTimer > 0 then
                vy = jumpPower
                inAir = true
                groundTimer = 0
                entity.numberOfJumps = math.max(0, (entity.numberOfJumps or 1) - 1)
                -- Air/Double Jumping
            elseif (entity.numberOfJumps or 0) > 0 then
                vy = jumpPower
                entity.numberOfJumps = entity.numberOfJumps - 1
            end
        end

        -- Collision Detection
        local groundY = 1e9
        local ceilingY = -1e9
        local x2, y2 = x + w, y + h
        local newX = x
        local lockLeft, lockRight, wallContact = false, false, false

        for _, b in ipairs(blocks) do
            -- Ignore mask
            if bit.band(bit.lshift(1, b.type or 0), ignoreMask) == 0 then
                local bx1, by1 = b.x, b.y
                local bx2, by2 = b.x + b.w, b.y + b.h

                local marginX = math.abs(vx) * dt + 2
                local marginY = math.abs(vy) * dt + 2

                if not (bx2 < x - marginX or bx1 > x2 + marginX or
                        by2 < y - marginY or by1 > y2 + marginY) then
                    -- Floor and Ceiling detection
                    if bx2 > x and bx1 < x2 then
                        if by1 >= y2 and by1 < groundY then groundY = by1 end
                        if by2 <= y and by2 > ceilingY then ceilingY = by2 end
                    end

                    -- Wall detection
                    if y2 > by1 and y < by2 then
                        if x2 >= bx1 and x < bx1 then
                            newX = bx1 - w
                            lockRight, wallContact = true, true
                            wallTimerRight = WALL_TOLERANCE
                        elseif x <= bx2 and x2 > bx2 then
                            newX = bx2
                            lockLeft, wallContact = true, true
                            wallTimerLeft = WALL_TOLERANCE
                        end
                    end
                end
            end
        end

        -- Vertical Physics (Gravity and Wall Friction)
        if not inAir then
            vy = 0
        else
            vy = math.min(speedMaxFall, vy + gravity * dt)
            if wallContact then
                -- Wall sliding friction
                local friction = wallCorrection * dt
                if vy > 0 then
                    vy = math.max(0, vy - friction)
                elseif vy < 0 then
                    vy = math.min(0, vy + friction)
                end
            end
        end

        -- Final Y Resolution
        local newY = y + vy * dt
        local currentlyInAir = true

        -- Landing on ground
        if newY + h >= groundY then
            newY = groundY - h
            vy = 0
            currentlyInAir = false
            entity.numberOfJumps = maxJumpsPossible
            groundTimer = GROUND_TOLERANCE
        end

        -- Hitting ceiling
        if vy < 0 and newY < ceilingY then
            vy = 0
            newY = ceilingY
        end

        -- Update Entity Table
        entity.x, entity.y = newX, newY
        entity.speedY, entity.speedX = vy, vx
        entity.inSky = currentlyInAir
        entity.leftLock, entity.rightLock = lockLeft, lockRight
        entity.wallTimerLeft, entity.wallTimerRight = wallTimerLeft, wallTimerRight
        entity.groundTimer = groundTimer
        entity.wallJumpLockTimer = lockTimer

        -- Reset Input Requests
        entity.requestJump = false
        entity.requestLeft = false
        entity.requestRight = false
    end
end

return physics
