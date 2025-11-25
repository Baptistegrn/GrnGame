add_rules("mode.release", "mode.debug")

add_requires("libsdl2", {system = false, configs = {shared = false}})
add_requires("libsdl_image", {system = false, configs = {shared = false}})
add_requires("libsdl_mixer", {system = false, configs = {shared = false}})
add_requires("quill", {system = false, configs = {shared = false}})
add_requires("zlib", {system = false, configs = {shared = false}})
add_requires("expat", {system = false, configs = {shared = false}})
add_requires("nanobind", {system = false}) 

target("LibGrnGame")
    set_languages("cxx17")
    set_kind("shared") 
    set_targetdir("../dist")

    on_load(function (target)
        import("lib.detect.find_tool")
        
        local os_plat = target:plat()
        
        local python = find_tool("python")
        local py_ver = "unknown"
        
        if python and python.program then
            local out, err = os.iorunv(python.program, {"--version"})
            

            local major, minor = out:match("Python (%d+)%.(%d+)")
            if major then
                py_ver = "python" .. major .. "." .. minor 
            end
        end

        local new_dir = path.join("../dist", os_plat, py_ver)
        target:set("targetdir", new_dir)
        
    end)
    add_files("src/**.cpp")
    add_files("src/**.c")
    add_headerfiles("src/**.h")
    add_headerfiles("src/**.hpp")
    

    -- Optimisation
    if is_mode("release") and not is_plat("windows") then
        set_policy("build.optimization.lto", true) 
    end

    -- macOS
    if is_plat("macosx") then
        add_frameworks("Cocoa", "CoreVideo", "IOKit", "ForceFeedback", "Carbon", "CoreAudio", "AudioToolbox", "Metal")
        add_defines("MACOS_PLATFORM")
    end

    -- Linux
    if is_plat("linux") then
        set_prefixname("") 
    end

    -- Windows
    if is_plat("windows") then
        add_rules("utils.symbols.export_all")
        set_extension(".pyd")
    end

    add_packages("libsdl", "libsdl_image", "libsdl_mixer", "zlib", "expat", "nanobind", "quill")