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
    padding: float2
    flip:   bool = false
    alpha:    int  = 255
    rotation: int  = 0
    ui_name: string
    cached_draw_position: float2
    has_cached_draw_position: bool = false

    def UiNode(name: string, ui_name: string; in_coeff: int =1; is_center: centering =centering.middle)
        Node`Node(self, name)
        let tex_size = texture_get_size(ui_name)
        self.w      = tex_size.x * in_coeff
        self.h      = tex_size.y * in_coeff
        self.coeff  = in_coeff
        self.center = is_center
        self.ui_name = ui_name

    def compute_centered_position() : float2
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
        let px = padding.x
        let py = padding.y

        let mid_x    = cx + (cw - fw) * 0.5f
        let mid_y    = cy + (ch - fh) * 0.5f
        let right_x  = cx + cw - fw - px
        let bottom_y = cy + ch - fh - py
        let left_x   = cx + px
        let top_y    = cy + py

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

    def get_draw_position() : float2
        if window_change() || !has_cached_draw_position
            cached_draw_position = compute_centered_position()
            position = cached_draw_position
            has_cached_draw_position = true

        return cached_draw_position

    def override render()
        super.render()
        let draw_pos = get_draw_position()
        draw(ui_name, draw_pos.x, draw_pos.y, coeff, flip, rotation, alpha)