options gen2

require node
require grngame_renderer

class UiNode : Node {
    center: bool
    flip_x: bool
    alpha: int
    rotation: int
    coeff: int

    def UiNode(ui_name: string, w: int, h: int, is_center: bool) {
        Node`Node(self, ui_name)
        self.coeff = 1
        self.center = is_center
        self.flip_x = false
        self.alpha = 255
        self.rotation = 0
    }

    def get_draw_position() : float2 {
        if (center) {
            //todo center position
            return position
        }
        return position
    }

    def override render() {
        super.render()
        if (alpha == 0) {
            return
        }
        var draw_pos = get_draw_position()
        draw(name, draw_pos.x, draw_pos.y,coeff, flip_x, rotation, alpha)
    }
}
