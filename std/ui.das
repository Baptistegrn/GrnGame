options gen2

require node
require grngame_renderer
require grngame_window

enum centering : int {
    left_up,
    right_up,
    middle,
    middle_down,
    middle_up,
    left,
    right
}

class UiNode : Node {
    w:        int
    h:        int
    center:   centering
    padding:  int
    flip_x:   bool
    alpha:    int
    rotation: int
    coeff:    int
    //todo remove sprite_w and sprite_h 
    def UiNode(ui_name: string, sprite_w: int, sprite_h: int, is_center: centering) {
        Node`Node(self, ui_name)
        self.w        = sprite_w
        self.h        = sprite_h
        self.center   = is_center
        self.padding  = 0
        self.coeff    = 1
        self.flip_x   = false
        self.alpha    = 255
        self.rotation = 0
    }
    //todo factorise
    def get_draw_position() : float2 {
        var cx = 0.0f
        var cy = 0.0f
        var cw = float(window_get_universe_width())
        var ch = float(window_get_universe_height())
        if (parent != null) {
            cx = parent.position.x
            cy = parent.position.y
            //cw = parent.w
            //ch = parent.h
        }
        if (!window_is_force_universe_scale()) {
            let ox = window_get_offset_x()
            let oy = window_get_offset_y()
            if (parent == null) {
                cx -= ox
                cy -= oy
                cw += ox * 2.0f
                ch += oy * 2.0f
            }
        }
        let fw = float(w)
        let fh = float(h)
        let p  = float(padding)

        if (center == centering.left_up) {
            return float2(cx + p, cy + p)
        } elif (center == centering.right_up) {
            return float2(cx + cw - fw - p, cy + p)
        } elif (center == centering.middle) {
            return float2(cx + cw / 2.0f - fw / 2.0f, cy + ch / 2.0f - fh / 2.0f)
        } elif (center == centering.middle_down) {
            return float2(cx + cw / 2.0f - fw / 2.0f, cy + ch - fh - p)
        } elif (center == centering.middle_up) {
            return float2(cx + cw / 2.0f - fw / 2.0f, cy + p)
        } elif (center == centering.left) {
            return float2(cx + p, cy + ch / 2.0f - fh / 2.0f)
        } else {
            return float2(cx + cw - fw - p, cy + ch / 2.0f - fh / 2.0f)
        }
    }

    def override render() {
        super.render()
        //todo factorise
        if (alpha == 0) {
            return
        }
        let draw_pos = get_draw_position()
        draw(name, draw_pos.x, draw_pos.y, coeff, flip_x, rotation, alpha)
    }
}
