import "std/wren/math/vec2" for Vec2

class SoundInfo {
  construct new(name) {
    _name = name
    _volume = 1.0
    _pitch = 1.0
    _pan = 0.0         
    _looping = false
    _fade_in = 0.0     
    _position = Vec2.new()
    _filters = []
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
    toString {
    var filter_str = ""
    for (f in _filters) {
        if (filter_str != "") filter_str = filter_str + ", "
        filter_str = filter_str + f.toString()
    }
    return "SoundInfo(name=%(name), volume=%(volume), pitch=%(pitch), pan=%(pan), looping=%(looping), fade_in=%(fade_in), position=%(position.toString), filters=[%(filter_str)])"
}
}

