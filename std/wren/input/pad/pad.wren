import "std/wren/input/pad/pad_button" for PadButton
import "std/wren/dev/log" for Log
import "std/wren/input/input_event" for InputEvent
import "std/wren/input/pad/pad_axis" for PadAxis

class Pad {
    // c bindings
    foreign static pressed(input, index)
    foreign static just_pressed(input, index)
    foreign static just_released(input,index)

    foreign static pad_rumble(index, left_rumble, right_rumble, time)

    foreign static first_pressed_index_for_button(input)

    foreign static stick_lx(index)
    foreign static stick_ly(index)
    foreign static stick_rx(index)
    foreign static stick_ry(index)
    foreign static trigger_l(index)
    foreign static trigger_r(index)

    foreign static controller_open(index)
    foreign static connected_count()

    // global auto-increment index management
    static next_index {
        if (__next_index == null) __next_index = 0
        var current = __next_index
        __next_index = __next_index + 1
        return current
    }

    construct new() { init_(Pad.next_index, 0, true) }
    construct new(button) { init_(-1, button, false) }

    init_(index, button, has_pad) {
        _index = index
        _button = button
        _has_pad = has_pad

        // axes values
        _stick_lx = 0.0
        _stick_ly = 0.0
        _stick_rx = 0.0
        _stick_ry = 0.0
        _trigger_l = 0.0
        _trigger_r = 0.0

        _just_pressed_events = {}
        _just_released_events = {}
        _pressed_events = {}

        _just_pressed_keys = []
        _just_released_keys = []
        _pressed_keys = []
        
        _aliases = {}

        _axis_aliases = {}
        _axis_events = {}
        _active_axis_keys = [] 

        if (_has_pad) {
            Log.log_debug("pad initialized with auto index: %(index)")
        } else {
            Log.log_debug("pad created, waiting for button: %(button)")
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

    get_count() { Pad.connected_count() }

    set_id(button) {
        _index = Pad.first_pressed_index_for_button(button)
        if (_index == -1) {
            _has_pad = false
        } else {
            _has_pad = true
            Log.log_info("pad acquired controller at index %(_index)")
        }
    }

    set_detect_button(button) { _button = button }
    
    add_alias(name, button) {
        _aliases[name] = button
        Log.log_debug("button alias added: '%(name)' -> button %(button)")
    }

    change_alias(name, new_button) {
        if (_aliases.containsKey(name)) {
            _aliases[name] = new_button
            Log.log_debug("button alias changed: '%(name)' -> new button %(new_button)")
        }
    }

    add_callback(alias, input_type, callback) {
        if (!_aliases.containsKey(alias)) return false
        add_callback_raw(alias, input_type, callback)
        return true
    }

    add_callback_raw(identifier, input_type, callback) {
        var events
        var keys
        
        if (input_type == InputEvent.just_pressed) {
            events = _just_pressed_events
            keys = _just_pressed_keys
        } else if (input_type == InputEvent.pressed) {
            events = _pressed_events
            keys = _pressed_keys
        } else if (input_type == InputEvent.just_released) {
            events = _just_released_events
            keys = _just_released_keys
        } else {
            return
        }

        if (!events.containsKey(identifier)) {
            events[identifier] = []
            keys.add(identifier)
        }
        events[identifier].add(callback)
    }

    
    add_axis_alias(name, axis) {
        _axis_aliases[name] = axis
        Log.log_debug("axis alias added: '%(name)' -> axis %(axis)")
    }

    change_axis_alias(name, new_axis) {
        if (_axis_aliases.containsKey(name)) {
            _axis_aliases[name] = new_axis
            Log.log_debug("axis alias changed: '%(name)' -> new axis %(new_axis)")
        }
    }

    add_axis_callback(alias, threshold, callback) {
        if (!_axis_aliases.containsKey(alias)) return false
        
        if (!_axis_events.containsKey(alias)) {
            _axis_events[alias] = []
            _active_axis_keys.add(alias)
        }
        
        _axis_events[alias].add([threshold, callback, 0.0])
        return true
    }

    resolve_button_(identifier) {
        if (identifier is String) return _aliases[identifier]
        return identifier
    }

    resolve_axis_(identifier) {
        if (identifier is String) return _axis_aliases[identifier]
        return identifier
    }

    update(dt) {
        if (!_has_pad) {
            set_id(_button)
            return
        }

        // execute button callbacks (resolving aliases dynamically)
        for (key in _just_pressed_keys) {
            var btn = resolve_button_(key)
            if (btn != null && Pad.just_pressed(btn, _index)) {
                for (cb in _just_pressed_events[key]) cb.call()
            }
        }

        for (key in _just_released_keys) {
            var btn = resolve_button_(key)
            if (btn != null && Pad.just_released(btn, _index)) {
                for (cb in _just_released_events[key]) cb.call()
            }
        }

        for (key in _pressed_keys) {
            var btn = resolve_button_(key)
            if (btn != null && Pad.pressed(btn, _index)) {
                for (cb in _pressed_events[key]) cb.call()
            }
        }

        // update axes
        _stick_lx = Pad.stick_lx(_index)
        _stick_ly = Pad.stick_ly(_index)
        _stick_rx = Pad.stick_rx(_index)
        _stick_ry = Pad.stick_ry(_index)
        _trigger_l = Pad.trigger_l(_index)
        _trigger_r = Pad.trigger_r(_index)

        // execute axes callbacks
        for (key in _active_axis_keys) {
            var axis = resolve_axis_(key)
            if (axis == null) continue

            var val = 0.0
            
            if (axis == PadAxis.STICK_LX) {
                val = _stick_lx
            } else if (axis == PadAxis.STICK_LY) {
                val = _stick_ly
            } else if (axis == PadAxis.STICK_RX) {
                val = _stick_rx
            } else if (axis == PadAxis.STICK_RY) {
                val = _stick_ry
            } else if (axis == PadAxis.TRIGGER_L) {
                val = _trigger_l
            } else if (axis == PadAxis.TRIGGER_R) {
                val = _trigger_r
            }
            
            for (item in _axis_events[key]) {
                var threshold = item[0]
                var cb = item[1]
                var last_val = item[2]
                
                if ((val - last_val).abs >= threshold) {
                    cb.call(val) 
                    item[2] = val 
                }
            }
        }
    }

    rumble(left_rumble, right_rumble, time) {
        Pad.pad_rumble(_index, left_rumble, right_rumble, time)
    }
}