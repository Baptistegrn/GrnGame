require grngame_sound
require grngame_renderer

require std/all

var emit : ParticleEmitter
var s = new SpriteNode("player", 100, 100)
var btn = new UiNode("1", 127, 123, centering.left_up)
var btn2 = new UiNode("3",127,123,centering.middle)
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
    s.render()
    emitter_render(emit)
    btn.render()
    btn2.render()
