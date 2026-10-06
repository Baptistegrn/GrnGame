import "std/wren/math/vec2" for Vec2

class SoundInfo {
  construct new(name) {
    _name = name
    _volume = 1.0
    _pitch = 1.0
    _pan = 0.0
    _looping = false
    _fade_in = 0.0
    _position = Vec2.new(Num.nan, Num.nan)
    _filters = []
  }

  static positional(name, x, y) {
    var info = SoundInfo.new(name)
    info.position = Vec2.new(x, y)
    return info
  }

  static music(name) {
    var info = SoundInfo.new(name)
    info.looping = true
    info.fade_in = 1.0
    return info
  }

  name { _name }
  name=(v) { _name = v }

  volume { _volume }
  volume=(v) { _volume = v }

  pitch { _pitch }
  pitch=(v) { _pitch = v }

  pan { _pan }
  pan=(v) { _pan = v }

  looping { _looping }
  looping=(v) { _looping = v }

  fade_in { _fade_in }
  fade_in=(v) { _fade_in = v }

  position { _position }
  position=(v) { _position = v }

  filters { _filters }
  filters=(v) { _filters = v }

  has_position { !_position.x.isNan && !_position.y.isNan }

  toString {
    var filter_str = _filters.map {|f| f.toString }.join(", ")
    var pos_str = has_position ? _position.toString : "none"
    return "SoundInfo(name=%(_name), volume=%(_volume), pitch=%(_pitch), pan=%(_pan), looping=%(_looping), fade_in=%(_fade_in), position=%(pos_str), filters=[%(filter_str)])"
  }
}