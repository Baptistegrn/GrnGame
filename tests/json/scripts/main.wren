import "std/wren/core/event" for Event
import "std/wren/core/time" for Time
import "std/wren/data/json" for Json

class Main {
static on_start() {

    Json.open("test.json",10,30)
    Json.open("big.json",100,200)
    var big_map = Json.get("big.json")
    __content = Json.get("test.json")

    Event.callback(Event.JsonSave, Fn.new {|name|
            Json.set("test.json",__content)
            Json.save("test.json")
        })

    var start = Time.now()
    Json.set("big.json",big_map)
    Json.save("big.json")
    var end = Time.now()
    System.print("%(end-start)")
    
}

static on_update(dt) {
    __content["time"] = __content["time"] + 1
}
static on_fixed_update(dt) {}
static on_render() {}
static on_destroy() {}


}