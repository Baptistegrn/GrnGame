class Event {
    static PadConnect    { 0 }
    static PadDisconnect { 1 }
    foreign static callback(event, fn)
}