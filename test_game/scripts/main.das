require grngame_sound
require grngame_renderer

require std/all

var s = new SpriteNode("player", "player", 100, 100, false, 200, 100)
var btn = new UiNode("btn", "1", 1, centering.middle)
var inpt = new PadInputNode()
var emit = new ParticleEmitterNode("emit")

[export]
def on_start()
    inpt.add_alias("interact", PadButton.PAD_LB)

    inpt.add_callback("interact", input_event.pressed, @() : void {
        print("hey\n");
    })

    let sound_info = sound_info_default()
    speech_say("Your thirst will not be quenched", sound_info)

    emit.set_amount(60)
    emit.set_lifetime(3.0)
    emit.set_explosiveness(0.3)
    emit.set_one_shot(true)
    emit.set_spread(6.3)
    emit.set_initial_velocity(50.0)
    emit.set_direction(0.0)
    emit.set_gravity(30.0)
    emit.set_damping(0.90)
    emit.set_scale_start(3.0)
    emit.set_scale_end(1.0)
    emit.set_color_start(255, 0, 0, 255)
    emit.set_color_end(255, 0, 0, 255)
    emit.set_positions(100.0, 100.0)

[export]
def on_update(dt : float)
    emit.update(dt)
    inpt.update(dt);

[export]
def on_render()
    cls(20, 100, 200)
    btn.render()
    s.render()
    emit.render()