import "std/wren/data/db" for Db,DbStmt
import "std/wren/dev/log" for Log

class Main {
  static on_start() {
    __db = Db.new()

    __db.open("test.db")
    __db.begin()
    for (t in 0...10) {
      var table = "table_%(t)"
      __db.write("CREATE TABLE IF NOT EXISTS %(table) (id INTEGER PRIMARY KEY, value TEXT);")
      for (i in 1..100) {
        __db.write("INSERT INTO %(table) (id, value) VALUES (%(i), 'item_%(i)');")
      }
    }
    __db.commit()

    __stmt = __db.prepare("UPDATE table_0 SET value = ? WHERE id = ?")
  }

  static on_update(dt) {
    __db.begin()
    for (i in 1..100) {
        __db.write("UPDATE table_0 SET value = 'updated_%(i)' WHERE id = %(i);")
    }
    __db.commit()
  }

  static on_fixed_update(dt) {    
  }

  static on_render() {}

  static on_destroy() {
    if (__stmt != null) {
      __stmt.free()
      __stmt = null
    }
    __db.close()
  }
}