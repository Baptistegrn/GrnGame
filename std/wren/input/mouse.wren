import "std/wren/math/vec2" for Vec2
import "std/wren/input/input_event" for InputEvent
import "std/wren/dev/log" for Log
import "std/wren/input/mouse_button" for MouseButton

class Mouse {
    foreign static left_pressed()
    foreign static left_just_pressed()
    foreign static left_just_released()
    foreign static right_pressed()
    foreign static right_just_pressed()
    foreign static right_just_released()
    foreign static x()
    foreign static y()
    foreign static scroll_x()
    foreign static scroll_y()
    foreign static move_mouse(x, y)
    foreign static show_cursor()
    foreign static hide_cursor()

    construct new(){
        _position = Vec2.new()
        _scroll = Vec2.new()

        _just_pressed_events = {}
        _just_released_events = {}
        _pressed_events = {}

        _just_pressed_buttons = []
        _just_released_buttons = []
        _pressed_buttons = []
    }

    add_callback(button, input_type, callback) {
        if (input_type == InputEvent.just_pressed) {
            if (!_just_pressed_events.containsKey(button)) {
                _just_pressed_events[button] = []
                _just_pressed_buttons.add(button)
            }
            _just_pressed_events[button].add(callback)
        } else if (input_type == InputEvent.pressed) {
            if (!_pressed_events.containsKey(button)) {
                _pressed_events[button] = []
                _pressed_buttons.add(button)
            }
            _pressed_events[button].add(callback)
        } else if (input_type == InputEvent.just_released) {
            if (!_just_released_events.containsKey(button)) {
                _just_released_events[button] = []
                _just_released_buttons.add(button)
            }
            _just_released_events[button].add(callback)
        }
    }

    update(dt) {
        for (btn in _just_pressed_buttons) {
            if ((btn == MouseButton.LEFT && Mouse.left_just_pressed()) || 
                (btn == MouseButton.RIGHT && Mouse.right_just_pressed())) {
                for (cb in _just_pressed_events[btn]) cb.call()
            }
        }

        for (btn in _just_released_buttons) {
            if ((btn == MouseButton.LEFT && Mouse.left_just_released()) || 
                (btn == MouseButton.RIGHT && Mouse.right_just_released())) {
                for (cb in _just_released_events[btn]) cb.call()
            }
        }

        for (btn in _pressed_buttons) {
            if ((btn == MouseButton.LEFT && Mouse.left_pressed()) || 
                (btn == MouseButton.RIGHT && Mouse.right_pressed())) {
                for (cb in _pressed_events[btn]) cb.call()
            }
        }

        _position.x = Mouse.x()
        _position.y = Mouse.y()
        _scroll.x = Mouse.scroll_x()
        _scroll.y = Mouse.scroll_y()
    }

    position { _position }
    scroll { _scroll }
}
