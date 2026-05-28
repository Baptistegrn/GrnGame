
foreign class Texture {
    foreign static draw_inner(name, x, y, scale, rotation, alpha_idx)
    foreign static width(name)
    foreign static height(name)
    // note : alpha is note the color but the index 255 -> max index possible -> max opacity 
    static draw(name, x, y) {
        return draw_inner(name, x, y, 1, 0, 255)
    }
    static draw(name, x, y, scale) {
        return draw_inner(name, x, y, scale, 0, 255)
    }
    static draw(name, x, y, scale, rotation) {
        return draw_inner(name, x, y, scale, rotation, 255)
    }
    static draw(name, x, y, scale, rotation, alpha_idx) {
        return draw_inner(name, x, y, scale, rotation, alpha_idx)
    }
}