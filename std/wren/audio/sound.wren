import "std/wren/dev/log" for Log
import "std/wren/audio/sound_info" for SoundInfo
import "std/wren/audio/filter_def" for FilterDef
import "std/wren/math/vec2" for Vec2

class Sound {
  foreign static sound_play_(name, volume, pitch, pan, looping, fade_in, pos_x, pos_y, filters)
  foreign static sound_stop_(name)
  foreign static sound_break_(name)
  foreign static sound_is_playing_(name)
  foreign static sound_is_playing_at_(name, x, y)
  foreign static set_listener_position_(x, y)

  static play(info) {
    return sound_play_(info.name, info.volume, info.pitch, info.pan, info.looping, info.fade_in, info.position.x, info.position.y, info.filters)
  }

  static stop(name) { sound_stop_(name) }
  static cut(name) { sound_break_(name) }

  static is_playing(name) { sound_is_playing_(name) }
  static is_playing_at(name, pos) { sound_is_playing_at_(name, pos.x, pos.y) }

  static set_listener_position(pos) { set_listener_position_(pos.x, pos.y) }

  static music_play(info) { play(info) }
  static sfx_play(info) { play(info) }
  static sfx_is_playing(name) { is_playing(name) }
  static sfx_is_playing_at(name, pos) { is_playing_at(name, pos) }
}