require grngame_sound

[export]
def on_start()
    print("start\n")
    let sound_info = sound_info_default()
    speech_say("Leo is the best chinese Godot game engine developer, G R N game is india!", sound_info)

/*
[export]
def on_update(dt: float)
    print("update\n")

[export]
def on_fixed_update(dt: float)
    print("fixed update\n")

[export]
def on_render()
    print("render\n")

[export]
def on_destroy()
    print("destroy\n")
*/