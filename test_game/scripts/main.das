require grngame_sound

[export]
def on_start()
    print("on_start\n")
    let sound_info = sound_info_default()
    speech_say("hello", sound_info)

[export]
def on_update(dt: float)
    print("on_update\n")

[export]
def on_fixed_update(dt: float)
    print("on_fixed_update\n")

[export]
def on_render()
    print("on_render\n")

[export]
def on_destroy()
    print("on_destroy\n")