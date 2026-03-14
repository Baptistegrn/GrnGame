require node
require grngame_renderer
require grngame_window

enum centering
    left_up
    right_up
    middle
    middle_down
    middle_up
    left
    right

class UiNode : Node
    w:        int
    h:        int
    center:   centering
    coeff:    int
    padding:  int  = 0
    flip_x:   bool = false
    alpha:    int  = 255
    rotation: int  = 0

    def UiNode(ui_name: string; in_coeff: int; is_center: centering)
        Node`Node(self, ui_name)
        let tex_size = texture_get_size(ui_name)
        self.w      = tex_size.x * in_coeff
        self.h      = tex_size.y * in_coeff
        self.coeff  = in_coeff
        self.center = is_center
    //todo : calculate if window is resizing 
    def get_draw_position() : float2
        let force_scale = window_is_force_universe_scale()
        
        let ox = force_scale ? 0.0f : window_get_offset_x()
        let oy = force_scale ? 0.0f : window_get_offset_y()
        var cx = 0.0f
        var cy = 0.0f
        var cw = 0.0f
        var ch = 0.0f

        if parent != null
            cx = parent.position.x - ox
            cy = parent.position.y - oy
            let parent_ui = unsafe(reinterpret<UiNode?> parent)
            cw = float(parent_ui.w)
            ch = float(parent_ui.h)
        else
            cx = -ox
            cy = -oy
            cw = float(window_get_universe_width())  + (ox * 2.0f)
            ch = float(window_get_universe_height()) + (oy * 2.0f)

        let fw = float(w)
        let fh = float(h)
        let p  = float(padding)

        let mid_x    = cx + (cw - fw) * 0.5f
        let mid_y    = cy + (ch - fh) * 0.5f
        let right_x  = cx + cw - fw - p
        let bottom_y = cy + ch - fh - p
        let left_x   = cx + p
        let top_y    = cy + p

        if center == centering.left_up
            return float2(left_x, top_y)
        elif center == centering.right_up
            return float2(right_x, top_y)
        elif center == centering.middle
            return float2(mid_x, mid_y)
        elif center == centering.middle_down
            return float2(mid_x, bottom_y)
        elif center == centering.middle_up
            return float2(mid_x, top_y)
        elif center == centering.left
            return float2(left_x, mid_y)
        else
            return float2(right_x, mid_y)

    def override render()
        super.render()
        let draw_pos = get_draw_position()
        draw(name, draw_pos.x, draw_pos.y, coeff, flip_x, rotation, alpha)