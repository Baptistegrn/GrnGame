foreign class Texture {
    foreign static draw_inner(name, x, y, scale, rotation, alpha)
    foreign static width(name)
    foreign static height(name)

    static draw(name, x, y) {
        return draw_inner(name, x, y, 1, 0, 255)
    }
    static draw(name, x, y, scale) {
        return draw_inner(name, x, y, scale, 0, 255)
    }
    static draw(name, x, y, scale, rotation) {
        return draw_inner(name, x, y, scale, rotation, 255)
    }
    static draw(name, x, y, scale, rotation, alpha) {
        return draw_inner(name, x, y, scale, rotation, alpha)
    }
}
