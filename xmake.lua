add_rules("mode.debug", "mode.release")
if is_plat("windows") then
    local msvcRuntime = is_mode("debug") and "MTd" or "MT"
    set_runtimes(msvcRuntime)
end

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

add_requires("libsdl3",       {version = "3.4.0"},      {configs = {shared = false}})
add_requires("libsdl3_image", {version = "3.2.0"},      {configs = {shared = false}})
add_requires("libsdl3_ttf",   {version = "3.2.2"},      {configs = {shared = false, freetype = false}, system = false})
add_requires("klib",          {version = "2024.06.03"}, {configs = {shared = false}})
add_requires("cglm",          {version = "v0.9.6"},     {configs = {shared = false}})
add_requires("soloud",        {version = "2020.02.07"}, {configs = {shared = false}})
add_requires("tinydir",       {version = "1.2.6"},      {configs = {shared = false}})
add_requires("wren",          {version = "0.4.0"},      {configs = {shared = false}})
add_requires("sqlite3", {version = "3-3.53.0+0"}, {configs = {shared = false}, system = false})

if not is_plat("wasm") then
    add_requires("quill", {version = "v11.0.2"}, {configs = {shared = false}})
    add_requires("efsw",{configs = {shared = false}, system = false})
end
if is_plat("wasm") then
    add_defines("WASM")
    add_ldflags(
        "--shell-file",
        "grngame/web/shell.html",
        {force = true}
    )
end

local asset_pipeline_python = is_host("windows") and "python" or "python3"

local function get_embedded_exe()
    local host = os.host()
    local name = is_host("windows") and "Embedded.exe" or "Embedded"
    return path.join("embedded", host:sub(1,1):upper() .. host:sub(2), name)
end

local function add_steam_support()
    if not is_plat("wasm") then
        add_linkdirs("plugins", {public = true})
        if     is_plat("linux")   then add_links("steam_api",   {public = true})
        elseif is_plat("windows") then add_links("steam_api64", {public = true})
        elseif is_plat("macosx")  then add_links("steam_api",   {public = true})
        end
    end
end

local function add_grngame_packages(with_quill_and_efsw)
    add_packages(
        "libsdl3", "libsdl3_image", "libsdl3_ttf",
        "klib", "cglm", "soloud", "tinydir",
        "wren", "freetype", "sqlite3",
        {public = true}
    )
    if with_quill_and_efsw then
        add_packages("quill","efsw", {public = true})
    end
end

local function add_grngame_defines()
    if     is_plat("linux")   then
        add_defines("GRNGAME_LINUX", "_GNU_SOURCE", {public = true})
        add_cxxflags("-frtti", "-fexceptions")
    elseif is_plat("windows") then add_defines("GRNGAME_WINDOWS", {public = true})
    elseif is_plat("macosx")  then add_defines("GRNGAME_MACOS",   {public = true})
    end

    if     is_mode("debug")   then add_defines("GRNGAME_DEBUG",   {public = true})
    elseif is_mode("release") then
        add_defines("GRNGAME_RELEASE", {public = true})
        if not is_plat("macosx") then
            set_policy("build.optimization.lto", true)
        end
    end

    add_defines("CGLM_USE_ANONYMOUS_STRUCT=1", {public = true})

    if has_config("tracy") then
        add_defines("TRACY_ENABLE", {public = true})
    end
end

local function set_binary_targetdir(target_name)
    set_targetdir(path.join("$(builddir)", "$(plat)", "$(arch)", "$(mode)", target_name))
end

set_warnings("all", "extra")

target("GrnGame")
    set_languages("c17", "cxx20")
    set_kind("static")
    add_steam_support()

    add_files("grngame/**.c", "grngame/**.cpp")
    add_headerfiles("grngame/**.h")
    add_includedirs(".", {public = true})
    if has_config("tracy") then
        add_files("external/tracy/public/TracyClient.cpp")
        add_includedirs("external/tracy/public", {public = true})
    end

    add_grngame_defines()
    add_grngame_packages(not is_plat("wasm"))

target("Editor")
    set_languages("c17", "cxx17")
    set_kind("binary")
    set_binary_targetdir("Editor")
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
        set_binary_targetdir("Embedded")
        add_files("embedded/main.c")
        add_headerfiles("grngame/**.h")
        add_deps("GrnGame")
end

target("WrenTest")
    set_languages("c17", "cxx17")
    set_kind("binary")
    set_binary_targetdir("WrenTest")
    add_files("test_game/scripts/main.c")
    add_deps("GrnGame")

    if not is_plat("wasm") then
        add_deps("Embedded")
    end

    if has_config("embed_assets") then
        add_defines("GRN_EMBED_ASSETS")
        add_includedirs("resources")
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
    set_binary_targetdir("SqlTest")
    add_deps("GrnGame")
    add_files("test_sql/main.c")