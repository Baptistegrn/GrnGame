import "std/wren/core/node" for Node
import "std/wren/renderer/texture" for Texture
import "std/wren/core/window" for Window
import "std/wren/math/position" for Position
import "std/wren/dev/log" for Log
import "std/wren/ui/centering" for Centering
import "std/wren/renderer/alpha" for Alpha
import "std/wren/ui/ui_info" for UiInfo

class UiNode is Node {
    construct new(name, info) {
        super(name)
        _ui_name  = info.ui_name
        _coeff    = info.coeff
        _center   = info.center
        _padding  = info.padding
        _flip     = info.flip
        _alpha    = info.alpha
        _rotation = info.rotation

        _coeff_debt       = 0 //cache coeff 
        _cached_draw_position = null
        _has_cached       = false

        _w = Texture.width(_ui_name)  * _coeff
        _h = Texture.height(_ui_name) * _coeff

        Log.log_info("UiNode '%(name)' created with ui '%(_ui_name)'")
    }

    coeff { _coeff }
    coeff=(v) {
        v = v.ceil
        var delta = v - _coeff
        if (delta == 0) return
        apply_coeff_delta_(delta)
    }

    apply_coeff_delta_(delta) {
        _coeff_debt = _coeff_debt + delta

        var applicable = _coeff_debt.max(-_coeff)   
        var new_coeff = (_coeff + applicable).max(0)
        var consumed  = new_coeff - _coeff
        if (consumed != 0) {
            _coeff_debt = _coeff_debt - consumed
            set_coeff_local_(new_coeff)
            mark_layout_dirty_local_()
        }
        for (child in children) {
            if (child is UiNode) child.apply_coeff_delta_(delta)
        }
    }

    center     { _center }
    center=(v) {
        _center = v
        propagate_(Fn.new {|n| n.mark_layout_dirty_local_() })
    }

    padding     { _padding }
    padding=(v) { _padding = v }

    flip     { _flip }
    flip=(v) { propagate_(Fn.new {|n| n.set_flip_local_(v) }) }

    alpha     { _alpha }
    alpha=(v) { propagate_(Fn.new {|n| n.set_alpha_local_(v) }) }

    rotation     { _rotation }
    rotation=(v) { propagate_(Fn.new {|n| n.set_rotation_local_(v) }) }

    ui_name     { _ui_name }
    ui_name=(v) {
        _ui_name = v
        _w = Texture.width(v)  * _coeff
        _h = Texture.height(v) * _coeff
        propagate_(Fn.new {|n| n.mark_layout_dirty_local_() })
    }

    w { _w }
    h { _h }

    propagate_(callback) {
        apply_to_ui_subtree_(callback)
    }

    apply_to_ui_subtree_(callback) {
        update_children(Fn.new {|node|
            if (node is UiNode) {
                callback.call(node)
            } else {
                Log.log_warning("Ui subtree update skipped non-UiNode '%(node.name)'")
            }
        })
    }

    mark_layout_dirty_local_() {
        _cached_draw_position = null
        _has_cached = false
    }

    set_coeff_local_(v) {
        _coeff = v.max(0)
        _w = Texture.width(_ui_name)  * _coeff
        _h = Texture.height(_ui_name) * _coeff
    }

    set_flip_local_(v)     { _flip = v }
    set_alpha_local_(v)    { _alpha = v }
    set_rotation_local_(v) { _rotation = v }

    get_offset_() {
        var force_scale = Window.force_universe_scale
        var ox = force_scale ? 0.0 : Window.offset_x
        var oy = force_scale ? 0.0 : Window.offset_y
        return Position.new(ox, oy)
    }

    get_target_position_() {
        var pos = get_offset_()
        if (parent != null) {
            return Position.new(parent.position.x - pos.x, parent.position.y - pos.y)
        }
        return Position.new(-pos.x, -pos.y)
    }

    get_target_size_() {
        if (parent != null) return Position.new(parent.w, parent.h)
        var offset = get_offset_()
        return Position.new(
            Window.universe_width  + (offset.x * 2.0),
            Window.universe_height + (offset.y * 2.0)
        )
    }

    compute_centered_position_() {
        var tp = get_target_position_()
        var ts = get_target_size_()
        var cx = tp.x  
        var cy = tp.y
        var cw = ts.x  
        var ch = ts.y
        var fw = _w    
        var fh = _h
        var px = _padding.x  
        var py = _padding.y

        var mid_x    = cx + (cw - fw) * 0.5 + px
        var mid_y    = cy + (ch - fh) * 0.5 + py
        var left_x   = cx + px
        var right_x  = cx + cw - fw - px
        var top_y    = cy + py
        var bottom_y = cy + ch - fh - py

        if (_center == Centering.left_up)     return Position.new(left_x,  top_y)
        if (_center == Centering.right_up)    return Position.new(right_x, top_y)
        if (_center == Centering.middle_down) return Position.new(mid_x,   bottom_y)
        if (_center == Centering.middle_up)   return Position.new(mid_x,   top_y)
        if (_center == Centering.left)        return Position.new(left_x,  mid_y)
        if (_center == Centering.left_down)   return Position.new(left_x,  bottom_y)
        if (_center == Centering.right_down)  return Position.new(right_x, bottom_y)
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