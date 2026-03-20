options gen2

require node
require grngame_input

enum KeyCode {
    KEY_A = 4,
    KEY_B = 5,
    KEY_C = 6,
    KEY_D = 7,
    KEY_E = 8,
    KEY_F = 9,
    KEY_G = 10,
    KEY_H = 11,
    KEY_I = 12,
    KEY_J = 13,
    KEY_K = 14,
    KEY_L = 15,
    KEY_M = 16,
    KEY_N = 17,
    KEY_O = 18,
    KEY_P = 19,
    KEY_Q = 20,
    KEY_R = 21,
    KEY_S = 22,
    KEY_T = 23,
    KEY_U = 24,
    KEY_V = 25,
    KEY_W = 26,
    KEY_X = 27,
    KEY_Y = 28,
    KEY_Z = 29,

    KEY_0 = 39,
    KEY_1 = 30,
    KEY_2 = 31,
    KEY_3 = 32,
    KEY_4 = 33,
    KEY_5 = 34,
    KEY_6 = 35,
    KEY_7 = 36,
    KEY_8 = 37,
    KEY_9 = 38,

    KEY_F1 = 58,
    KEY_F2 = 59,
    KEY_F3 = 60,
    KEY_F4 = 61,
    KEY_F5 = 62,
    KEY_F6 = 63,
    KEY_F7 = 64,
    KEY_F8 = 65,
    KEY_F9 = 66,
    KEY_F10 = 67,
    KEY_F11 = 68,
    KEY_F12 = 69,

    KEY_RETURN = 40,
    KEY_ESCAPE = 41,
    KEY_BACKSPACE = 42,
    KEY_TAB = 43,
    KEY_SPACE = 44,
    KEY_DELETE = 76,
    KEY_INSERT = 73,
    KEY_HOME = 74,
    KEY_END = 77,
    KEY_PAGEUP = 75,
    KEY_PAGEDOWN = 78,

    KEY_UP = 82,
    KEY_DOWN = 81,
    KEY_LEFT = 80,
    KEY_RIGHT = 79,

    KEY_LSHIFT = 225,
    KEY_RSHIFT = 229,
    KEY_LCTRL = 224,
    KEY_RCTRL = 228,
    KEY_LALT = 226,
    KEY_RALT = 230,
    KEY_LGUI = 227,
    KEY_RGUI = 231,
    KEY_CAPS = 57,

    KEY_KP0 = 98,
    KEY_KP1 = 89,
    KEY_KP2 = 90,
    KEY_KP3 = 91,
    KEY_KP4 = 92,
    KEY_KP5 = 93,
    KEY_KP6 = 94,
    KEY_KP7 = 95,
    KEY_KP8 = 96,
    KEY_KP9 = 97,
    KEY_KP_ENTER = 88,
    KEY_KP_PLUS = 87,
    KEY_KP_MINUS = 86,
    KEY_KP_MUL = 85,
    KEY_KP_DIV = 84,
    KEY_KP_DOT = 99,

    KEY_PRINTSCREEN = 70,
    KEY_SCROLLLOCK = 71,
    KEY_PAUSE = 72,
    KEY_MENU = 118
}

enum PadButton {
    PAD_SOUTH = 0,
    PAD_EAST = 1,
    PAD_WEST = 2,
    PAD_NORTH = 3,

    PAD_LB = 9,
    PAD_RB = 10,

    PAD_LS = 7,
    PAD_RS = 8,

    PAD_START = 6,
    PAD_BACK = 4,
    PAD_GUIDE = 5,

    PAD_DPAD_UP = 11,
    PAD_DPAD_DOWN = 12,
    PAD_DPAD_LEFT = 13,
    PAD_DPAD_RIGHT = 14,

    PAD_MISC1 = 15,
    PAD_MISC2 = 21,
    PAD_MISC3 = 22,
    PAD_MISC4 = 23,
    PAD_MISC5 = 24,
    PAD_MISC6 = 25,

    PAD_PADDLE1 = 16,
    PAD_PADDLE2 = 17,
    PAD_PADDLE3 = 18,
    PAD_PADDLE4 = 19,

    PAD_TOUCHPAD = 20,

    PAD_BUTTON_COUNT = 26
}

enum input_event {
    just_pressed,
    pressed
}

class InputNode : Node {
    aliases: table<string, KeyCode>
    just_pressed_events : table<KeyCode, array<lambda>>
    pressed_events : table<KeyCode, array<lambda>>

    def InputNode() {
        Node`Node(self, "InputNode")
        aliases = table<string, KeyCode>()
        just_pressed_events = table<KeyCode, array<lambda>>()
        pressed_events = table<KeyCode, array<lambda>>()
    }

    def add_alias(name: string, key: KeyCode) : void {
        aliases.insert(name, key)
    }

    def add_callback(alias: string, input_type: input_event, var callback: lambda) : bool {
        if (!aliases.key_exists(alias)) {
            return false
        }

        let aliased_key = aliases |> get_value(alias)
        add_callback_raw(aliased_key, input_type, callback)
        return true
    }

    def add_callback_raw(key: KeyCode, input_type: input_event, var callback: lambda) : void {
        if (input_type == input_event.just_pressed) {
            if (!just_pressed_events.key_exists(key)) {
                just_pressed_events[key] <- array<lambda>()
            }
            just_pressed_events[key].emplace(callback)
        }
        elif (input_type == input_event.pressed) {
            if (!pressed_events.key_exists(key)) {
                pressed_events[key] <- array<lambda>()
            }
            pressed_events[key].emplace(callback)
        }
    }


    def override update(dt: float) {
        super.update(dt)

        for (key, callbacks in keys(just_pressed_events), values(just_pressed_events)) {
            if (key_just_pressed(int(key))) {
                for (cb in callbacks) {
                    invoke(cb)
                }
            }
        }

        for (key, callbacks in keys(pressed_events), values(pressed_events)) {
            if (key_pressed(int(key))) {
                for (cb in callbacks) {
                    invoke(cb)
                }
            }
        }
    }
}