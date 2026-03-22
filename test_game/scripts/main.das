require grngame_sound
require grngame_renderer

require std/all

var emit = new ParticleEmitterNode("emit")
var s = new SpriteNode("player", "player", 100, 100)
var btn = new UiNode("btn", "1", 1, centering.middle)
var inpt = new InputNode()

[export]
def on_start()
    inpt.add_pad_alias("interact",PadButton.PAD_SOUTH )

    inpt.add_pad_callback("interact",input_event.pressed, @() : void {
        print("hey\n");
    })

    let sound_info = sound_info_default()
    speech_say("Your thirst will not be quenched", sound_info)
    emit.set_lifetime(2.0)
    emit.set_spread(20.0)
    emit.set_initial_velocity(120.0)
    emit.set_gravity(300.0)
    emit.set_scale_start(8.0)
    emit.set_scale_end(1.0)
    emit.set_one_shot(false)
    s.coeff = 3

[export]
def on_update(dt : float)
    emit.update(dt)
    inpt.update(dt)

[export]
def on_render()
    cls(20, 100, 200)
    emit.render()
    btn.render()
    s.render()