local animation = {}

--- Updates a frame-based animation
-- @param currentFrame The current index of the sprite frame
-- @param timer Accumulated time since the last frame swap
-- @param startFrame The first frame index of the animation sequence
-- @param endFrame The last frame index of the animation sequence
-- @param speed The duration (in seconds) of each frame
-- @param dt Delta time from the current frame
-- @param loop Boolean: if true, restarts; if false, stays on the last frame
-- @return newFrame, newTimer
function animation.update(currentFrame, timer, startFrame, endFrame, speed, dt, loop)
    timer = timer + dt
    if timer >= speed then
        timer = 0
        currentFrame = currentFrame + 1
    end
    if currentFrame < startFrame or currentFrame > endFrame then
        if loop then
            currentFrame = startFrame
        else
            currentFrame = endFrame
        end
    end

    return currentFrame, timer
end

return animation
