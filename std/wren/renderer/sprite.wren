foreign class Sprite {
    construct new(name, w, h) {}

    foreign name
    foreign name=(n)
    foreign w
    foreign w=(val)
    foreign h
    foreign h=(val)

    foreign draw_inner(frame, x, y, scale, flip, rotation, alpha)

    draw(frame, x, y) {
        return draw_inner(frame, x, y, 1, false, 0, 255)
    }
    draw(frame, x, y, scale) {
        return draw_inner(frame, x, y, scale, false, 0, 255)
    }
    draw(frame, x, y, scale, flip) {
        return draw_inner(frame, x, y, scale, flip, 0, 255)
    }
    draw(frame, x, y, scale, flip, rotation) {
        return draw_inner(frame, x, y, scale, flip, rotation, 255)
    }
    draw(frame, x, y, scale, flip, rotation, alpha) {
        return draw_inner(frame, x, y, scale, flip, rotation, alpha)
    }
}
