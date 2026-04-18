import "std/wren/dev/log" for Log

foreign class FilterDef {
    foreign construct new()

    foreign type
    foreign type=(v)

    foreign reverb_room
    foreign reverb_room=(v)
    foreign reverb_damp
    foreign reverb_damp=(v)
    foreign reverb_width
    foreign reverb_width=(v)
    foreign reverb_wet
    foreign reverb_wet=(v)

    foreign echo_delay
    foreign echo_delay=(v)
    foreign echo_decay
    foreign echo_decay=(v)
    foreign echo_wet
    foreign echo_wet=(v)

    foreign bassboost_boost
    foreign bassboost_boost=(v)

    static reverb(room, damp, width, wet) {
        var f = FilterDef.new()
        f.type         = 0
        f.reverb_room  = room
        f.reverb_damp  = damp
        f.reverb_width = width
        f.reverb_wet   = wet
        return f
    }

    static reverb(room, damp) { FilterDef.reverb(room, damp, 1.0, 0.5) }
    static reverb(room)       { FilterDef.reverb(room, 0.5, 1.0, 0.5)  }
    static reverb()           { FilterDef.reverb(0.5,  0.5, 1.0, 0.5)  }

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
