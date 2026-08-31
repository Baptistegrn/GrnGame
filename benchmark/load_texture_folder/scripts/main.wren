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

/*
 * Environment: Desktop, 8 Threads
 * Palette Size: 32 colors
 *
 * | Test | Img Count | Unique Pixels | Grid Size | Time Before | Time After | Gain  |
 * |------|-----------|---------------|-----------|-------------|------------|-------|
 * | 1    | 100       | 128           | 100x100   | 0.979640s   | 0.224287s  | ~4.3x |
 * | 2    | 10        | 128           | 1000x1000 | 0.355364s   | 0.158079s  | ~2.2x |
 * | 3    | 100       | 1024          | 100x100   | 1.287326s   | 0.337792s  | ~3.8x |
 * | 4    | 10        | 1024          | 1000x1000 | 0.357195s   | 0.115759s  | ~3.1x |
 */

class Main {
    static on_start() {
    }

    static on_update(dt) {

    }

    static on_fixed_update(dt) {}
    static on_render() {
        Texture.draw("image_palette_1",30,30,4)
    }
    static on_destroy() {}
}
