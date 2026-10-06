import "std/wren/core/event" for Event
import "std/wren/core/time" for Time
import "std/wren/data/json" for Json
import "std/wren/dev/log" for Log
import "std/wren/dev/exit" for Exit

class Main {
static on_start() {
    Log.info("hello from grngame")
}

static on_update(dt) {
    Exit.engine_stop()
}
static on_fixed_update(dt) {}
static on_render() {}
static on_destroy() {}


}