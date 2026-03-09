add_rules("mode.debug", "mode.release")

add_requires("libsdl3", {version = "3.4.0"},{configs={runtimes="MT", shared=false}})
add_requires("libsdl3_image", {version = "3.2.0"},{configs={runtimes="MT", shared=false}})
add_requires("libsdl3_ttf", {version = "3.2.2"},{configs={runtimes="MT", shared=false}})
add_requires("quill", {version = "v11.0.2"},{configs={runtimes="MT", shared=false}})
add_requires("klib", {version = "2024.06.03"},{configs={runtimes="MT", shared=false}})
add_requires("cglm", {version = "v0.9.6"},{configs={runtimes="MT", shared=false}})
add_requires("soloud",{configs={runtimes="MT", shared=false}})
add_requires("tinydir",{configs={runtimes="MT", shared=false}})

set_warnings("all", "extra")
target("GrnGame")
    set_languages("c17", "cxx17")
    set_kind("static")

    add_rules("utils.bin2obj", {extensions = ".txt"})
    add_files("grngame/input/gamecontrollerdb.txt")

    add_files("grngame/**.c")
    add_files("grngame/**.cpp")
    add_headerfiles("grngame/**.h")
    add_headerfiles("grngame/**.hpp")

    add_includedirs("." , { public = true }) 

    if is_plat("linux") then
        add_defines(
            "GRNGAME_LINUX",
            "_GNU_SOURCE", -- So we get access to GNU/Posix extensions
            { public = true })
    elseif is_plat("windows") then
        add_defines("GRNGAME_WINDOWS", { public = true })
                set_runtimes("MT")
    elseif is_plat("macos") then
        add_defines("GRNGAME_MACOS", { public = true })
    end

    if is_mode("debug") then
        add_defines("GRNGAME_DEBUG", { public = true })
    elseif is_mode("release") then 
        add_defines("GRNGAME_RELEASE" , { public = true })
        set_policy("build.optimization.lto", true)
    end

    add_packages(
        "libsdl3",
        "libsdl3_image",
        "libsdl3_ttf",
        "quill",
        "klib",
        "cglm",
        "soloud",
        "tinydir",
        { public = true }
    )

target("App")    
    set_languages("c17", "cxx17")
    set_kind("binary")
                set_runtimes("MT")
    add_files("app/main.c")
    add_deps("GrnGame")

target("Tests")    
    set_languages("c17", "cxx17")
    set_kind("binary")
                set_runtimes("MT")
    add_files("tests/main.c")
    add_deps("GrnGame")