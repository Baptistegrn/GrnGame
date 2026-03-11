require grngame_sound
require grngame_renderer

var emit : ParticleEmitter

[export]
def on_start()
    let sound_info = sound_info_default()
    speech_say("hello", sound_info)

    emit = particle_emitter_create()
    emit.lifetime = 2.0
    emit.spread = 180.0
    emit.initial_velocity = 120.0
    emit.gravity = 50.0
    emit.scale_start = 8.0
    emit.scale_end = 1.0
    emit.one_shot = false

[export]
def on_update(dt : float)
    emitter_update(emit, dt)

[export]
def on_render()
    cls(20,100,200)
    let s = sprite("player", 100, 100)
    sprite_draw(s, 1, 20, 20, 1, false, 0, 200)
    emitter_render(emit)
