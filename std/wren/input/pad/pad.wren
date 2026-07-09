import "std/wren/input/pad/pad_button" for PadButton
import "std/wren/input/pad/pad_axis" for PadAxis
import "std/wren/dev/log" for Log
import "std/wren/input/input_event" for InputEvent
import "std/wren/input/pad/pad_const" for PadConst
import "std/wren/core/guard" for Guard

class Pad {
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
    foreign static sticks_triggers(index)
    foreign static controller_open(index)
    foreign static connected_count()

    construct new() {
        if (__next_index == null) {
            __next_index = 0
        }
        _index = __next_index
        __next_index = __next_index + 1
        _button_init = 0
  
        _has_pad = true
        _values = [0.0, 0.0, 0.0, 0.0, 0.0, 0.0]

        _just_pressed_events = {}
        _just_released_events = {}
        _pressed_events = {}

        _just_pressed_keys = []
        _just_released_keys = []
        _pressed_keys = []
        
        _button_aliases = {}
        _axis_aliases = {}
        _axis_events = {}
        _active_axis_keys = [] 

        Log.debug("Pad initialized with auto index: %(_index)")
    }

    construct new(button) {
        _index = -1
        _button_init = 0

        // Guard primary type, then manually check kind
        if (Guard.isType(button, PadConst, "Pad.new")) {
            //todo : add bind pad with AXIS 
            if (button.kind == PadConst.BUTTON) {
                _button_init = button.value
            } else {
                Log.warn("Pad.new: parameter must be a BUTTON kind")
            }
        }
        
        _has_pad = false
        _values = [0.0, 0.0, 0.0, 0.0, 0.0, 0.0]

        _just_pressed_events = {}
        _just_released_events = {}
        _pressed_events = {}

        _just_pressed_keys = []
        _just_released_keys = []
        _pressed_keys = []
        
        _button_aliases = {}
        _axis_aliases = {}
        _axis_events = {}
        _active_axis_keys = [] 

        Log.debug("Pad created, waiting for button: %(button)")
    }

    index { _index }
    
    index=(v) { 
        if (!Guard.isType(v, Num, "Pad.index=")) return
        _index = v 
    }
    
    stick_lx { _values[PadAxis.STICK_LX.value] }
    stick_ly { _values[PadAxis.STICK_LY.value] }
    stick_rx { _values[PadAxis.STICK_RX.value] }
    stick_ry { _values[PadAxis.STICK_RY.value] }
    trigger_l { _values[PadAxis.TRIGGER_L.value] }
    trigger_r { _values[PadAxis.TRIGGER_R.value] }

    get_count() { Pad.connected_count() }
    
    set_detect_button(button) {         
        if (!Guard.isType(button, PadConst, "Pad.set_detect_button")) return
        
        if (button.kind == PadConst.BUTTON) {
            _button_init = button.value
        } else {
            Log.warn("Pad.set_detect_button: button must be a BUTTON kind")
        }
    }

    set_id(button) {
        if (!Guard.isType(button, Num, "Pad.set_id")) return

        _index = Pad.first_pressed_index_for_button(button)
        if (_index == -1) {
            _has_pad = false
        } else {
            _has_pad = true
            Log.info("Pad acquired controller at index %(_index)")
        }
    }

    bind(name, input) {
        if (!Guard.isType(name, String, "Pad.bind")) return
        if (!Guard.isType(input, PadConst, "Pad.bind")) return

        if (input.kind == PadConst.BUTTON) {
            _button_aliases[name] = input.value
        } else if (input.kind == PadConst.AXIS) {
            _axis_aliases[name] = input.value
        } else {
            Log.warn("bind: input must be BUTTON or AXIS, name: %(name)")
        }
    }

    resolve_button_(id) {
        if (id is String) {
            if (!_button_aliases.containsKey(id)) {
                Log.warn("Button alias not found: %(id)")
                return null
            }
            return _button_aliases[id]
        }
        
        // Manual fallback check if not a string alias
        if (!(id is Num)) {
            Log.warn("resolve_button_: id must be a String alias or Num ID")
            return null
        }
        
        return id
    }

    resolve_axis_(id) {
        if (id is String) {
            if (!_axis_aliases.containsKey(id)) {
                Log.warn("Axis alias not found: %(id)")
                return null
            }
            return _axis_aliases[id]
        }

        // Manual fallback check
        if (!(id is Num)) {
            Log.warn("resolve_axis_: id must be a String alias or Num ID")
            return null
        }

        return id
    }

    on(alias, event, callback) {
        if (!Guard.isType(alias, String, "Pad.on")) return false
        if (!Guard.isType(callback, Fn, "Pad.on")) return false

        if (!_button_aliases.containsKey(alias)) {
            Log.warn("Button alias not found: %(alias)")
            return false
        }

        var events
        var keys

        // Route to appropriate event dictionary
        if (event == InputEvent.just_pressed) {
            events = _just_pressed_events
            keys = _just_pressed_keys
        } else if (event == InputEvent.pressed) {
            events = _pressed_events
            keys = _pressed_keys
        } else if (event == InputEvent.just_released) {
            events = _just_released_events
            keys = _just_released_keys
        } else {
            Log.warn("Invalid event type for button callback. alias: %(alias)")
            return false
        }

        if (!events.containsKey(alias)) {
            events[alias] = []
            keys.add(alias)
        }

        events[alias].add(callback)
        return true
    }

