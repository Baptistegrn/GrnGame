add_rules("mode.debug", "mode.release")

-- SDL et extensions
add_requires("libsdl2", {system = false})
add_requires("libsdl2_image", {system = false})
add_requires("libsdl2_mixer", {system = false})

-- Audio
add_requires("libflac", {system = false})
add_requires("libogg", {system = false})
add_requires("libvorbis", {system = false})
add_requires("mpg123", {system = false})
add_requires("libopus", {system = false})

-- Image et compression
add_requires("libpng", {system = false})
add_requires("libjpeg-turbo", {system = false})
add_requires("libtiff", {system = false})
add_requires("libwebp", {system = false})
add_requires("zlib", {system = false})
add_requires("expat", {system = false})

-- Utilitaires C++
add_requires("nanobind")
add_requires("quill")

target("LibGrnGame")
    set_languages("cxx17")
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

	add_packages(
	    "libsdl2",
	    "libsdl2_image",
	    "libsdl2_mixer",
	    "libflac",
	    "libogg",
	    "libvorbis",
	    "mpg123",
	    "libopus",
	    "libpng",
	    "libjpeg-turbo",
	    "libtiff",
	    "libwebp",
	    "zlib",
	    "expat",
	    "nanobind",
	    "quill"
	)

