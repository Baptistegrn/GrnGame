class Position{
    construct new(x,y){
        _x = x
        _y = y
    }
    construct new(){
        _x = 0
        _y = 0
    }
    x { _x }
    y { _y }

    x=(v) { _x = v }
    y=(v) { _y = v }

    toString { "Position(%(x), %(y))" }
}

class Log{
    //bind cpp
    foreign static log_error(message)
    foreign static log_info(message)
    foreign static log_warning(message)
    foreign static log_debug(message)
    foreign static log_critical(message)
}