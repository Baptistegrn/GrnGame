import "std/wren/ui/ui_node" for UiNode
import "std/wren/ui/centering" for Centering
import "std/wren/math/position" for Position
import "std/wren/renderer/alpha" for Alpha

class Main {
  static on_start() {
    __ui_root = UiNode.new("ui_root", "1", 3, Centering.middle, Position.new(0, 0), false, Alpha.opaque, 0)

    __ui_panel = UiNode.new("littlebutton", "1", 2, Centering.left_down)
    __ui_root.add_child(__ui_panel)

        __ui_panel2 = UiNode.new("otherlittlebutton", "1", 1, Centering.middle_down)
    __ui_panel.add_child(__ui_panel2)
            __ui_panel3 = UiNode.new("otherlittlebutton", "1", 1, Centering.right_down)
    __ui_panel.add_child(__ui_panel3)

  }

  static on_update(dt) {
  }

  static on_fixed_update(dt) {}
  
  static on_render() {
    __ui_root.render()
  }
  
  static on_destroy() {
  }
}

