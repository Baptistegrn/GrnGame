import "config" for Config

foreign class Window {

    foreign static scale
    foreign static width
    foreign static height
    foreign static universe_width
    foreign static universe_height
    foreign static offset_x
    foreign static offset_y
    foreign static change
    foreign static force_universe_scale

    foreign static apply_config(name, version,
                                universeWidth, universeHeight,
                                windowWidth, windowHeight,
                                fullscreen, maximised,
                                resizable, bordered,
                                forceUniverseScale)

    static apply() {
        apply_config(Config.name, Config.version,
            Config.universeWidth, Config.universeHeight,
            Config.windowWidth, Config.windowHeight,
            Config.fullscreen, Config.maximised, Config.resizable,
            Config.bordered, Config.forceUniverseScale)
    }

}