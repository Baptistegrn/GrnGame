import "std/wren/audio/sound"       for Sound
import "std/wren/audio/sound_info"  for SoundInfo
import "std/wren/math/vec2"         for Vec2
import "std/wren/renderer/texture"  for Texture
import "std/wren/input/pad_node"    for PadNode
import "std/wren/input/pad_button"  for PadButton
import "std/wren/input/input_event" for InputEvent
import "std/wren/ui/ui_node"        for UiNode
import "std/wren/ui/ui_info"        for UiInfo
import "std/wren/ui/centering"      for Centering
import "std/wren/dev/log"           for Log

class Main {

  static on_start() {
    __time      = 0.0
    __angle     = 0.0
    __score     = 0
    __speed     = 60.0
    __music_vol = 1.0
    __offsets   = []

    __pad = PadNode.new()

    // 5 callbacks manette
    __pad.add_callback_raw(PadButton.PAD_SOUTH, InputEvent.just_pressed,
      Fn.new { __score = __score + 10 })

    __pad.add_callback_raw(PadButton.PAD_EAST, InputEvent.just_pressed,
      Fn.new { __angle = 0.0 })

    __pad.add_callback_raw(PadButton.PAD_NORTH, InputEvent.just_pressed,
      Fn.new { __speed = __speed + 10.0 })

    __pad.add_callback_raw(PadButton.PAD_WEST, InputEvent.just_pressed,
      Fn.new {
        var s = __speed - 10.0
        __speed = s < 10.0 ? 10.0 : s
      })

    __pad.add_callback_raw(PadButton.PAD_START, InputEvent.just_pressed,
      Fn.new { __music_vol = __music_vol > 0.5 ? 0.0 : 1.0 })

    // 3 sons
    __snd_a = SoundInfo.new("1", 0.8, 1.0, 0.0, true, 0.5, Vec2.new(0.0,   0.0), [])
    __snd_b = SoundInfo.new("1", 0.5, 1.2, 0.0, true, 0.0, Vec2.new(20.0,  0.0), [])
    __snd_c = SoundInfo.new("1", 0.6, 0.9, 0.0, true, 0.2, Vec2.new(-20.0, 0.0), [])

    Sound.play(__snd_a)
    Sound.play(__snd_b)
    Sound.play(__snd_c)

    // 5 UI nodes
    __ui_tl = UiNode.new("ui_tl", UiInfo.new("1", 1, Centering.left_up,    Vec2.new(4.0, 4.0)))
    __ui_tr = UiNode.new("ui_tr", UiInfo.new("2", 1, Centering.right_up,   Vec2.new(4.0, 4.0)))
    __ui_bl = UiNode.new("ui_bl", UiInfo.new("3", 1, Centering.left_down,  Vec2.new(4.0, 4.0)))
    __ui_br = UiNode.new("ui_br", UiInfo.new("4", 1, Centering.right_down, Vec2.new(4.0, 4.0)))
    __ui_mc = UiNode.new("ui_mc", UiInfo.new("1", 1, Centering.middle,     Vec2.new(0.0, 0.0)))

    Log.log_info("Stress test démarré")
  }

  static on_update(dt) {
    __pad.update(dt)

    __time  = __time  + dt
    __angle = __angle + dt * 45.0

    // Stick gauche Y → speed
    var s = __speed + __pad.stick_ly * dt * 5.0
    __speed = s < 10.0 ? 10.0 : (s > 300.0 ? 300.0 : s)

    // Triggers → volume musique (clamp manuel)
    var v = __music_vol + __pad.trigger_l * dt * 0.5 - __pad.trigger_r * dt * 0.5
    __music_vol = v < 0.0 ? 0.0 : (v > 1.0 ? 1.0 : v)

    // Boucle maths : 10 offsets sin/cos
    __offsets = []
    var i = 0
    while (i < 10) {
      var phase = __time + i * 0.628
      __offsets.add(Vec2.new(phase.sin * 30.0, (phase * 1.3).cos * 15.0))
      i = i + 1
    }

    // Positions sons changeantes
    __snd_a.position = Vec2.new(__time.sin * 40.0, 0.0)
    __snd_b.position = Vec2.new((__time * 1.5).cos * 60.0, __time.sin * 20.0)
    __snd_c.position = Vec2.new(-(__time * 0.7).sin * 50.0, (__time * 2.0).cos * 10.0)

    Sound.set_listener_position(0.0, 0.0)

    if (__score > 999) {
      __score = 0
      Log.log_info("Score reset")
    }
  }

  static on_fixed_update(dt) {}

  static on_render() {
    // 10 × "1" : offsets animés + rotation + scale
    var i = 0
    while (i < 10) {
      var sc  = 1.0 + (__time + i).sin * 0.15
      var rot = __angle + i * 36.0
      Texture.draw("1", __offsets[i].x, __offsets[i].y, sc, rot)
      i = i + 1
    }

    // 8 × "2" : défilement horizontal
    i = 0
    while (i < 8) {
      var px = (__time * __speed + i * 24.0) % 320.0 - 160.0
      Texture.draw("2", px, -50.0, 1.0, 0.0)
      i = i + 1
    }

    // 6 × "3" : cercle tournant
    i = 0
    while (i < 6) {
      var phase = __time + i * 1.047
      Texture.draw("3", phase.cos * 80.0, phase.sin * 80.0, 1.0, __angle * -1.0)
      i = i + 1
    }

    // 6 × "4" : zigzag vertical
    i = 0
    while (i < 6) {
      var py = (__time * 40.0 + i * 20.0).sin * 40.0
      Texture.draw("4", (i - 2.5) * 30.0, py, 1.2, 0.0)
      i = i + 1
    }

    // 5 UI
    __ui_tl.render()
    __ui_tr.render()
    __ui_bl.render()
    __ui_br.render()
    __ui_mc.render()
  }

  static on_destroy() {
    Sound.stop("music")
    Log.log_info("Fin. Score : %(__score)")
  }
}