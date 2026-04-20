import "std/wren/core/node" for Node
import "std/wren/renderer/texture" for Texture
import "std/wren/core/window" for Window
import "std/wren/math/position" for Position
import "std/wren/dev/log" for Log
import "std/wren/ui/centering" for Centering
import "std/wren/renderer/alpha" for Alpha

class UiNode is Node {
    construct new(name, ui_name) {
        super(name)
        init_(name, ui_name, 1, Centering.middle, Position.new(), false, Alpha.opaque, 0)
    }

    construct new(name, ui_name, coeff) {
        super(name)
        init_(name, ui_name, coeff, Centering.middle, Position.new(), false, Alpha.opaque, 0)
    }

    construct new(name, ui_name, coeff, center) {
        super(name)
        init_(name, ui_name, coeff, center, Position.new(), false, Alpha.opaque, 0)
    }

    construct new(name, ui_name, coeff, center, padding) {
        super(name)
        init_(name, ui_name, coeff, center, padding, false, Alpha.opaque, 0)
    }

    construct new(name, ui_name, coeff, center, padding, flip) {
        super(name)
        init_(name, ui_name, coeff, center, padding, flip, Alpha.opaque, 0)
    }

    construct new(name, ui_name, coeff, center, padding, flip, alpha) {
        super(name)
        init_(name, ui_name, coeff, center, padding, flip, alpha, 0)
    }

    construct new(name, ui_name, coeff, center, padding, flip, alpha, rotation) {
        super(name)
        init_(name, ui_name, coeff, center, padding, flip, alpha, rotation)
    }

    init_(name, ui_name, coeff, center, padding, flip, alpha, rotation) {
        _w = Texture.width(ui_name) * coeff
        _h = Texture.height(ui_name) * coeff
        _coeff = coeff
        _center = center
        _ui_name = ui_name
        _padding = padding
        _flip = flip
        _alpha = alpha
        _rotation = rotation

        _cached_draw_position = null
        _has_cached = false

        Log.log_info("UiNode '%(name)' created with ui '%(ui_name)'")
    }

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
    ui_name { _ui_name }
    w { _w }
    h { _h }


    get_offset_() {
        var force_scale = Window.force_universe_scale
        var ox = force_scale ? 0.0 : Window.offset_x
        var oy = force_scale ? 0.0 : Window.offset_y
        return Position.new(ox, oy)
    }

    get_target_position_() {
        if (parent != null) {
            var pos = get_offset_()
            return Position.new(parent.position.x - pos.x, parent.position.y - pos.y)
        }

        var offset = get_offset_()
        return Position.new(-offset.x, -offset.y)
    }

    get_target_size_() {
        if (parent != null) {
            return Position.new(parent.w, parent.h)
        }

        var offset = get_offset_()
        return Position.new(
            Window.universe_width + (offset.x * 2.0),
            Window.universe_height + (offset.y * 2.0)
        )
    }

    compute_centered_position_() {
        var target_pos = get_target_position_()
        var target_size = get_target_size_()

        var cx = target_pos.x
        var cy = target_pos.y
        var cw = target_size.x
        var ch = target_size.y

        var fw = _w
        var fh = _h
        var px = _padding.x
        var py = _padding.y

        var mid_x    = cx + (cw - fw) * 0.5 + px
        var mid_y    = cy + (ch - fh) * 0.5 + py
        var right_x  = cx + cw - fw - px
        var bottom_y = cy + ch - fh - py
        var left_x   = cx + px
        var top_y    = cy + py

        if (_center == Centering.left_up) {
            return Position.new(left_x, top_y)
        }
        if (_center == Centering.right_up) {
            return Position.new(right_x, top_y)
        }
        if (_center == Centering.middle) {
            return Position.new(mid_x, mid_y)
        }
        if (_center == Centering.middle_down) {
            return Position.new(mid_x, bottom_y)
        }
        if (_center == Centering.middle_up) {
            return Position.new(mid_x, top_y)
        }
        if (_center == Centering.left) {
            return Position.new(left_x, mid_y)
        }
        if (_center == Centering.left_down) {
            return Position.new(left_x, bottom_y)
        }
        if (_center == Centering.right_down) {
            return Position.new(right_x, bottom_y)
        }
    
        return Position.new(mid_x, mid_y)
    }

    get_draw_position_() {
        if (Window.change || !_has_cached) {
            _cached_draw_position = compute_centered_position_()
            position = _cached_draw_position
            _has_cached = true
        }
        return _cached_draw_position
    }

    render() {
        var pos = get_draw_position_()
        Texture.draw(_ui_name, pos.x, pos.y, _coeff, _flip, _rotation, _alpha)
        super.render()
    }

}
