import "std/wren/math/vec2" for Vec2

class SoundInfo {
    construct new() { init_("default", 1.0, 1.0, 0.0, false, 0.0, Vec2.new(0.0, 0.0), []) }
    construct new(name) { init_(name, 1.0, 1.0, 0.0, false, 0.0, Vec2.new(0.0, 0.0), []) }
    construct new(name, volume) { init_(name, volume, 1.0, 0.0, false, 0.0, Vec2.new(0.0, 0.0), []) }
    construct new(name, volume, pitch) { init_(name, volume, pitch, 0.0, false, 0.0, Vec2.new(0.0, 0.0), []) }
    construct new(name, volume, pitch, pan) { init_(name, volume, pitch, pan, false, 0.0, Vec2.new(0.0, 0.0), []) }
    construct new(name, volume, pitch, pan, looping) { init_(name, volume, pitch, pan, looping, 0.0, Vec2.new(0.0, 0.0), []) }
    construct new(name, volume, pitch, pan, looping, fade_in) { init_(name, volume, pitch, pan, looping, fade_in, Vec2.new(0.0, 0.0), []) }
    construct new(name, volume, pitch, pan, looping, fade_in, position) { init_(name, volume, pitch, pan, looping, fade_in, position, []) }
    construct new(name, volume, pitch, pan, looping, fade_in, position, filters) { init_(name, volume, pitch, pan, looping, fade_in, position, filters) }

    init_(name, volume, pitch, pan, looping, fade_in, position, filters) {
        _name = name
        _volume = volume
        _pitch = pitch
        _pan = pan
        _looping = looping
        _fade_in = fade_in
        _position = position
        _filters = filters
    }

    name { _name }
    name=(v) { _name = v }
    volume { _volume }
    volume=(v) { _volume = v }
    pitch { _pitch }
    pitch=(v) { _pitch = v }
    pan { _pan }
    pan=(v) { _pan = v }
    looping { _looping }
    looping=(v) { _looping = v }
    fade_in { _fade_in }
    fade_in=(v) { _fade_in = v }
    position { _position }
    position=(v) { _position = v }
    filters { _filters }
    filters=(v) { _filters = v }
}
