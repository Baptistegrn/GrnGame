import "std/wren/data/file" for File
import "std/wren/data/json" for JSON
import "std/wren/dev/log" for Log

class JSONFile {
    static ensureWarning() {
        if (__warned == null) {
            Log.log_warning("JSON parsing and I/O are expensive, avoid using File methods in update() or draw().")
            __warned = true
        }
    }
    static exists(path) {
        return File.exists(path)
    }

    static load(path) {
        ensureWarning()
        var content = File.read(path)
        if (content == null) {
            Log.log_warning("JSONFile: Impossible to load %(path)")
            return null
        }
        return JSON.parse(content)
    }

    static save(path, object) {
        ensureWarning()
        var jsonString = JSON.stringify(object)
        if (jsonString == null) {
            Log.log_warning("JSONFile: Failed to stringify object for %(path)")
            return false
        }
        return File.write(path, jsonString)
    }
}