    on(alias, event, threshold, callback) {
        if (!Guard.isType(alias, String, "Pad.on (axis)")) return false
        if (!Guard.isType(threshold, Num, "Pad.on (axis)")) return false
        if (!Guard.isType(callback, Fn, "Pad.on (axis)")) return false

        if (event != InputEvent.changed) {
            Log.warn("on: Only InputEvent.changed is supported for axis")
            return false
        }
        if (!_axis_aliases.containsKey(alias)) {
            Log.warn("Axis alias not found: %(alias)")
            return false
        }

        if (!_axis_events.containsKey(alias)) {
            _axis_events[alias] = []
            _active_axis_keys.add(alias)
        }

        _axis_events[alias].add([threshold, callback, 0.0])
        return true
    }

    rebind(name, input) {
        if (!Guard.isType(name, String, "Pad.rebind")) return false
        if (!Guard.isType(input, PadConst, "Pad.rebind")) return false

        if (input.kind == PadConst.BUTTON) {
            if (_button_aliases.containsKey(name)) {
                _button_aliases[name] = input.value
                return true
            }
            Log.warn("Button alias not found: %(name)")
        } else if (input.kind == PadConst.AXIS) {
            if (_axis_aliases.containsKey(name)) {
                _axis_aliases[name] = input.value
                return true
            }
            Log.warn("Axis alias not found: %(name)")
        } else {
            Log.warn("rebind: input must be BUTTON or AXIS, name: %(name)")
        }

        return false
    }

    update(dt) {
        if (!Guard.isType(dt, Num, "Pad.update")) return

        if (!_has_pad) {
            set_id(_button_init)
            return
        }

        // Process button states
        for (key in _just_pressed_keys) {
            var button = resolve_button_(key)
            if (button != null && Pad.just_pressed(button, _index)) {
                for (cb in _just_pressed_events[key]) {
                    cb.call()
                }
            }
        }

        for (key in _pressed_keys) {
            var button = resolve_button_(key)
            if (button != null && Pad.pressed(button, _index)) {
                for (cb in _pressed_events[key]) {
                    cb.call()
                }
            }
        }

        for (key in _just_released_keys) {
            var button = resolve_button_(key)
            if (button != null && Pad.just_released(button, _index)) {
                for (cb in _just_released_events[key]) {
                    cb.call()
                }
            }
        }

        // Update axes values
        _values[PadAxis.STICK_LX.value] = Pad.stick_lx(_index)
        _values[PadAxis.STICK_LY.value] = Pad.stick_ly(_index)
        _values[PadAxis.STICK_RX.value] = Pad.stick_rx(_index)
        _values[PadAxis.STICK_RY.value] = Pad.stick_ry(_index)
        _values[PadAxis.TRIGGER_L.value] = Pad.trigger_l(_index)
        _values[PadAxis.TRIGGER_R.value] = Pad.trigger_r(_index)

        // Trigger axis callbacks
        for (key in _active_axis_keys) {
            var axis = resolve_axis_(key)
            if (axis == null) continue

            var value = _values[axis]

            for (event in _axis_events[key]) {
                var threshold = event[0]
                var callback = event[1]
                var lastValue = event[2]

                if ((value - lastValue).abs >= threshold) {
                    callback.call(value)
                    event[2] = value
                }
            }
        }
    }
    //todo : make hight level
    rumble(left_rumble, right_rumble, time) {
        if (!Guard.isType(left_rumble, Num, "Pad.rumble")) return
        if (!Guard.isType(right_rumble, Num, "Pad.rumble")) return
        if (!Guard.isType(time, Num, "Pad.rumble")) return
        
        Pad.pad_rumble(_index, left_rumble, right_rumble, time)
    }

    list_bindings() {
        //todo replace id by button / axis name 
        Log.info("Pad Bindings (Index: %(_index))")
        Log.info("Button Aliases:")
        for (key in _button_aliases.keys) {
            Log.info("  - '%(key)' maps to ID: %(_button_aliases[key])")
        }
        Log.info("Axis Aliases:")
        for (key in _axis_aliases.keys) {
            Log.info("  - '%(key)' maps to ID: %(_axis_aliases[key])")
        }
    }

    list_callbacks() {
        Log.info("Pad Callbacks (Index: %(_index))")
        Log.info("Just Pressed:")
        for (key in _just_pressed_keys) {
            Log.info("  - '%(key)': %(_just_pressed_events[key].count) bound function(s)")
        }
        Log.info("Pressed:")
        for (key in _pressed_keys) {
            Log.info("  - '%(key)': %(_pressed_events[key].count) bound function(s)")
        }
        Log.info("Just Released:")
        for (key in _just_released_keys) {
            Log.info("  - '%(key)': %(_just_released_events[key].count) bound function(s)")
        }
        Log.info("Axis Changed:")
        for (key in _active_axis_keys) {
            Log.info("  - '%(key)': %(_axis_events[key].count) bound configuration(s)")
        }
    }

    debug() {
        list_bindings()
        list_callbacks()
    }
}