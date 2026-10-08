import "std/wren/core/event" for Event
import "std/wren/core/time" for Time
import "std/wren/data/json" for Json
import "std/wren/dev/log" for Log
import "std/wren/dev/exit" for Exit
import "std/wren/audio/sound" for Sound
import "std/wren/audio/sound_info" for SoundInfo
import "std/wren/audio/filter_def" for FilterDef
import "std/wren/math/vec2" for Vec2

class Main {
static on_start() {
    // var info = SoundInfo.new("test")
    // Sound.play(info)

    // var info = SoundInfo.new("test")
    // info.volume = 0.1
    // info.pitch = 1.3      // speed up 
    // info.pan = -0.5       // at the left
    // Sound.play(info)

    // var info = SoundInfo.positional("test", -100, -90) // should up progressively from right to the middle and after go to the left 
    // Sound.play(info)
    __x = 0
    __y = 0
    var a = Animal.new("rock")
    var b= Animal.new("rocky")
    System.print(b.toString())
    var info = SoundInfo.new("test")
    info.volume = 10
    info.filters = [FilterDef.echo(0.3, 2.0, 4.0), FilterDef.bassboost(3.0)]
    Sound.play(info)
}

static on_update(dt) {
    __x = __x -0.3
    //__y = __y +0.3
Sound.set_listener_position(Vec2.new(__x,__y))
}
static on_fixed_update(dt) {}
static on_render() {}
static on_destroy() {}


}