import "std/wren/audio/sound" for Sound
import "std/wren/audio/sound_info" for SoundInfo
import "std/wren/audio/filter_def" for FilterDef
import "std/wren/math/vec2" for Vec2
import "std/wren/renderer/texture" for Texture
import "std/wren/renderer/particule" for ParticleEmitter
import "std/wren/renderer/primitive" for Primitive 

class Main {

  static on_start() {
    __emitter = ParticleEmitter.new()
    __emitter.pos_x = 100
    __emitter.pos_y = 30
    
    __emitter.lifetime = 2.0

    __emitter.explosiveness = 0.0
    __emitter.one_shot = false
    __emitter.local_coords = false

    __emitter.direction = 3.0
    __emitter.spread = 0.61     

    __emitter.initial_velocity = 250
    __emitter.gravity = 400
    __emitter.damping = 0.98
    __emitter.scale_start = 1.0
    __emitter.scale_end = 1.0
    __emitter.scale_amount_curve = 1.0

    __emitter.color_start = 5    
    __emitter.color_end = 10  
    __emitter.alpha_start = 10  
    __emitter.alpha_end =  5    

    __emitter.turbulence = 2.0
    __emitter.spawn_radius = 20.0
    __emitter.spawn_rect_h = 50.0
    __emitter.spawn_rect_w = 50.0
  }

  static on_update(dt) {
    __emitter.update(dt)
  }

  static on_fixed_update(dt) {}

  static on_render() {
    Texture.draw("player", 0, 0)
    Primitive.rect_fill(150, 30, 80, 40, 13, 9)
    Primitive.circle_fill(200, 150, 25, 4, 255)
    __emitter.render()
  }

  static on_destroy() {
  }
}