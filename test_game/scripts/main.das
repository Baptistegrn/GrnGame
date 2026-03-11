require grngame_sound
require grngame_renderer

[export]
def on_start()
    let sound_info = sound_info_default()
    speech_say("hello", sound_info)


[export]
def on_render()
    let s = sprite("player", 100, 100)
    sprite_draw(s,1,20.0,20.0,1,true,0,200)
