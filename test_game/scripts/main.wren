import "std/wren/all" for Sound, SoundInfo, FilterDef, Position, Log, PadNode, InputEvent, PadButton

class Wren {
  static on_start() {
    var boost = FilterDef.bassboost(10)
    var echo = FilterDef.echo(100,0.3)

    Sound.speach_say(SoundInfo.new("hello I am new here , what I need to do my friend ?",3.0, 4.0, 1.0, false, 1.0, Position.new(), [echo]))

    __pad1 = PadNode.new()
    __pad2 = PadNode.new()
    System.print(__pad2.index)
    __pad1.add_alias("jump", PadButton.PAD_SOUTH)
    __pad1.add_alias("music", PadButton.PAD_EAST)
        __pad2.add_alias("jump", PadButton.PAD_SOUTH)
    __pad2.add_alias("music", PadButton.PAD_EAST)
    __pad1.add_callback("jump", InputEvent.just_pressed, Fn.new {
        var echo = FilterDef.echo(100, 0.3)
        Sound.speach_say(SoundInfo.new("Jump!", 0.5, 1.0, 0.0, false, 0.0, Position.new(), [echo]))
    })
        __pad2.add_callback("jump", InputEvent.just_pressed, Fn.new {      
        var echo = FilterDef.echo(100, 0.3)
        Sound.speach_say(SoundInfo.new("Jump! bis", 0.5, 1.0, 0.0, false, 0.0, Position.new(), [echo]))
    })
    __pad1.add_callback("music", InputEvent.just_pressed, Fn.new {
        var boost = FilterDef.bassboost()
        Sound.play(SoundInfo.new("music", 1.0, 1.0, 0.0, false, 0.0, Position.new(), [boost]))
    })
  }

  static on_update(dt) {
          __pad1.update(dt)
          __pad2.update(dt)
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
