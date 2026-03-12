options gen2

module node public;

class Node {
    name : string
    position : float2
    children : array<Node?>
    parent : Node?

    def Node(name : string) {
        self.name = name
        self.parent = null
        self.position = float2(0.0, 0.0)
    }

    def add_child(var node : Node?) {
        node.parent = unsafe(addr(self))
        children.push(node)
    }

    def get_node(name : string) : Node? {
        for (child in children) {
            if (child.name == name) {
                return child;
            }
        }
        return null
    }

    def start() {
        for (c in children) {
            c.start()
        }
    }

    def update(dt : float) {
        for (c in children) {
            c.update(dt)
        }
    }

    def render(dt : float)  {
        for (c in children) {
            c.render(dt)
        }
    }

    def fixed_update(dt : float)  {
        for (c in children) {
            c.fixed_update(dt)
        }
    }

    def destroy() {
        for (c in children) {
            c.destroy()
        }
    }

    def finalize {
        destroy()
    }
}