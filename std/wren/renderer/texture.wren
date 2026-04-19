foreign class Texture {
    foreign static draw_inner(name, x, y, scale, flip, rotation, alpha)
    foreign static width(name)
    foreign static height(name)

    static draw(name, x, y) {
        return draw_inner(name, x, y, 1, false, 0, 255)
    }
    static draw(name, x, y, scale) {
        return draw_inner(name, x, y, scale, false, 0, 255)
    }
    static draw(name, x, y, scale, flip) {
        return draw_inner(name, x, y, scale, flip, 0, 255)
    }
    static draw(name, x, y, scale, flip, rotation) {
        return draw_inner(name, x, y, scale, flip, rotation, 255)
    }
    static draw(name, x, y, scale, flip, rotation, alpha) {
        return draw_inner(name, x, y, scale, flip, rotation, alpha)
    }
}
