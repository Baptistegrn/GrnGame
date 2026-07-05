

class PadConst {
    static BUTTON { 0 }
    static AXIS { 1 }

    construct new(kind, value) {
        _kind = kind
        _value = value
    }

    kind { _kind }
    value { _value }
}