import "std/wren/all" for Sound, SoundInfo, FilterDef, Position, Log, PadNode, InputEvent, PadButton, Window,Node,UiNode

class Main {
  static on_start() {

    __ui_nodes = []
    for (i in 0...100) {
        var tex = (i % 3 + 1).toString
        var node = UiNode.new("ui_%(i)", tex)
        node.padding.x = i 
        node.padding.y = i
        node.alpha=200
        node.rotation = 100
        
        __ui_nodes.add(node)
    }

  }

  static on_update(dt) {
  }

  static on_fixed_update(dt) {}
  
  static on_render() {
    for (node in __ui_nodes) {
        node.render()
    }
  }
  
  static on_destroy() {
  }
}

