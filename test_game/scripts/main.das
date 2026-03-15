require grngame_sound
require grngame_renderer

require std/all

var emit = new ParticleEmitterNode("emit")
var s = new SpriteNode("player", 100, 100)
var btn = new UiNode("1", 1, centering.middle)

[export]
def on_start()
    let sound_info = sound_info_default()
    speech_say("Baptiste will you marry me, my Indian love ?", sound_info)
    emit.set_lifetime(2.0)
    emit.set_spread(180.0)
    emit.set_initial_velocity(120.0)
    emit.set_gravity(50.0)
    emit.set_scale_start(8.0)
    emit.set_scale_end(1.0)
    emit.set_one_shot(false)

[export]
def on_update(dt : float)
    emit.update(dt)

[export]
def on_render()
    cls(20,100,200)
    s.render()
    emit.render()
    btn.render()