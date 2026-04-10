import "std/wren/utils" for Position, Log

class SoundInfo {
    construct new() {
        _name    = "default"
        _volume  = 1.0
        _pitch   = 1.0
        _pan     = 0.0
        _looping = false
        _fade_in = 0.0
        _position = Position.new(0.0, 0.0)
        _filters = []
    }

    construct new(name) {
        _name    = name
        _volume  = 1.0
        _pitch   = 1.0
        _pan     = 0.0
        _looping = false
        _fade_in = 0.0
        _position = Position.new(0.0, 0.0)
        _filters = []
    }

    construct new(name, volume) {
        _name    = name
        _volume  = volume
        _pitch   = 1.0
        _pan     = 0.0
        _looping = false
        _fade_in = 0.0
        _position = Position.new(0.0, 0.0)
        _filters = []
    }

    construct new(name, volume, pitch) {
        _name    = name
        _volume  = volume
        _pitch   = pitch
        _pan     = 0.0
        _looping = false
        _fade_in = 0.0
        _position = Position.new(0.0, 0.0)
        _filters = []
    }

    construct new(name, volume, pitch, pan) {
        _name    = name
        _volume  = volume
        _pitch   = pitch
        _pan     = pan
        _looping = false
        _fade_in = 0.0
        _position = Position.new(0.0, 0.0)
        _filters = []
    }

    construct new(name, volume, pitch, pan, looping) {
        _name    = name
        _volume  = volume
        _pitch   = pitch
        _pan     = pan
        _looping = looping
        _fade_in = 0.0
        _position = Position.new(0.0, 0.0)
        _filters = []
    }

    construct new(name, volume, pitch, pan, looping, fade_in) {
        _name    = name
        _volume  = volume
        _pitch   = pitch
        _pan     = pan
        _looping = looping
        _fade_in = fade_in
        _position = Position.new(0.0, 0.0)
        _filters = []
    }

    construct new(name, volume, pitch, pan, looping, fade_in, position) {
        _name    = name
        _volume  = volume
        _pitch   = pitch
        _pan     = pan
        _looping = looping
        _fade_in = fade_in
        _position = position
        _filters = []
    }

    construct new(name, volume, pitch, pan, looping, fade_in, position, filters) {
        _name    = name
        _volume  = volume
        _pitch   = pitch
        _pan     = pan
        _looping = looping
        _fade_in = fade_in
        _position = position
        _filters = filters
    }

    name     { _name }
    volume   { _volume }
    pitch    { _pitch }
    pan      { _pan }
    looping  { _looping }
    fade_in  { _fade_in }
    position { _position }
    filters  { _filters }

    name=(v)     { _name = v }
    volume=(v)   { _volume = v }
    pitch=(v)    { _pitch = v }
    pan=(v)      { _pan = v }
    looping=(v)  { _looping = v }
    fade_in=(v)  { _fade_in = v }
    position=(v) { _position = v }
    filters=(v)  { _filters = v }
}

class Sound {
    // bind cpp
    foreign static play(name, volume, pitch, pan, looping, fade_in, pos_x, pos_y, filters)
    foreign static stop(name)
    foreign static is_playing(name)
    foreign static info_at(x, y)
    foreign static speach_say(name, volume, pitch, pan, looping, fade_in, pos_x, pos_y, filters)

    static play(info) {
        Log.log_info("Playing sound: %(info.name) (vol: %(info.volume), pitch: %(info.pitch))")
        play(info.name, info.volume, info.pitch, info.pan, info.looping, info.fade_in, info.position.x, info.position.y, info.filters)
    }
    static speach_say(info) {
        Log.log_info("Playing speech: %(info.name) (vol: %(info.volume))")
        speach_say(info.name, info.volume, info.pitch, info.pan, info.looping, info.fade_in, info.position.x, info.position.y, info.filters)
    }
}
foreign class FilterDef {
    foreign construct new()

    foreign type
    foreign type=(v)

    // foreign reverb_room
    // foreign reverb_room=(v)
    // foreign reverb_damp
    // foreign reverb_damp=(v)
    // foreign reverb_width
    // foreign reverb_width=(v)
    // foreign reverb_wet
    // foreign reverb_wet=(v)

    foreign echo_delay
    foreign echo_delay=(v)
    foreign echo_decay
    foreign echo_decay=(v)
    foreign echo_wet
    foreign echo_wet=(v)

    foreign bassboost_boost
    foreign bassboost_boost=(v)

    // static reverb(room, damp, width, wet) {
    //     var f = FilterDef.new()
    //     f.type         = 0
    //     f.reverb_room  = room
    //     f.reverb_damp  = damp
    //     f.reverb_width = width
    //     f.reverb_wet   = wet
    //     return f
    // }

    // static reverb(room, damp) { FilterDef.reverb(room, damp, 1.0, 0.5) }
    // static reverb(room)       { FilterDef.reverb(room, 0.5, 1.0, 0.5)  }
    // static reverb()           { FilterDef.reverb(0.5,  0.5, 1.0, 0.5)  }

    static echo(delay, decay, wet) {
        Log.log_debug("Creating Echo filter: delay=%(delay), decay=%(decay), wet=%(wet)")
        var f = FilterDef.new()
        f.type       = 1
        f.echo_delay = delay
        f.echo_decay = decay
        f.echo_wet   = wet
        return f
    }

    static echo(delay, decay) { FilterDef.echo(delay, decay, 0.5) }
    static echo(delay)        { FilterDef.echo(delay, 0.5,   0.5) }
    static echo()             { FilterDef.echo(0.3,   0.5,   0.5) }

    static bassboost(boost) {
        Log.log_debug("Creating Bassboost filter: boost=%(boost)")
        var f = FilterDef.new()
        f.type            = 2
        f.bassboost_boost = boost
        return f
    }

    static bassboost() { FilterDef.bassboost(1.5) }
}