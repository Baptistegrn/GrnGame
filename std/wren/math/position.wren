class Position{
    construct new(x,y){
        _x = x
        _y = y
    }
    construct new(){
        _x = 0.0
        _y = 0.0
    }
    x { _x }
    y { _y }

    x=(v) { _x = v }
    y=(v) { _y = v }

    +(other) {
        return Position.new(_x + other.x, _y + other.y)
    }

    toString { "Position(%(x), %(y))" }
}
