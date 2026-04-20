import "std/wren/ui/ui_node" for UiNode
import "std/wren/ui/centering" for Centering
import "std/wren/math/position" for Position
import "std/wren/renderer/alpha" for Alpha
import "std/wren/input/pad_node" for PadNode
import "std/wren/input/pad_button" for PadButton
import "std/wren/dev/log" for Log
import "std/wren/audio/sound" for Sound
import "std/wren/audio/sound_info" for SoundInfo

class Main {
  static on_start() {
    __pad = PadNode.new()
    __frame = 0
    __selected = 0
    __textures = ["1", "2", "3"]
    __centers = [
      Centering.middle,
      Centering.middle_down,
      Centering.middle_up,
      Centering.left,
      Centering.left_down,
      Centering.left_up,
      Centering.right,
      Centering.right_down,
      Centering.right_up
    ]
    //Sound.play(SoundInfo.new("music"))

    __ui_root = UiNode.new("ui_root", "1", 4, Centering.middle, Position.new(0, 0), false, Alpha.opaque, 0)

    __ui_panel = UiNode.new("littlebutton", "2", 3, Centering.left_down, Position.new(0, 0), false, Alpha.opaque, 0)
    __ui_root.add_child(__ui_panel)

    __ui_panel2 = UiNode.new("otherlittlebutton", "3", 2, Centering.middle_up, Position.new(0, 0), false, Alpha.opaque, 0)
    __ui_panel.add_child(__ui_panel2)

    __ui_panel3 = UiNode.new("otherlittlebutton3", "1", 1, Centering.left, Position.new(0, 0), false, Alpha.opaque, 0)
    __ui_panel.add_child(__ui_panel3)

    __nodes = [__ui_root, __ui_panel, __ui_panel2, __ui_panel3]

    Log.log_info("UI stress test ready")
    Log.log_info("START/BACK select node | SOUTH/NORTH center | WEST/EAST texture")
    Log.log_info("LB/RB coeff | Triggers alpha | Right stick rotation | LS flip")
    log_selected_()
  }

  static get_selected_() { __nodes[__selected] }

  static mod_index_(value, size) {
    var r = value % size
    if (r < 0) return r + size
    return r
  }

  static clamp_(v, min, max) {
    if (v < min) return min
    if (v > max) return max
    return v
  }

  static center_index_(centerValue) {
    for (i in 0...__centers.count) {
      if (__centers[i] == centerValue) return i
    }
    return 0
  }

  static texture_index_(name) {
    for (i in 0...__textures.count) {
      if (__textures[i] == name) return i
    }
    return 0
  }

  static log_selected_() {
    var node = get_selected_()
    Log.log_info("Selected %(node.name) [index %(__selected)]")
  }

  static next_selected_(step) {
    __selected = mod_index_(__selected + step, __nodes.count)
    log_selected_()
  }

  static cycle_center_(step) {
    var node = get_selected_()
    var index = center_index_(node.center)
    node.center = __centers[mod_index_(index + step, __centers.count)]
  }

  static cycle_texture_(step) {
    var node = get_selected_()
    var index = texture_index_(node.ui_name)
    node.ui_name = __textures[mod_index_(index + step, __textures.count)]
  }

  static change_coeff_(delta) {
    var node = get_selected_()
    node.coeff = clamp_(node.coeff + delta, 0.25, 8.0)
  }

  static change_alpha_(delta) {
    var node = get_selected_()
    node.alpha = clamp_(node.alpha + delta, 0, 255)
  }

  static change_rotation_(delta) {
    var node = get_selected_()
    node.rotation = node.rotation + delta
  }

  static toggle_flip_() {
    var node = get_selected_()
    node.flip = !node.flip
  }

  static handle_pad_(dt) {
    __pad.update(dt)
    if (!__pad.has_pad) return

    var index = __pad.index

    if (PadNode.just_pressed(PadButton.PAD_START, index)) next_selected_(1)
    if (PadNode.just_pressed(PadButton.PAD_BACK, index)) next_selected_(-1)

    if (PadNode.just_pressed(PadButton.PAD_SOUTH, index)) cycle_center_(1)
    if (PadNode.just_pressed(PadButton.PAD_NORTH, index)) cycle_center_(-1)

    if (PadNode.just_pressed(PadButton.PAD_WEST, index)) cycle_texture_(1)
    if (PadNode.just_pressed(PadButton.PAD_EAST, index)) cycle_texture_(-1)

    if (PadNode.pressed(PadButton.PAD_LB, index)) change_coeff_(-dt * 2.5)
    if (PadNode.pressed(PadButton.PAD_RB, index)) change_coeff_(dt * 2.5)

    if (__pad.trigger_l > 0.05) change_alpha_(-240 * __pad.trigger_l * dt)
    if (__pad.trigger_r > 0.05) change_alpha_(240 * __pad.trigger_r * dt)

    if (__pad.stick_rx > 0.12 || __pad.stick_rx < -0.12) {
      change_rotation_(200 * __pad.stick_rx * dt)
    }

    if (PadNode.just_pressed(PadButton.PAD_LS, index)) toggle_flip_()
  }

  static on_update(dt) {
    __frame = __frame + 1
    if(__frame ==1){
          __pad.rumble(0,30000,1000)
    }
    handle_pad_(dt)
  }

  static on_fixed_update(dt) {}
  
  static on_render() {
    __ui_root.render()
  }
  
  static on_destroy() {
  }
}
