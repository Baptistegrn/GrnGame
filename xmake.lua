add_rules("mode.debug", "mode.release")
local msvcRuntime = is_mode("debug") and "MTd" or "MT"
set_runtimes(msvcRuntime)

option("tracy")
    set_default(false)
    set_showmenu(true)
    set_description("Enable Tracy profiler instrumentation")
option_end()

add_requires("libsdl3", {version = "3.4.0"},{configs={shared=false}})
add_requires("libsdl3_image", {version = "3.2.0"},{configs={shared=false}})
add_requires("libsdl3_ttf", {version = "3.2.2"}, {configs={shared=false, freetype=false}, system=false})
add_requires("quill", {version = "v11.0.2"},{configs={shared=false}})
add_requires("klib", {version = "2024.06.03"},{configs={shared=false}})
add_requires("cglm", {version = "v0.9.6"},{configs={shared=false}})
add_requires("soloud",{version = "2020.02.07", configs={shared=false}})
add_requires("tinydir",{version = "1.2.6", configs={shared=false}})
add_requires("wren", {version = "0.4.0", configs = {shared=false}})

local asset_pipeline_python = is_plat("windows") and "python" or "python3"

local function add_steam_support()
    add_linkdirs("plugins", { public = true })
    if is_plat("linux") then
        add_links("steam_api", { public = true })
    elseif is_plat("windows") then 
        add_links("steam_api64", { public = true })
    elseif is_plat("macosx") then 
        add_links("steam_api", { public = true }) 
    end
end

set_warnings("all", "extra")
target("GrnGame")
    set_languages("c17", "cxx20")
    set_kind("static")
    add_steam_support()

    add_rules("utils.bin2obj", {extensions = ".txt"})
    add_files("grngame/input/gamecontrollerdb.txt")

    add_files("grngame/**.c")
    add_files("grngame/**.cpp")
    add_headerfiles("grngame/**.h")
    add_headerfiles("grngame/**.hpp")

    if has_config("tracy") then
        add_files("external/tracy/public/TracyClient.cpp")
        add_includedirs("external/tracy/public", { public = true })
    end

    add_includedirs("." , { public = true }) 

    if is_plat("linux") then
        add_defines(
            "GRNGAME_LINUX",
            "_GNU_SOURCE", -- So we get access to GNU/Posix extensions
            { public = true })
        add_cxxflags("-frtti", "-fexceptions")
    elseif is_plat("windows") then
        add_defines("GRNGAME_WINDOWS", { public = true })
    elseif is_plat("macosx") then
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
        "wren",
        "freetype",
        { public = true }
    )

    -- Force cglm struct aliases (.x/.y/.z/.w) across C and C++ translation units.
    add_defines("CGLM_USE_ANONYMOUS_STRUCT=1", { public = true })

    if has_config("tracy") then
        add_defines("TRACY_ENABLE", { public = true })
    end

target("Editor")
    set_languages("c17", "cxx17")
    set_kind("binary")
    set_targetdir(path.join("$(builddir)", "$(plat)", "$(arch)", "$(mode)", "Editor"))
    add_files("editor/main.c")
    add_headerfiles("grngame/**.h")
    add_headerfiles("grngame/**.hpp")
    add_deps("GrnGame")
    after_build(function(target)
        local scripts_dir = path.join(target:targetdir(), "scripts")
        os.mkdir(scripts_dir)
        os.cp("editor/main.wren", path.join(scripts_dir, "main.wren"))
    end)

target("Embedded")
    set_languages("c17", "cxx17")
    set_kind("binary")
    add_files("embedded/main.c")
    add_headerfiles("grngame/**.h")
    add_headerfiles("grngame/**.hpp")
    add_deps("GrnGame")

target("WrenTest")    
    set_languages("c17", "cxx17")
    set_kind("binary")
    add_files("test_game/scripts/main.c")
    add_deps("GrnGame")
    add_deps("Embedded")
    --add_defines("GRN_EMBED_ASSETS")

    -- before_build(function(target)
    --     local embedded_exe = target:dep("Embedded"):targetfile()
    --     if os.isexec(embedded_exe) then
    --         os.mkdir("build/generated")
    --         os.execv(embedded_exe, {
    --             "build/generated/embedded_assets.h",
    --             "test_game/scripts",
    --             "test_game/assets",
    --             "std"
    --         })
    --     end
    -- end)

        after_build(function(target)
        os.execv(asset_pipeline_python, {
            "scripts/asset_pipeline.py",
            "test_game",
            target:targetdir()
        })
    end)
