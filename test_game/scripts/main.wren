import "std/wren/audio/sound" for Sound
import "std/wren/audio/sound_info" for SoundInfo
import "std/wren/audio/filter_def" for FilterDef
import "std/wren/math/vec2" for Vec2
import "std/wren/renderer/texture" for Texture
import "std/wren/renderer/particule" for ParticleEmitter
import "std/wren/renderer/primitive" for Primitive 
import "std/wren/core/window" for Window
import "config" for Config
import "std/wren/dev/log" for Log
import "std/wren/dev/level" for Level
import "std/wren/dev/log_destination" for LogDestination
import "std/wren/input/pad_node" for PadNode
import "std/wren/input/pad_button" for PadButton
import "std/wren/input/input_event" for InputEvent


class Main {
  static on_start() {
    __pad = PadNode.new(PadButton.PAD_LB)

    __pad.add_alias("debug",    PadButton.PAD_SOUTH)
    __pad.add_alias("info",     PadButton.PAD_EAST)
    __pad.add_alias("warn",     PadButton.PAD_NORTH)
    __pad.add_alias("error",    PadButton.PAD_WEST)
    __pad.add_alias("critical", PadButton.PAD_RB)

    __pad.add_callback("debug", InputEvent.just_pressed, Fn.new {
            Log.log_set_lvl(Level.debug)
      Log.log_debug("debug")
      Log.log_info("info")
      Log.log_warning("warn")
      Log.log_error("error")
      Log.log_critical("critical")
    })

    __pad.add_callback("info", InputEvent.just_pressed, Fn.new {
            Log.log_set_lvl(Level.info)
      Log.log_debug("debug")
      Log.log_info("info")
      Log.log_warning("warn")
      Log.log_error("error")
      Log.log_critical("critical")
    })

    __pad.add_callback("warn", InputEvent.just_pressed, Fn.new {
            Log.log_set_lvl(Level.warning)
      Log.log_debug("debug")
      Log.log_info("info")
      Log.log_warning("warn")
      Log.log_error("error")
      Log.log_critical("critical")
    })

    __pad.add_callback("error", InputEvent.just_pressed, Fn.new {
            Log.log_set_lvl(Level.error)
      Log.log_debug("debug")
      Log.log_info("info")
      Log.log_warning("warn")
      Log.log_error("error")
      Log.log_critical("critical")
    })

    __pad.add_callback("critical", InputEvent.just_pressed, Fn.new {
            Log.log_set_lvl(Level.critical)
      Log.log_debug("debug")
      Log.log_info("info")
      Log.log_warning("warn")
      Log.log_error("error")
      Log.log_critical("critical")
    })

  }

  static on_update(dt) {
    __pad.update(dt)
  }

  static on_fixed_update(dt) {}

  static on_render() {
    Texture.draw("player", 20, 0, 1, 0, 200)

  }

  static on_destroy() {}
}
