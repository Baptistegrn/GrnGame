options gen2

require node
require grngame_input

class InputNode : Node {
    aliases: table<string, int>
    events : table<int, array<lambda>>

    def InputNode() {
        Node`Node(self, "InputNode")
    }

    def add_alias(name: string, key: int) : void {
        aliases.insert(name, key)
    }

    def add_callback(alias: string, var callback: lambda) : bool {
        if (!aliases.key_exists(alias)) {
            return false
        }

        let aliased_key = aliases |> get_value(alias)
        add_callback_raw(aliased_key, callback)
        return true
    }

    def add_callback_raw(key: int, var callback: lambda) : void {
        if (!events.key_exists(key)) {
            events[key] <- array<lambda>()
        }
        events[key].emplace(callback)
    }

    def override update(dt: float) {
        for (key, callbacks in keys(events), values(events)) {
            if (key_just_pressed(key)) {
                for (cb in callbacks) {
                    invoke(cb)
                }
            }
        }
    }
}