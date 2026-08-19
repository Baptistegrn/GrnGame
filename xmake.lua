-- custom package from a pull request
includes("packages/s/soloud/xmake.lua")
-- fix from official package because in wasm pthread option is force set to 0
includes("packages/l/libsdl3fix/xmake.lua")

add_rules("mode.debug", "mode.release")


if is_plat("windows") then
    local msvcRuntime = is_mode("debug") and "MTd" or "MT"
    set_runtimes(msvcRuntime)
end

local asset_pipeline_python = is_host("windows") and "python" or "python3"

option("tracy")
    set_default(false)
    set_showmenu(true)
    set_description("Enable Tracy profiler instrumentation")
option_end()


option("embed_assets")
    set_default(is_plat("wasm"))
    set_showmenu(true)
    set_description("Generate embedded assets before building game (auto-enabled for wasm)")
option_end()

if is_arch("x64") then
    add_defines("GRNGAME_X64")
end

if has_config("tracy") then
    add_requires("tracy")
end

if is_plat("wasm") then
    add_requireconfs("**", {
        configs = {
            cflags   = {"-pthread", "-matomics", "-mbulk-memory"},
            cxxflags = {"-pthread", "-matomics", "-mbulk-memory"},
            ldflags  = {"-pthread"}
        }
    })
end

-- render + input
add_requires("libsdl3fix",       {version = "3.4.0"},      {configs = {shared = false}})
add_requires("libsdl3_image", {version = "3.2.0"},      {configs = {shared = false}})
add_requires("libsdl3_ttf",   {version = "3.2.2"},      {configs = {shared = false, freetype = false}, system = false})

-- maths
add_requires("klib",          {version = "2024.06.03"}, {configs = {shared = false}})
add_requires("cglm",          {version = "v0.9.6"},     {configs = {shared = false}})

-- song
add_requires("soloud pr402", {configs = {shared = false, cxflags = is_arch("arm64") and "-DDR_MP3_NO_SIMD" or nil }})

-- folder
add_requires("tinydir",       {version = "1.2.6"},      {configs = {shared = false}})
if not is_plat("wasm") then
    add_requires("efsw",  {version = "1.6.2"},{configs = {shared = false}, system = false})
end

-- scripting
add_requires("wren",          {version = "0.4.0"},      {configs = {shared = false}})

-- simd
add_requires("highway",       {version = "1.3.0"},      {configs = {shared = false}})

-- files
add_requires("sqlite3",       {version = "3-3.53.0+0"}, {configs = {shared = false}, system = false})
add_requires("cjson",{configs = {shared = false}})

--logs
if not is_plat("wasm") then
    add_requires("quill", {version = "v11.0.2"}, {configs = {shared = false}})
end

set_warnings("all", "extra")

target("GrnGame")
    add_defines("WITH_SDL3_STATIC")
    set_languages("c17", "cxx20")
    set_kind("static")

    add_files("grngame/**.c", "grngame/**.cpp")
    add_headerfiles("grngame/**.h")
    add_includedirs(".", {public = true})

    -- packages
    add_packages(
        "libsdl3fix", "libsdl3_image", "libsdl3_ttf",
        "klib", "cglm", "soloud", "tinydir",
        "wren", "freetype", "sqlite3", "highway", "Libimagequant", "cjson",
        {public = true}
    )
    if not is_plat("wasm") then
        add_packages("quill", "efsw", {public = true})
    end
    if has_config("tracy") then
        add_packages("tracy", {public = true})
    end

    -- platform defines
    if     is_plat("linux")   then
        add_defines("GRNGAME_LINUX", "_GNU_SOURCE", {public = true})
        add_cxxflags("-frtti", "-fexceptions")
    elseif is_plat("windows") then
        add_defines("GRNGAME_WINDOWS", {public = true})
    elseif is_plat("macosx")  then
        add_defines("GRNGAME_MACOS", {public = true})
    end

    -- mode defines
    if     is_mode("debug")   then
        add_defines("GRNGAME_DEBUG", {public = true})
    elseif is_mode("release") then
        add_defines("GRNGAME_RELEASE", {public = true})
        if not is_plat("macosx") then
            set_policy("build.optimization.lto", true)
        end
    end

    if has_config("embed_assets") then
        add_defines("GRNGAME_EMBED_ASSETS", {public = true})
    end

    add_defines("CGLM_USE_ANONYMOUS_STRUCT=1", {public = true})

    if has_config("tracy") then
        add_defines("TRACY_ENABLE", {public = true})
    end

    if not (has_config("embed_assets") or is_plat("wasm")) then
        add_defines("GRNGAME_HOT_RELOAD_ENABLE")
    end

    if is_plat("wasm") then
        add_defines("GRNGAME_WASM")
        add_cxflags("-pthread", {public = true, force = true})
        add_ldflags(
            "--shell-file", "grngame/web/shell.html",
            "-sFORCE_FILESYSTEM=1",
            "-sASYNCIFY",
            "-sALLOW_MEMORY_GROWTH=0",
            "-sPTHREAD_POOL_SIZE=navigator.hardwareConcurrency",
            "-pthread",
            {public = true, force = true}
        )
    end



