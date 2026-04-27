import "std/wren/math/position" for Position
import "std/wren/ui/centering" for Centering
import "std/wren/renderer/alpha" for Alpha

class UiInfo {
    construct new() { init_("default", 1, Centering.middle, Position.new(), false, Alpha.opaque, 0) }
    construct new(ui_name) { init_(ui_name, 1, Centering.middle, Position.new(), false, Alpha.opaque, 0) }
    construct new(ui_name, coeff) { init_(ui_name, coeff, Centering.middle, Position.new(), false, Alpha.opaque, 0) }
    construct new(ui_name, coeff, center) { init_(ui_name, coeff, center, Position.new(), false, Alpha.opaque, 0) }
    construct new(ui_name, coeff, center, padding) { init_(ui_name, coeff, center, padding, false, Alpha.opaque, 0) }
    construct new(ui_name, coeff, center, padding, flip) { init_(ui_name, coeff, center, padding, flip, Alpha.opaque, 0) }
    construct new(ui_name, coeff, center, padding, flip, alpha) { init_(ui_name, coeff, center, padding, flip, alpha, 0) }
    construct new(ui_name, coeff, center, padding, flip, alpha, rotation) { init_(ui_name, coeff, center, padding, flip, alpha, rotation) }

    init_(ui_name, coeff, center, padding, flip, alpha, rotation) {
        _ui_name = ui_name
        _coeff = coeff
        _center = center
        _padding = padding
        _flip = flip
        _alpha = alpha
        _rotation = rotation
    }

    ui_name { _ui_name }
    ui_name=(v) { _ui_name = v }
    coeff { _coeff }
    coeff=(v) { _coeff = v }
    center { _center }
    center=(v) { _center = v }
    padding { _padding }
    padding=(v) { _padding = v }
    flip { _flip }
    flip=(v) { _flip = v }
    alpha { _alpha }
    alpha=(v) { _alpha = v }
    rotation { _rotation }
    rotation=(v) { _rotation = v }
}