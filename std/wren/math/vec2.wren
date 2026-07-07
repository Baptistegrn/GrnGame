class Vec2 {
    construct new(x,y){
        _x =x
        _y =y
    }
    construct new(){
        _x = Num.nan
        _y = Num.nan
    }
    
    x { _x }
    y { _y }

    x=(v) { _x = v }
    y=(v) { _y = v }

    +(other) {
    if (other is Num) {
        return Vec2.new(_x+other,_y+other)
    }
        return Vec2.new(_x + other.x, _y + other.y)
    }

    *(other){
        if (other is Num) {
            return Vec2.new(_x * other, _y * other)
        }
        return Vec2.new(_x * other.x, _y * other.y)
    }
    
    -(other){
        if (other is Num) {
        return Vec2.new(_x-other,_y-other)
    }
        return Vec2.new(_x - other.x, _y - other.y)
    }
    
    /(other){
        if (other is Num) {
            return Vec2.new(_x / other, _y / other)
        }
        return Vec2.new(_x / other.x, _y / other.y)
    }

    toString { "%(x),%(y)" }
}