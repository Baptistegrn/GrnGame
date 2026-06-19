// standard library imports
import "std/wren/input/pad/pad" for Pad
import "std/wren/input/pad/pad_button" for PadButton
import "std/wren/input/pad/pad_axis" for PadAxis
import "std/wren/input/input_event" for InputEvent

class Main {
  static on_start() {
    // initialize the first controller
    __pad = Pad.new()// you can set a key to connect the controller

    // --- buttons aliases setup ---
    __pad.add_alias("jump", PadButton.A)
    __pad.add_alias("trigger_rumble", PadButton.Y)
    
    // --- axes aliases setup (sticks and triggers) ---
    __pad.add_axis_alias("move_horizontal", PadAxis.LEFT_X)
    __pad.add_axis_alias("move_vertical", PadAxis.LEFT_Y)
    
    __pad.add_axis_alias("aim", PadAxis.LEFT_TRIGGER)
    __pad.add_axis_alias("shoot", PadAxis.RIGHT_TRIGGER)

    // --- callbacks setup ---
    
    // jump action (just pressed)
    __pad.add_callback("jump", InputEvent.just_pressed, Fn.new {
      System.print("action: jump! (button a pressed)")
    })

    // rumble action (just pressed)
    __pad.add_callback("trigger_rumble", InputEvent.just_pressed, Fn.new {
      System.print("action: rumble started! (button y pressed)")
      // trigger rumble: left motor at 80%, right motor at 80%, for 500 milliseconds
      __pad.rumble(80, 80, 500)
    })

    // horizontal stick movement callback
    __pad.add_axis_callback("move_horizontal", Fn.new { |val|
        System.print("moving horizontally: " + val.toString)
    })

    // vertical stick movement callback
    __pad.add_axis_callback("move_vertical", Fn.new { |val|
        System.print("moving vertically: " + val.toString)
    })

    // left trigger callback (aim)
    __pad.add_axis_callback("aim", Fn.new { |val|
        System.print("aiming... pressure: " + val.toString)
    })

    // right trigger callback (shoot)
    __pad.add_axis_callback("shoot", Fn.new { |val|
        System.print("shooting! pressure: " + val.toString)
    })
    
  }

  static on_update(dt) {
      __pad.update(dt)
  }

  static on_fixed_update(dt) {
  }

  static on_render() {
  }

  static on_destroy() {
  }
}