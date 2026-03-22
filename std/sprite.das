options gen2

require node
require grngame_renderer

class SpriteNode : Node {
    spr : Sprite
    frame : int
    flip : bool
    alpha : int
    rotation : int
    coeff : int

    def SpriteNode(name: string, sprite_name : string, w : int, h : int) {
        Node`Node(self, name)
        self.spr = sprite(sprite_name, w, h)
        self.frame = 0
        self.flip = false
        self.alpha = 255
        self.rotation = 0
        self.coeff = 1
    }

    def override render() {
        super.render()
        let wp = get_world_position()
        sprite_draw(spr, frame, wp.x, wp.y, coeff, flip, rotation, alpha)
    }
}