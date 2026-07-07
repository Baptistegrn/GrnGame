import "std/wren/math/vec2" for Vec2
import "std/wren/input/pad/pad" for Pad
import "std/wren/input/pad/pad_button" for PadButton
import "std/wren/input/pad/pad_axis" for PadAxis
import "std/wren/input/input_event" for InputEvent

import "std/wren/dev/log" for Log

import "std/wren/audio/sound" for Sound
import "std/wren/audio/sound_info" for SoundInfo
import "std/wren/audio/filter_def" for FilterDef

class Main {
    static on_start() {
        var info = SoundInfo.new("music")
        //info.position = Vec2.new(0.0,0.0)
        Sound.music_play(info) 
        __pos = Vec2.new(0.0,0.0)
    }

    static on_update(dt) {
        __pos = __pos +0.1
        Sound.set_listener_position(__pos)
    }

    static on_fixed_update(dt) {}
    static on_render() {}
    static on_destroy() {}
}