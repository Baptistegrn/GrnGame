// Global engine configuration.

import "std/wren/renderer/primitive" for Primitive
import "std/wren/dev/log" for Log
import "std/wren/core/window" for Window

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
        // 1 = Text file
        // 2 = Json file
        __logDestination = 0

        // Preserve universe aspect ratio.
        // Black bars may be added when required.
        __forceUniverseScale = true

        // Screen clear color index.
        __renderClear = 0

        // Active color palette.
    __colorPalette = [
    "#FF0000", "#83603d", "#FFFF00", "#7FFF00",
    "#00FF00", "#00FF7F", "#00FFFF", "#007FFF",
    "#0000FF", "#7F00FF", "#FF00FF", "#FF007F",
    "#8B0000", "#FF4500", "#FFD700", "#ADFF2F",
    "#32CD32", "#20B2AA", "#00CED1", "#1E90FF",
    "#4169E1", "#8A2BE2", "#DA70D6", "#C71585",
    "#2F4F4F", "#696969", "#708090", "#A9A9A9",
    "#D2691E", "#B8860B", "#BC8F8F", "#F5F5F5"
    ]

    // intern
    __palette_has_changed = false
    __window_has_changed = false

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
    static palette_has_changed { __palette_has_changed }
    static window_has_changed  { __window_has_changed }

    // Setters
    static name=(v)             { __name = v }
    static version=(v)          { __version = v }
    static fps=(v)              { __fps = v }

    static windowWidth=(v)      { 
        __windowWidth = v 
        __window_has_changed = true 
    }

    static windowHeight=(v)     { 
        __windowHeight = v
         __window_has_changed = true
    }

    static universeWidth=(v)    { 
        __universeWidth = v
         __window_has_changed = true
    }

    static universeHeight=(v)   { 
        __universeHeight = v
         __window_has_changed = true
    }

    static resizable=(v)        { 
        __resizable = v
         __window_has_changed = true
    }

    static fullscreen=(v)       { 
        __fullscreen = v
        __window_has_changed = true
    }

    static maximised=(v)        { 
        __maximised = v
        __window_has_changed = true
    }

    static enableLogs=(v)       { __enableLogs = v }
    static logDestination=(v)   { __logDestination = v }

    static forceUniverseScale=(v) { 
        __forceUniverseScale = v
        __window_has_changed = true 
    }

    static renderClear=(v)      { __renderClear = v }

    static colorPalette=(v)     { 
        __colorPalette = v 
        __palette_has_changed = true
    }
    static bordered=(v)         { 
        __bordered = v
        __window_has_changed = true
    }

    static window_has_changed=(v){ __window_has_changed =v}
    static palette_has_changed=(v){ __palette_has_changed =v}

    static apply(){
        if (Config.window_has_changed) {
            Window.apply_config(Config.name, Config.version,
                Config.universeWidth, Config.universeHeight,
                Config.windowWidth, Config.windowHeight,
                Config.fullscreen, Config.maximised, Config.resizable,
                Config.bordered, Config.forceUniverseScale)
                __window_has_changed = false
        }

        Log.apply_config(Config.enableLogs, Config.logDestination)
        Primitive.set_render_color(Config.renderClear)

        if (Config.palette_has_changed){
            Primitive.palette_reload()
            __palette_has_changed = false
        }
    }
    
}

Config.init()
