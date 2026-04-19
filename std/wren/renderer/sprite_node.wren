import "std/wren/core/node" for Node
import "std/wren/renderer/sprite" for Sprite
import "std/wren/dev/log" for Log

class SpriteNode is Node {
    construct new(name, sprite_name, w, h, flip, alpha, rotation, coeff) {
        super(name)
        _spr = Sprite.new(sprite_name, w, h)
        _frame = 0
        _flip = flip
        _alpha = alpha
        _rotation = rotation
        _coeff = coeff
        Log.log_info("SpriteNode '%(name)' created with sprite '%(sprite_name)'")
    }

    construct new(name, sprite_name, w, h) {
        super(name)
        _spr = Sprite.new(sprite_name, w, h)
        _frame = 0
        _flip = false
        _alpha = 255
        _rotation = 0
        _coeff = 1
        Log.log_info("SpriteNode '%(name)' created with sprite '%(sprite_name)'")
    }

    spr { _spr }
    frame { _frame }
    frame=(v) { _frame = v }
    flip { _flip }
    flip=(v) { _flip = v }
    alpha { _alpha }
    alpha=(v) { _alpha = v }
    rotation { _rotation }
    rotation=(v) { _rotation = v }
    coeff { _coeff }
    coeff=(v) { _coeff = v }

    render() {
        super.render()
        var wp = get_world_position()
        _spr.draw(_frame, wp.x, wp.y, _coeff, _flip, _rotation, _alpha)
    }
}
