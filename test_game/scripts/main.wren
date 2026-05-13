import "std/wren/data/db" for Db,DbStmt
import "std/wren/dev/log" for Log
import "std/wren/audio/sound" for Sound
import "std/wren/audio/sound_info" for SoundInfo
import "std/wren/math/vec2" for Vec2
import "std/wren/audio/filter_def" for FilterDef
import "std/wren/renderer/texture" for Texture

class Main {
  static on_start() {
    __x = 0
        Sound.play(SoundInfo.new("music"))
        
  }

  static on_update(dt) {
  }

  static on_fixed_update(dt) {}

  static on_render() {
    Texture.draw("1", 10, 10,3)
  }

  static on_destroy() {}
}

