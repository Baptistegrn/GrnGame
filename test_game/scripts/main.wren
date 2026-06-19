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
    __pad = PadNode.new()

    __pad.add_alias("resize",      PadButton.PAD_SOUTH)
    __pad.add_alias("fullscreen",  PadButton.PAD_EAST)
    __pad.add_alias("resizable",   PadButton.PAD_NORTH)
    __pad.add_alias("clear_color", PadButton.PAD_WEST)
    __pad.add_alias("palette",     PadButton.PAD_RB)

    // --- Toggle entre deux tailles de fenetre ---
    __bigWindow = false
    __pad.add_callback("resize", InputEvent.just_pressed, Fn.new {
      __bigWindow = !__bigWindow
      if (__bigWindow) {
        Config.windowWidth  = 1280
        Config.windowHeight = 720
      } else {
        Config.windowWidth  = 640
        Config.windowHeight = 360
      }
      Log.log_info("Resize -> %(Config.windowWidth)x%(Config.windowHeight)")
      Config.apply()
    })

    // --- Toggle plein ecran ---
    __pad.add_callback("fullscreen", InputEvent.just_pressed, Fn.new {
      Config.fullscreen = !Config.fullscreen
      Log.log_info("Fullscreen -> %(Config.fullscreen)")
      Config.apply()
    })

    // --- Toggle resizable ---
    __pad.add_callback("resizable", InputEvent.just_pressed, Fn.new {
      Config.resizable = !Config.resizable
      Log.log_info("Resizable -> %(Config.resizable)")
      Config.apply()
    })

    // --- Cycle sur la couleur de clear (index 0 a 3) ---
    __clearIndex = 0
    __pad.add_callback("clear_color", InputEvent.just_pressed, Fn.new {
      __clearIndex = (__clearIndex + 1) % 4
      Config.renderClear = __clearIndex
      Log.log_info("RenderClear -> %(Config.renderClear)")
      Config.apply()
    })

    // --- Recharge une nouvelle palette de couleurs ---
    __pad.add_callback("palette", InputEvent.just_pressed, Fn.new {
      Config.colorPalette = [
        "#000000", "#1A1A2E", "#16213E", "#0F3460",
        "#E94560", "#FF2E63", "#08D9D6", "#252A34"
      ]
      Log.log_info("Palette rechargee")
      Config.apply()
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
