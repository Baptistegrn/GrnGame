class Event {
    static PadConnect    { 0 }
    static PadDisconnect { 1 }
    static callback(index, fn) {
            callback_a(index, fn, fn.arity)
        }

    foreign static callback_a(index, fn, arity)
}