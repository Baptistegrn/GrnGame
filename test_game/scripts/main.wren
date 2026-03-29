
import "std/wren/sound" for Sound, FilterDef

class Wren {
  static on_start() {
    var boost =FilterDef.bassboost()
    var echo = FilterDef.echo()
    Sound.play(
      "song",
      1.0,
      1.0,
      0.0,
      false,
      2.0,
      100,
      200,
      [boost,echo]
    )
    }

    static on_update(dt) {
    }

    static on_fixed_update(dt) {}
    static on_render() {}
    static on_destroy() {
    }
}

var on_start        = Fn.new { Wren.on_start() }
var on_update       = Fn.new {|dt| Wren.on_update(dt) }
var on_fixed_update = Fn.new {|dt| Wren.on_fixed_update(dt) }
var on_render       = Fn.new { Wren.on_render() }
var on_destroy      = Fn.new { Wren.on_destroy() }