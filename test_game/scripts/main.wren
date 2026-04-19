import "std/wren/all" for Sound, SoundInfo, FilterDef, Position, Log, PadNode, InputEvent, PadButton, Window

class Wren {
  static on_start() {
    System.print("Game started!")

    // Add a simple bassboost filter
    var boost = FilterDef.bassboost(5)

    Window.apply_black_stripes()

    // Test sound playback
    // Sound.play(SoundInfo.new("init_sound", 1.0, 1.0, 0.0, false, 0.0, Position.new(), [boost]))

    __pad1 = PadNode.new()
    __pad1.add_alias("jump", PadButton.PAD_SOUTH)
    __pad1.add_alias("action", PadButton.PAD_EAST)
    __pad1.add_alias("start", PadButton.PAD_START)
    
    __pad1.add_callback("jump", InputEvent.just_pressed, Fn.new {
        System.print("Jump Pressed!")
    })

    __pad1.add_callback("action", InputEvent.just_pressed, Fn.new {
        System.print("Action Pressed!")
        var echo = FilterDef.echo(100, 0.3)
        Sound.speach_say(SoundInfo.new("Action!", 0.5, 1.0, 0.0, false, 0.0, Position.new(), [echo]))
    })
    
    __pad1.add_callback("start", InputEvent.just_pressed, Fn.new {
        var w = Window.offset_x
        var h = Window.offset_y
        System.print("Start Pressed! Window size => Width: %(w), Height: %(h)")
    })

    __frame = 0
  }

  static on_update(dt) {
    __pad1.update(dt)
    __frame = __frame + 1
  }

  static on_fixed_update(dt) {}
  
  static on_render() {
  }
  
  static on_destroy() {
  }
}

var on_start        = Fn.new { Wren.on_start() }
var on_update       = Fn.new {|dt| Wren.on_update(dt) }
var on_fixed_update = Fn.new {|dt| Wren.on_fixed_update(dt) }
var on_render       = Fn.new { Wren.on_render() }
var on_destroy      = Fn.new { Wren.on_destroy() }
