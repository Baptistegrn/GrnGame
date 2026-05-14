import "std/wren/data/db" for Db,DbStmt
import "std/wren/dev/log" for Log
import "std/wren/audio/sound" for Sound
import "std/wren/audio/sound_info" for SoundInfo
import "std/wren/math/vec2" for Vec2
import "std/wren/audio/filter_def" for FilterDef
import "std/wren/renderer/texture" for Texture
import "std/wren/input/pad_node" for PadNode

class Main {
  static on_start() {
    __x = 0
    __i = PadNode.new()
  }

  static on_update(dt) {
    __i.update(dt)
    __x = __x + __i.trigger_l *dt *200.0
    __x = __x - __i.trigger_r *dt *200.0
  }

  static on_fixed_update(dt) {}

  static on_render() {

    Texture.draw("1", __x, 3, 1)
    Texture.draw("4", __x, 20,1)
  }

  static on_destroy() {}
}

