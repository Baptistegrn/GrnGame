import "std/wren/ui/ui_node" for UiNode
import "std/wren/ui/ui_info" for UiInfo
import "std/wren/ui/centering" for Centering
import "std/wren/math/vec2" for Vec2
import "std/wren/renderer/alpha" for Alpha
import "std/wren/input/pad_node" for PadNode
import "std/wren/input/pad_button" for PadButton
import "std/wren/dev/log" for Log
import "std/wren/audio/sound" for Sound
import "std/wren/audio/sound_info" for SoundInfo
import "std/wren/input/mouse" for Mouse
import "std/wren/input/mouse_button" for MouseButton
import "std/wren/input/input_event" for InputEvent

class Main {
  static on_start() {
    __button = UiNode.new("button",UiInfo.new("1"))
    
    // Test Mouse callbacks
    __mouse = Mouse.new()
    
    Log.log_info("=== Testing Mouse Callbacks ===")
    
    // Left button callbacks
    __mouse.add_callback(MouseButton.LEFT, InputEvent.just_pressed, Fn.new {
      Log.log_info("Left click PRESSED at x=%(Mouse.x()), y=%(Mouse.y())")
    })
    
    __mouse.add_callback(MouseButton.LEFT, InputEvent.pressed, Fn.new {
      Log.log_debug("Left click HELD")
    })
    
    __mouse.add_callback(MouseButton.LEFT, InputEvent.just_released, Fn.new {
      Log.log_info("Left click RELEASED")
    })
    
    // Right button callbacks
    __mouse.add_callback(MouseButton.RIGHT, InputEvent.just_pressed, Fn.new {
      Log.log_info("Right click PRESSED at x=%(Mouse.x()), y=%(Mouse.y())")
    })
    
    __mouse.add_callback(MouseButton.RIGHT, InputEvent.just_released, Fn.new {
      Log.log_info("Right click RELEASED")
    })
    
    Log.log_info("Mouse callbacks registered. Try clicking!")
  }

  static on_update(dt) {
    __mouse.update(dt)
    //System.print(__mouse.scroll)
  }

  static on_fixed_update(dt) {}
  
  static on_render() {
    __button.render()
  }
  
  static on_destroy() {
  }
}
