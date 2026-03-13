options gen2

require node
require grngame_renderer

class SpriteNode : Node {
    spr: Sprite
    frame: int
    flip_x: bool
    alpha: int
    rotation: int
    coeff: int

    def SpriteNode(sprite_name: string, w: int, h: int) {
        Node`Node(self, sprite_name)
        self.spr = sprite(sprite_name, w, h)
        self.frame = 0
        self.flip_x = false
        self.alpha = 255
        self.rotation = 0
        self.coeff = 1
    }

    def override render() {
        super.render()
        sprite_draw(spr, frame, position.x, position.y, coeff, flip_x, rotation, alpha)
    }
}