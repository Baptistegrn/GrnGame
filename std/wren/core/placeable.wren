class Placeable {
    construct new(name, x, y) {
        _name = name
        _pos = Vec2.new(x, y)
    }
    
    name { _name }
    pos { _pos }
    
    update(dt) {}
    draw() {}
}