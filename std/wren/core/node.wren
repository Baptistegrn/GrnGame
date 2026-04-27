import "std/wren/math/position" for Position

class Node {
    construct new(name) {
        _name = name
        _parent = null
        _position = Position.new()
        _children = []
    }

    add_child(node) {
        node.parent = this
        _children.add(node)
        return node
    }

    get_world_position() {
        return _parent != null ? _parent.get_world_position() + _position : _position
    }

    get_node(name) {
        for (child in _children) {
            if (child.name == name) return child
        }
        return null
    }

    update_children(callback) {
        callback.call(this)
        for (child in _children) {
            if (child is Node) child.update_children(callback)
        }
    }

    name     { _name }
    name=(v) { _name = v }

    parent     { _parent }
    parent=(v) { _parent = v }

    position     { _position }
    position=(v) { _position = v }

    children { _children }

    start() {
        for(c in _children) c.start()
    }

    update(dt) {
        for(c in _children) c.update(dt)
    }

    render() {
        for(c in _children) c.render()
    }

    fixed_update(dt) {
        for(c in _children) c.fixed_update(dt)
    }

    destroy() {
        for(c in _children) c.destroy()
    }
}