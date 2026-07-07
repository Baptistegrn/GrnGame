import "std/wren/dev/log" for Log
import "std/wren/audio/sound_info" for SoundInfo
import "std/wren/audio/filter_def" for FilterDef
import "std/wren/math/vec2" for Vec2

class Sound {
  foreign static sound_play_music_(name, volume, pitch, pan, looping, fade_in, pos_x, pos_y, filters)
  foreign static music_stop_(name)
  foreign static music_is_playing_(name)
  foreign static music_is_playing_at_(name, x, y)
  foreign static sound_play_sfx_(name, volume, pitch, pan, looping, fade_in, pos_x, pos_y, filters)
  foreign static sfx_is_playing_(name)
  foreign static sfx_is_playing_at_(name, x, y)
  foreign static speech_say_(name, volume, pitch, pan, looping, fade_in, pos_x, pos_y, filters)
  foreign static set_listener_position_(x, y)

  static play(info) {
    sound_play_sfx_(info.name, info.volume, info.pitch, info.pan, info.looping, info.fade_in, info.position.x, info.position.y, info.filters)
  }

  static music_play(info) {
    sound_play_music_(info.name, info.volume, info.pitch, info.pan, info.looping, info.fade_in,info.position.x, info.position.y, info.filters)
  }

  static stop(name) {
    music_stop_(name)
  }

  static is_playing(name) {
    return music_is_playing_(name)
  }

  static is_playing_at(name,pos) {
    return music_is_playing_at_(name,pos.x,pos.y)
  }

  static sfx_play(info) {
    sound_play_sfx_(info.name, info.volume, info.pitch, info.pan, info.looping, info.fade_in,info.position.x, info.position.y, info.filters)
  }

  static sfx_is_playing(name) {
    return sfx_is_playing_(name)
  }

  static sfx_is_playing_at(name,pos) {
    return sfx_is_playing_at_(name,pos.x,pos.y)
  }

  static speech_say(info) {
    speech_say_(info.name, info.volume, info.pitch, info.pan, info.looping, info.fade_in, info.position.x, info.position.y, info.filters)
  }

  static set_listener_position(pos) {
    set_listener_position_(pos.x,pos.y)
  }
}