options gen2

require node
require grngame_input

class InputNode : Node {
    events: table<int, array<lambda>>

    def register_callback(key: int, callback: lambda): int {
        return 0
    }

    def override update(dt: float) {

    }
}