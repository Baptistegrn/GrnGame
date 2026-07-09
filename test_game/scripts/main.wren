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

class Main {
    static on_start() {
    }

    static on_update(dt) {
        if(Keyboard.just_pressed(1,KeyCode.KEY_0)){
            System.print("touche 0")
        }

    }

    static on_fixed_update(dt) {}
    static on_render() {}
    static on_destroy() {}
}