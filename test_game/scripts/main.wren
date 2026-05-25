import "std/wren/audio/sound" for Sound
import "std/wren/audio/sound_info" for SoundInfo
import "std/wren/audio/filter_def" for FilterDef
import "std/wren/math/vec2" for Vec2

class Main {

  static on_start() {
    System.print("Démarrage du test audio...")
    __x = 0.0
    Sound.set_listener_position(0.0, 0.0)

    var myFilter = FilterDef.new()
    myFilter.type = 2 
    myFilter.echo_delay = 0.3
    myFilter.echo_decay = 0.5
    myFilter.echo_wet = 0.8
    var info = SoundInfo.new("music")
    info.volume = 1.0
    info.pitch = 1.0
    info.pan = 0.0
    info.looping = true
    info.fade_in = 1.5 
    info.position = Vec2.new(0.0,0.0)
    info.filters = [myFilter]
    Sound.sfx_play(info)
  }

  static on_update(dt) {
    __x = __x + (3.0 * dt)
    Sound.set_listener_position(__x, 0.0)
  }

  static on_fixed_update(dt) {}

  static on_render() {}

  static on_destroy() {
    Sound.stop("music")
  }
}