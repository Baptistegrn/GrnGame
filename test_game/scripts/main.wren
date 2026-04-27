import "std/wren/ui/ui_node" for UiNode
import "std/wren/ui/ui_info" for UiInfo
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
    __button = UiNode.new("button",UiInfo.new("1"))
  }

  static on_update(dt) {
  }

  static on_fixed_update(dt) {}
  
  static on_render() {
    __button.render()
  }
  
  static on_destroy() {
  }
}