target("Editor")
    set_languages("c17", "cxx17")
    set_kind("binary")
    set_targetdir(path.join("$(builddir)", "$(plat)", "$(arch)", "$(mode)", "Editor"))
    add_files("editor/main.c")
    add_headerfiles("grngame/**.h")
    add_deps("GrnGame")
    after_build(function(target)
        local scripts_dir = path.join(target:targetdir(), "scripts")
        os.mkdir(scripts_dir)
        os.cp("editor/main.wren", path.join(scripts_dir, "main.wren"))
        os.cp("grngame/input/gamecontrollerdb.txt", target:targetdir())
    end)

if not is_plat("wasm") then
    target("Embedded")
        set_languages("c17", "cxx17")
        set_kind("binary")
        set_targetdir(path.join("$(builddir)", "$(plat)", "$(arch)", "$(mode)", "Embedded"))
        add_files("embedded/main.c")
        add_headerfiles("grngame/**.h")
        add_deps("GrnGame")
end

target("WrenTest")
    set_languages("c17", "cxx17")
    set_kind("binary")
    set_targetdir(path.join("$(builddir)", "$(plat)", "$(arch)", "$(mode)", "WrenTest"))
    add_files("test_game/scripts/main.c")
    add_deps("GrnGame")

    if not is_plat("wasm") then
        add_deps("Embedded")
    end

    after_build(function(target)
        os.execv(asset_pipeline_python, {
            "scripts/asset_pipeline.py",
            "test_game",
            target:targetdir()
        })
        os.cp("grngame/input/gamecontrollerdb.txt", target:targetdir())
    end)

target("SqlTest")
    set_languages("c17", "cxx17")
    set_kind("binary")
    set_targetdir(path.join("$(builddir)", "$(plat)", "$(arch)", "$(mode)", "SqlTest"))
    add_deps("GrnGame")
    add_files("test_sql/main.c")

target("EmbeddedBenchmark")
    set_languages("c17", "cxx17")
    set_kind("binary")
    set_targetdir(path.join("$(builddir)", "$(plat)", "$(arch)", "$(mode)", "EmbeddedBenchmark"))
    add_files("benchmark/embedded/main.c")
    add_headerfiles("grngame/**.h")
    add_deps("GrnGame")

target("LoadTextureFolderBenchmark")
    set_languages("c17", "cxx17")
    set_kind("binary")
    set_targetdir(path.join("$(builddir)", "$(plat)", "$(arch)", "$(mode)", "LoadTextureFolderBenchmark"))
    add_files("benchmark/load_texture_folder/scripts/main.c")
    add_deps("GrnGame")

    if not is_plat("wasm") then
        add_deps("Embedded")
    end

    after_build(function(target)
        os.execv(asset_pipeline_python, {
            "scripts/asset_pipeline.py",
            "benchmark/load_texture_folder",
            target:targetdir()
        })
        os.cp("grngame/input/gamecontrollerdb.txt", target:targetdir())
    end)
