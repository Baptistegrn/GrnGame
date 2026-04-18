import "std/wren/input/key_code" for KeyCode
import "std/wren/input/pad_button" for PadButton
import "std/wren/dev/log" for Log
import "std/wren/input/input_event" for InputEvent

class PadNode{
    //bind cpp
    foreign static pressed(input, index)
    foreign static just_pressed(input, index)
    foreign static first_pressed_index_for_button(input)
    foreign static sticks(index)
    foreign static triggers(index)
    foreign static controller_open(index)
    foreign static connected_count()

    static next_index {
        if (__next_index == null) __next_index = 0
        var current = __next_index
        __next_index = __next_index + 1
        return current
    }

    construct new() { init_(PadNode.next_index, 0, true) }
    construct new(button) { init_(-1, button, false) }

    init_(index, button, has_pad) {
        _index = index
        _button = button
        _has_pad = has_pad

        _stick_lx = 0.0
        _stick_ly = 0.0
        _stick_rx = 0.0
        _stick_ry = 0.0
        _trigger_l = 0.0
        _trigger_r = 0.0

        _just_pressed_events = {}
        _pressed_events = {}
        _just_pressed_buttons = []
        _pressed_buttons = []
        _aliases = {}

        if (_has_pad) {
            Log.log_debug("PadNode initialized with auto-increment index: %(index)")
        } else {
            Log.log_debug("PadNode created, waiting for button detection: %(button)")
        }
    }

    index { _index }
    index=(v) { _index = v }
    
    stick_lx { _stick_lx }
    stick_ly { _stick_ly }
    stick_rx { _stick_rx }
    stick_ry { _stick_ry }
    trigger_l { _trigger_l }
    trigger_r { _trigger_r }
    has_pad { _has_pad }

    add_alias(name, button) {
        _aliases[name] = button
        Log.log_debug("Alias added: '%(name)' -> button %(button)")
    }

    get_count() { PadNode.connected_count() }

    set_id(button) {
        _index = PadNode.first_pressed_index_for_button(button)
        if (_index == -1) {
            _has_pad = false
        } else {
            _has_pad = true
            Log.log_info("PadNode acquired controller at index %(_index)")
        }
    }

    set_detect_button(button) { _button = button }

    add_callback(alias, input_type, callback) {
        if (!_aliases.containsKey(alias)) return false
        add_callback_raw(_aliases[alias], input_type, callback)
        return true
    }

    add_callback_raw(button, input_type, callback) {
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
        }
    }

    update(dt) {
        if (!_has_pad) {
            set_id(_button)
            return
        }

        for (btn in _just_pressed_buttons) {
            if (PadNode.just_pressed(btn, _index)) {
                for (cb in _just_pressed_events[btn]) cb.call()
            }
        }

        for (btn in _pressed_buttons) {
            if (PadNode.pressed(btn, _index)) {
                for (cb in _pressed_events[btn]) cb.call()
            }
        }

        var st = PadNode.sticks(_index)
        if (st != null && st.count >= 4) {
            _stick_lx = st[0]
            _stick_ly = st[1]
            _stick_rx = st[2]
            _stick_ry = st[3]
        }

        var tr = PadNode.triggers(_index)
        if (tr != null && tr.count >= 2) {
            _trigger_l = tr[0]
            _trigger_r = tr[1]
        }
    }
}
