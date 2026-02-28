song.loadFolder("assets/sounds")

utils.setUpdateCallback(function()
    image.cls(20, 20, 20)

    if input.keyJustPressed("space") then
        song.play("assets/sounds/music.wav", 0, 0, 80)
    end

    if input.keyJustPressed("l") then
        song.play("assets/sounds/music.wav", -1, 0, 80)
    end

    if input.keyJustPressed("s") then
        song.stop("assets/sounds/music.wav")
    end

    if input.keyJustPressed("p") then
        song.pause("assets/sounds/music.wav")
    end

    if input.keyJustPressed("r") then
        song.resume("assets/sounds/music.wav")
    end

    if input.keyJustPressed("up") then
        song.stopChannel(0)
    end
end)
