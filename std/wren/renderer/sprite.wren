foreign class Sprite {
    construct new(name, w, h) {}

    foreign name
    foreign name=(n)
    foreign w
    foreign w=(val)
    foreign h
    foreign h=(val)

    foreign draw_inner(frame, x, y, scale, rotation, alpha)

    draw(frame, x, y) {
        return draw_inner(frame, x, y, 1, 0, 255)
    }
    draw(frame, x, y, scale) {
        return draw_inner(frame, x, y, scale, 0, 255)
    }
    draw(frame, x, y, scale, rotation) {
        return draw_inner(frame, x, y, scale, rotation, 255)
    }
    draw(frame, x, y, scale, rotation, alpha) {
        return draw_inner(frame, x, y, scale, rotation, alpha)
    }
}
