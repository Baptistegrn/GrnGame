class Color {
    construct new(r, g, b) {
        _r = r
        _g = g
        _b = b
    }

    r { _r }
    g { _g }
    b { _b }

    static black   { Color.new(0, 0, 0) }
    static white   { Color.new(255, 255, 255) }
    static red     { Color.new(255, 0, 0) }
    static green   { Color.new(0, 255, 0) }
    static blue    { Color.new(0, 0, 255) }

    static gray        { Color.new(128, 128, 128) }
    static lightGray   { Color.new(192, 192, 192) }
    static darkGray    { Color.new(64, 64, 64) }

    static orange  { Color.new(255, 165, 0) }
    static coral   { Color.new(255, 127, 80) }
    static salmon  { Color.new(250, 128, 114) }
    static gold    { Color.new(255, 215, 0) }

    static cyan    { Color.new(0, 255, 255) }
    static teal    { Color.new(0, 128, 128) }
    static navy    { Color.new(0, 0, 128) }
    static sky     { Color.new(135, 206, 235) }

    static lime    { Color.new(50, 205, 50) }
    static olive   { Color.new(128, 128, 0) }
    static forest  { Color.new(34, 139, 34) }

    static magenta { Color.new(255, 0, 255) }
    static purple  { Color.new(128, 0, 128) }
    static violet  { Color.new(238, 130, 238) }
    static pink    { Color.new(255, 192, 203) }

    static brown   { Color.new(139, 69, 19) }
    static chocolate { Color.new(210, 105, 30) }
    static tan     { Color.new(210, 180, 140) }
}
