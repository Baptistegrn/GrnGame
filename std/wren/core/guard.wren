import "std/wren/dev/log" for Log

class Guard {
    static isType(value, expectedType, msg) {
        if (!(value is expectedType)) {
            Log.warn("%(msg) (Expected: %(expectedType), Received: %(value.type))")
            return false
        }
        return true
    }
}