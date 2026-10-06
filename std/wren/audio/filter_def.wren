import "std/wren/dev/log" for Log

foreign class FilterDef {
    foreign construct new()

    foreign type
    foreign type=(v)

    foreign echo_delay
    foreign echo_delay=(v)
    foreign echo_decay
    foreign echo_decay=(v)
    foreign echo_wet
    foreign echo_wet=(v)

    foreign bassboost_boost
    foreign bassboost_boost=(v)

    static echo(delay, decay, wet) {
        Log.debug("Creating Echo filter: delay=%(delay), decay=%(decay), wet=%(wet)")
        var f = FilterDef.new()
        f.type       = 0
        f.echo_delay = delay
        f.echo_decay = decay
        f.echo_wet   = wet
        return f
    }

    static echo(delay, decay) { FilterDef.echo(delay, decay, 0.5) }
    static echo(delay)        { FilterDef.echo(delay, 0.5,   0.5) }
    static echo()             { FilterDef.echo(0.3,   0.5,   0.5) }

    static bassboost(boost) {
        Log.debug("Creating Bassboost filter: boost=%(boost)")
        var f = FilterDef.new()
        f.type            = 1
        f.bassboost_boost = boost
        return f
    }

    static bassboost() { FilterDef.bassboost(1.5) }

    toString() {
        if (this.type == 1) {
            return "FilterDef(echo delay=%(this.echo_delay), decay=%(this.echo_decay), wet=%(this.echo_wet))"
        } else if (this.type == 2) {
            return "FilterDef(bassboost boost=%(this.bassboost_boost))"
        }
        return "FilterDef(unknown)"
    }
}