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

    def SpriteNode(name: string, sprite_name : string, w : int, h : int,flip:bool = false,alpha:int =255,rotation:int= 0,coeff:int =1) {
        Node`Node(self, name)
        self.spr = sprite(sprite_name, w, h)
        self.frame = 0
        self.flip = flip
        self.alpha = alpha
        self.rotation = rotation
        self.coeff =coeff
    }

    def override render() {
        super.render()
        let wp = get_world_position()
        sprite_draw(spr, frame, wp.x, wp.y, coeff, flip, rotation, alpha)
    }
}