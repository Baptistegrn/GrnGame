import "std/wren/audio/sound" for Sound
import "std/wren/audio/sound_info" for SoundInfo
import "std/wren/audio/filter_def" for FilterDef
import "std/wren/math/vec2" for Vec2
import "std/wren/renderer/texture" for Texture
import "std/wren/renderer/particule" for ParticleEmitter
import "std/wren/renderer/primitive" for Primitive 


class Main {
  static on_start() {}

  static on_update(dt) {}

  static on_fixed_update(dt) {}

  static on_render() {Texture.draw("player", 0, 0,1,0,200)}

  static on_destroy() {}
}