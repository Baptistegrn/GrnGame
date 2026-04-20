import "std/wren/ui/ui_node" for UiNode
import "std/wren/ui/centering" for Centering
import "std/wren/math/position" for Position
import "std/wren/renderer/alpha" for Alpha
import "std/wren/dev/log" for Log

class Main {
    static on_start() {
    }

    static on_update(dt) {
    }

    static on_fixed_update(dt) {
    }

    static on_render() {
        __ui_root.render()
    }

    static on_destroy() {
    }
}
