import "std/wren/math/vec2" for Vec2
import "std/wren/input/pad/pad" for Pad
import "std/wren/input/pad/pad_button" for PadButton
import "std/wren/input/pad/pad_axis" for PadAxis
import "std/wren/input/input_event" for InputEvent
import "std/wren/dev/log" for Log
import "std/wren/core/event" for Event

class Main {
    static on_start() {
        Event.callback(Event.PadConnect, Fn.new {|index|
            System.println("%(index)")
        })

        Event.callback(Event.PadDisconnect, Fn.new {|index|
            System.println("%(index)")
        })
    }

    static on_update(dt) {}
    static on_fixed_update(dt) {}
    static on_render() {}
    static on_destroy() {}
}