// Global engine configuration.

class Config {
    static init() {
        // Application metadata
        __name = "My Game"
        __version = "1.0.0"

        // Engine update rate
        __fps = 60

        // bordered
        __bordered = true

        // Window size
        __windowWidth = 640
        __windowHeight = 360

        // Internal rendering resolution
        __universeWidth = 300
        __universeHeight = 300

        // Window behavior
        __resizable = true
        __fullscreen = false
        __maximised = false

        // Logging
        __enableLogs = true

        // Log output:
        // 0 = Console
        // 1 = JSON file
        // 2 = Text file
        __logDestination = 0

        // Preserve universe aspect ratio.
        // Black bars may be added when required.
        __forceUniverseScale = true

        // Screen clear color index.
        __renderClear = 1

        // Active color palette.
        __colorPalette = []
    }

    // Getters
    static name             { __name }
    static version          { __version }
    static fps              { __fps }
    static windowWidth      { __windowWidth }
    static windowHeight     { __windowHeight }
    static universeWidth    { __universeWidth }
    static universeHeight   { __universeHeight }
    static resizable        { __resizable }
    static fullscreen       { __fullscreen }
    static maximised        { __maximised }
    static enableLogs       { __enableLogs }
    static logDestination   { __logDestination }
    static forceUniverseScale { __forceUniverseScale }
    static renderClear      { __renderClear }
    static colorPalette     { __colorPalette }
    static bordered         { __bordered }

    // Setters
    static name=(v)             { __name = v }
    static version=(v)          { __version = v }
    static fps=(v)              { __fps = v }
    static windowWidth=(v)      { __windowWidth = v }
    static windowHeight=(v)     { __windowHeight = v }
    static universeWidth=(v)    { __universeWidth = v }
    static universeHeight=(v)   { __universeHeight = v }
    static resizable=(v)        { __resizable = v }
    static fullscreen=(v)       { __fullscreen = v }
    static maximised=(v)        { __maximised = v }
    static enableLogs=(v)       { __enableLogs = v }
    static logDestination=(v)   { __logDestination = v }
    static forceUniverseScale=(v) { __forceUniverseScale = v }
    static renderClear=(v)      { __renderClear = v }
    static colorPalette=(v)     { __colorPalette = v }
    static bordered=(v)         { __bordered = v }
}

Config.init()