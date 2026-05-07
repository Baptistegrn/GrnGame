import "std/wren/dev/log" for Log
import "std/wren/audio/sound_info" for SoundInfo
import "std/wren/audio/filter_def" for FilterDef

class Sound {
    foreign static play(name, volume, pitch, pan, looping, fade_in, pos_x, pos_y, filters)
    foreign static stop(name)
    foreign static is_playing(name)
    foreign static info_at(x, y)
    foreign static speach_say(name, volume, pitch, pan, looping, fade_in, pos_x, pos_y, filters)
    foreign static set_listener_position(x, y)

    static play(info) {
        Log.log_info("Playing sound: %(info.toString)")
        play(info.name, info.volume, info.pitch, info.pan, info.looping, info.fade_in, info.position.x, info.position.y, info.filters)
    }
    static speach_say(info) {
        Log.log_info("Playing speech: %(info.toString)")
        speach_say(info.name, info.volume, info.pitch, info.pan, info.looping, info.fade_in, info.position.x, info.position.y, info.filters)
    }


}
