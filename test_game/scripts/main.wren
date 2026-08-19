import "std/wren/math/vec2" for Vec2
import "std/wren/input/pad/pad" for Pad
import "std/wren/input/pad/pad_button" for PadButton
import "std/wren/input/pad/pad_axis" for PadAxis
import "std/wren/input/input_event" for InputEvent

import "std/wren/dev/log" for Log

import "std/wren/audio/sound" for Sound
import "std/wren/audio/sound_info" for SoundInfo
import "std/wren/audio/filter_def" for FilterDef

import "std/wren/input/keyboard/key_code" for KeyCode
import "std/wren/input/keyboard/keyboard" for Keyboard
import "std/wren/renderer/primitive" for Primitive
import "std/wren/renderer/texture" for Texture

class Main {
    static on_start() {
        __x  = 400
        __y = 400
        Sound.play(SoundInfo.new("music"))
    }

    static on_update(dt) {
        if(Keyboard.pressed(0,KeyCode.KEY_Z)){
            __x = __x -1000 *dt
        }
        if(Keyboard.pressed(0,KeyCode.KEY_S)){
            __x = __x +1000 *dt
        }
        if(Keyboard.pressed(0,KeyCode.KEY_Q)){
            __y = __y -1000 *dt
        }
        if(Keyboard.pressed(0,KeyCode.KEY_D)){
            __y = __y +1000 *dt
        }

    }

    static on_fixed_update(dt) {}
    static on_render() {
        Primitive.rect_fill(__y,__x,48,48,1,255)
    }
    static on_destroy() {}
}
