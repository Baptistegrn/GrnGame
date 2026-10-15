import "std/wren/core/event" for Event
import "std/wren/core/time" for Time
import "std/wren/data/json" for Json
import "std/wren/dev/log" for Log
import "std/wren/dev/exit" for Exit
import "std/wren/audio/sound" for Sound
import "std/wren/audio/sound_info" for SoundInfo
import "std/wren/audio/filter_def" for FilterDef
import "std/wren/math/vec2" for Vec2
import "std/wren/renderer/texture" for Texture

class Main {
static on_start() {
    Event.callback(Event.DataNewVersion, Fn.new {|version_old, version_new|
        Log.info("%(version_old) -> %(version_new)")
    })

}

static on_update(dt) {
}
static on_fixed_update(dt) {}
static on_render() {
}
static on_destroy() {}


}