import "std/wren/data/db" for Db,DbStmt
import "std/wren/dev/log" for Log
import "std/wren/audio/sound" for Sound
import "std/wren/audio/sound_info" for SoundInfo
import "std/wren/math/vec2" for Vec2
import "std/wren/audio/filter_def" for FilterDef

class Main {
  static on_start() {
    __x = 0
        Sound.play(SoundInfo.new("music", 1, 1.0, 0.0, false, 0.0, Vec2.new(100.0, 0.0)))
  }

  static on_update(dt) {
    __x= __x+1
  Sound.set_listener_position(__x,0)
  }

  static on_fixed_update(dt) {}

  static on_render() {}

  static on_destroy() {}
}

