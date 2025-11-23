add_rules("mode.debug", "mode.release")

local libs_static = {
    system = false, 
    configs = {
        shared = false, 
        pic = true
    }
}


add_requires("zlib", libs_static)
add_requires("expat", libs_static)
add_requires("libflac", libs_static)
add_requires("libogg", libs_static)
add_requires("libvorbis", libs_static)
add_requires("libopus", libs_static)
add_requires("libpng", libs_static)
add_requires("libjpeg-turbo", libs_static)


add_requires("libsdl2", libs_static)


add_requires("libsdl2_mixer", {
    system = false,
    configs = {
        shared = false, 
        pic = true,
        flac = true,
        vorbis = true,
        opus = true,
        mpg123 = true
    }
})

add_requires("libsdl2_image", {
    system = false,
    configs = {
        shared = false, 
        pic = true,
        png = true,
        jpeg = true,
        tiff = true,
        webp = true
    }
})

-- Outils C++
add_requires("nanobind") 
add_requires("quill", libs_static)

target("LibGrnGame")
    set_languages("cxx17")
    set_kind("shared") 
    set_targetdir("../dist")
    
    add_files("src/**.cpp")
    add_files("src/**.c")
    add_headerfiles("src/**.h")
    add_headerfiles("src/**.hpp")

    if is_mode("release") and not is_plat("windows") then
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
        add_rules("utils.symbols.export_all")
        set_extension(".pyd")
    end


    add_packages(
        "libsdl2",
        "libsdl2_image",
        "libsdl2_mixer",
        "zlib",     
        "expat",    
        "nanobind",
        "quill"
    )
