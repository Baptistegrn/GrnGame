add_rules("mode.debug", "mode.release")

add_requires("libsdl2", {configs = {system = false}})
add_requires("libsdl2_image", {configs = {system = false}})
add_requires("libsdl2_mixer", {configs = {system = false}})
add_requires("nanobind")

target("LibGrnGame")
    set_languages("cxx23")
    set_kind("shared") 
    set_targetdir("../dist")
    add_files("src/**.cpp")
    add_files("src/**.c")
    add_headerfiles("src/**.h")
    add_headerfiles("src/**.hpp")

    if is_mode("release") and not is_plat("windows") then
        -- ajout de la lto en optimisation, mais pas sur windows car ca pète utils.symbols.export_all
        set_policy("build.optimization.lto", true) 
    end

    if is_plat("macosx") then
        add_frameworks("Cocoa", "CoreVideo", "IOKit", "ForceFeedback", "Carbon", "CoreAudio", "AudioToolbox")
        add_defines("MACOS_PLATFORM")
    end

    if is_plat("linux") then
        set_prefixname("") 
    end

    if is_plat("windows") then
        add_rules("utils.symbols.export_all") -- besoin d'exporter les symboles sur windows
        set_extension(".pyd")
    end

    add_packages("libsdl2", "libsdl2_image", "libsdl2_mixer", "nanobind")