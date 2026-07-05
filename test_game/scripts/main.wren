// Gamepad example

import "std/wren/input/pad/pad" for Pad
import "std/wren/input/pad/pad_button" for PadButton
import "std/wren/input/pad/pad_axis" for PadAxis
import "std/wren/input/input_event" for InputEvent
import "std/wren/dev/log" for Log

class Main {
    static on_start() {
        // First pad initialized with auto-incremented index (index 0)
        __pad = Pad.new() 
        
        // Second pad will wait and acquire the controller only when 'X' is pressed
        __second_pad = Pad.new(PadButton.X) 

        // Uncomment these lines to test the Guard system in the console

        /* 
        
        // Error: Constructor expects a PadConst.BUTTON, given a String
        __error_pad = Pad.new("Not a button") 
        
        // Error: bind expects a PadConst, given a Num
        __pad.bind("error_alias", 8) 
        
        // Error: callback expects a Fn, given a String
        __pad.on("jump", InputEvent.pressed, "not_a_function") 
        
        // Error: axis threshold expects a Num, given a String
        __pad.on("move_horizontal", InputEvent.changed, "0.15", Fn.new { }) 
        
        // Error: rumble expects numbers, given a String for left_rumble
        __pad.rumble("high", 0, 2000) 
        
        */

        // you can set a lot of buttons for the same action 
        __second_pad.bind("test_button", PadButton.X)
        __second_pad.on("test_button", InputEvent.pressed, Fn.new {
            System.print("Second pad test button pressed")
        })

        // Button aliases for player 1
        __pad.bind("jump", PadButton.A)
        __pad.bind("trigger_rumble", PadButton.Y)
        __pad.bind("switch_jump", PadButton.X)

        // Axis aliases for player 1
        __pad.bind("move_horizontal", PadAxis.STICK_LX)
        __pad.bind("move_vertical", PadAxis.STICK_LY)
        __pad.bind("aim", PadAxis.TRIGGER_L)
        __pad.bind("shoot", PadAxis.TRIGGER_R)


        __pad.on("jump", InputEvent.pressed, Fn.new {
            System.print("action: jump!")
        })

        //you can have more than 1 callback 
        /* 
        __pad.on("jump", InputEvent.pressed, Fn.new {
            System.print("action: jumpbis!")
        })
        */

        __pad.on("trigger_rumble", InputEvent.just_pressed, Fn.new {
            System.print("action: rumble!")
            // Left rumble, right rumble, duration in ms
            __pad.rumble(100, 0, 2000) 
        })

        __pad.on("switch_jump", InputEvent.just_pressed, Fn.new {
            System.print("Jump remapped to B")
            // Test rebind feature
            __pad.rebind("jump", PadButton.B) 
        })


        __pad.on("move_horizontal", InputEvent.changed, 0.15, Fn.new { |value|
            System.print("moving horizontal: %(value)")
        })

        __pad.on("move_vertical", InputEvent.changed, 0.15, Fn.new { |value|
            System.print("moving vertical: %(value)")
        })

        __pad.on("aim", InputEvent.changed, 0.1, Fn.new { |value|
            System.print("aim pressure: %(value)")
        })

        __pad.on("shoot", InputEvent.changed, 0.1, Fn.new { |value|
            System.print("shoot pressure: %(value)")
        })


        // Get the number of currently connected controllers
        Log.info("Controllers connected: %(__pad.get_count())")

        // Change the detection button for the second pad
        __second_pad.set_detect_button(PadButton.A)

        // Disable persistence on deconnect for pad with serial number
        // __pad.disable_persistence()

        // debug pad 

        __pad.debug()
    }

    static on_update(dt) {
        // Essential to process callbacks and state changes
        __pad.update(dt)
        __second_pad.update(dt)
        
        // You can read sticks or triggers directly without callbacks
        //System.print("Raw stick X: %(__pad.stick_lx)")
        
    }

    static on_fixed_update(dt) {}
    static on_render() {}
    static on_destroy() {}
}