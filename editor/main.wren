
class Main {
    static on_start() {}
    static on_update(dt) {}
    static on_fixed_update(dt) {}
    static on_render() {}
    static on_destroy() {}
}

var on_start        = Fn.new { Main.on_start() }
var on_update       = Fn.new {|dt| Main.on_update(dt) }
var on_fixed_update = Fn.new {|dt| Main.on_fixed_update(dt) }
var on_render       = Fn.new { Main.on_render() }
var on_destroy      = Fn.new { Main.on_destroy() }