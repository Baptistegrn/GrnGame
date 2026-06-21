// Gamepad example

// Standard library imports
import "std/wren/input/pad/pad" for Pad
import "std/wren/input/pad/pad_button" for PadButton
import "std/wren/input/pad/pad_axis" for PadAxis
import "std/wren/input/input_event" for InputEvent

class Main {
  static on_start() {
    // Create a gamepad instance.
    // You can also provide a button to use for controller detection/binding.
    __pad = Pad.new()

    // This pad is automatically assigned to the first connected controller.
    // The second Pad instance will be assigned to the second controller, and so on.

    // --- button aliases setup ---
    __pad.add_alias("jump", PadButton.A)
    __pad.add_alias("trigger_rumble", PadButton.Y)
    
    // Alias used to dynamically remap the jump action.
    __pad.add_alias("switch_jump", PadButton.X)
    
    // --- axis aliases setup (sticks and triggers) ---
    __pad.add_axis_alias("move_horizontal", PadAxis.LEFT_X)
    __pad.add_axis_alias("move_vertical", PadAxis.LEFT_Y)
    
    __pad.add_axis_alias("aim", PadAxis.LEFT_TRIGGER)
    __pad.add_axis_alias("shoot", PadAxis.RIGHT_TRIGGER)

    // --- callbacks setup ---
    
    // Triggered when the jump button is pressed.
    __pad.add_callback("jump", InputEvent.pressed, Fn.new {
      System.print("action: jump! (button pressed)")
    })

    // Triggered when the rumble button is pressed.
    __pad.add_callback("trigger_rumble", InputEvent.just_pressed, Fn.new {
      System.print("action: rumble started! (button Y pressed)")

      // Start controller vibration:
      // left motor = 80%, right motor = 80%, duration = 500 ms
      __pad.rumble(100, 0, 2000)
    })

    // Dynamically remap the jump action.
    __pad.add_callback("switch_jump", InputEvent.pressed, Fn.new {
      System.print("action: jump button has been changed to B!")
      __pad.change_alias("jump", PadButton.B)
    })

    // Horizontal stick movement callback.
    // Triggered when the value changes by at least 0.15.
    __pad.add_axis_callback("move_horizontal", 0.15, Fn.new { |val|
        System.print("moving horizontally: " + val.toString)
    })

    // Vertical stick movement callback.
    // Triggered when the value changes by at least 0.15.
    __pad.add_axis_callback("move_vertical", 0.15, Fn.new { |val|
        System.print("moving vertically: " + val.toString)
    })

    // Left trigger callback.
    // Triggered when the value changes by at least 0.1.
    __pad.add_axis_callback("aim", 0.1, Fn.new { |val|
        System.print("aiming... pressure: " + val.toString)
    })

    // Right trigger callback.
    // Triggered when the value changes by at least 0.1.
    __pad.add_axis_callback("shoot", 0.1, Fn.new { |val|
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