import "std/wren/sound" for Sound,SoundInfo, FilterDef
import "std/wren/utils" for Position,Log
import "std/wren/input" for PadNode, PadButton, InputEvent

class Wren {
  static on_start() {
    __pad = PadNode.new()
    __pad.add_alias("jump", PadButton.PAD_SOUTH)
    __pad.add_alias("music", PadButton.PAD_EAST)
    __pad.add_callback("jump", InputEvent.just_pressed, Fn.new {
        System.print("Jump! (Button South / A pressed)")
        var echo = FilterDef.echo(100, 0.3)
        Sound.speach_say(SoundInfo.new("Jump!", 0.5, 1.0, 0.0, false, 0.0, Position.new(), [echo]))
    })
    __pad.add_callback("music", InputEvent.just_pressed, Fn.new {
        System.print("Play/Restart Music! (Button East / B pressed)")
        var boost = FilterDef.bassboost()
        Sound.play(SoundInfo.new("music", 1.0, 1.0, 0.0, false, 0.0, Position.new(), [boost]))
    })
  }

  static on_update(dt) {
      if (__pad != null) {
          __pad.update(dt)

      }
  }

  static on_fixed_update(dt) {}
  static on_render() {}
  static on_destroy() {}
}

var on_start        = Fn.new { Wren.on_start() }
var on_update       = Fn.new {|dt| Wren.on_update(dt) }
var on_fixed_update = Fn.new {|dt| Wren.on_fixed_update(dt) }
var on_render       = Fn.new { Wren.on_render() }
var on_destroy      = Fn.new { Wren.on_destroy() }
