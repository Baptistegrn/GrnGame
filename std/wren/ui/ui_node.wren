import "std/wren/core/node" for Node
import "std/wren/renderer/texture" for Texture
import "std/wren/core/window" for Window
import "std/wren/math/position" for Position
import "std/wren/dev/log" for Log

class Centering {
    static left_up     { 0 }
    static right_up    { 1 }
    static middle      { 2 }
    static middle_down { 3 }
    static middle_up   { 4 }
    static left        { 5 }
    static right       { 6 }
}

class UiNode is Node {
    construct new(name, ui_name, in_coeff, is_center) {
        super(name)
        _w = Texture.width(ui_name) * in_coeff
        _h = Texture.height(ui_name) * in_coeff
        _coeff = in_coeff
        _center = is_center
        _ui_name = ui_name
        
        _padding = Position.new(0, 0)
        _flip = false
        _alpha = 255
        _rotation = 0
        _cached_draw_position = null
        _has_cached = false
        Log.log_info("UiNode '%(name)' created with ui '%(ui_name)'")
    }

    construct new(name, ui_name) {
        super(name)
        _w = Texture.width(ui_name)
        _h = Texture.height(ui_name)
        _coeff = 1
        _center = Centering.middle
        _ui_name = ui_name
        
        _padding = Position.new(0, 0)
        _flip = false
        _alpha = 255
        _rotation = 0
        _cached_draw_position = null
        _has_cached = false
        Log.log_info("UiNode '%(name)' created with ui '%(ui_name)'")
    }

    padding { _padding }
    padding=(v) { _padding = v }
    flip { _flip }
    flip=(v) { _flip = v }
    alpha { _alpha }
    alpha=(v) { _alpha = v }
    rotation { _rotation }
    rotation=(v) { _rotation = v }
    ui_name { _ui_name }

    compute_centered_position() {
        var force_scale = Window.force_universe_scale
        var ox = force_scale ? 0.0 : Window.offset_x
        var oy = force_scale ? 0.0 : Window.offset_y

        var cx = 0.0
        var cy = 0.0
        var cw = 0.0
        var ch = 0.0

        if (parent != null) {
            cx = parent.position.x - ox
            cy = parent.position.y - oy
            cw = parent.w // parent assumed to also be UiNode
            ch = parent.h
        } else {
            cx = -ox
            cy = -oy
            cw = Window.universe_width + (ox * 2.0)
            ch = Window.universe_height + (oy * 2.0)
        }

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
        
        // default right
        return Position.new(right_x, mid_y)
    }

    get_draw_position() {
        if (Window.change || !_has_cached) {
            _cached_draw_position = compute_centered_position()
            position = _cached_draw_position
            _has_cached = true
        }
        return _cached_draw_position
    }

    render() {
        super.render()
        var pos = get_draw_position()
        Texture.draw(_ui_name, pos.x, pos.y, _coeff, _flip, _rotation, _alpha)
    }

    w { _w }
    h { _h }
}
