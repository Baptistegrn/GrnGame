import "std/wren/all" for Unicorn


class Wren {
  static on_start() {//System.print("on start")
  }
  static on_update(dt) {//System.print("on update %(dt)")
  }
  static on_fixed_update(dt) {}
  static on_render() {//System.print("on render")
  }
  static on_destroy() {}
}
var fred = Unicorn.new("Fred", "palomino")
var on_start = Fn.new { Wren.on_start() }
var on_update = Fn.new {|dt| Wren.on_update(dt) }
var on_fixed_update = Fn.new {|dt| Wren.on_fixed_update(dt) }
var on_render = Fn.new { Wren.on_render() }
var on_destroy = Fn.new { Wren.on_destroy() }

