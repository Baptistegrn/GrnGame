import "std/wren/utils/file" for File
import "std/wren/json" for JSON

class JSONFile {
    static exists(path) {
        return File.exists(path)
    }

    static load(path) {
        var content = File.read(path)
        if (content == null) return null
        return JSON.parse(content)
    }

    static save(path, object) {
        return File.write(path, JSON.stringify(object))
    }
}