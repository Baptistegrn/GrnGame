-- Dépendances globales
add_requires("libsdl2",        {configs={runtimes="MT", shared=false, pic=true}})
add_requires("libsdl2_image",  {configs={runtimes="MT", shared=false, png=true, jpg=false, tiff=false, webp=false, pic=true}})
add_requires("libsdl2_mixer",  {configs={runtimes="MT", shared=false, wav=true, mp3=false, flac=false, vorbis=false, pic=true}})
add_requires("zlib",           {configs={runtimes="MT", shared=false, pic=true}})
add_requires("quill",          {configs={runtimes="MT", shared=false, pic=true}})
add_requires("lua",            {configs={runtimes="MT", kind="static", pic=true}})
add_requires("sol2",{configs={runtimes="MT", kind="static", pic=true}})
add_requires("enet6")
add_requires("yyjson")

-- mode release uniquement ( debug gere par des options )
add_rules("mode.release")

--extension executable
local os =""

-- Option debug
option("debug")
    set_default(false)
    set_description("Activate quill logs in file log")
    set_showmenu(true)

-- Dossier de sortie selon la config
local outdir = "bin/release"

if has_config("debug") then
    outdir = "bin/debug"
end

--lib
target("GrnGame")
    set_kind("shared")
    add_rules("utils.symbols.export_all")

    set_languages("c17", "cxx17")
    set_targetdir(outdir)

    -- Sources
    add_files("src/**.c")
    add_files("src/**.cpp")
    add_headerfiles("src/**.h")
    add_headerfiles("src/**.hpp")

    --packages
    add_packages(
        "libsdl2",
        "libsdl2_image",
        "libsdl2_mixer",
        "zlib",
        "quill",
        "lua",
        "sol2",
        "enet6",
        "yyjson",
        { public = true }
    )

    --flags
    if has_config("debug") then
        add_defines("DEBUG_MODE")
    end

    -- Plateforme
    if is_plat("windows") then
        -- runtime statique
        set_runtimes("MT")
        add_ldflags("/OPT:REF", "/OPT:ICF", {force=true})
        os = "Windows"

    elseif is_plat("linux") then
        -- phtread pour sdl 
        add_syslinks("pthread", "dl", "m")
        -- c++ en statique (NON FONCTIONNEL)
        add_ldflags("-static-libgcc", "-static-libstdc++", {force=true})
        os = "Linux"

    elseif is_plat("macosx") then
        -- Frameworks macOS pour SDL
        add_frameworks("Cocoa", "IOKit", "CoreVideo", "CoreAudio", "AudioToolbox", "Carbon", "ForceFeedback", "Metal")
        add_syslinks("iconv")
        os = "MacOs"
    end

--game app
target("App".. os)
    set_kind("binary")
    set_targetdir(outdir)
    if has_config("debug_mode") then
        add_defines("DEBUG_MODE")
    end
    add_files("src/moteur_lua/lancer_moteur.c")
    -- inclusions moteur
    add_includedirs("src/headers_sdl")
    add_includedirs("src/bindings_c")

    -- Dépendance moteur
    add_deps("GrnGame")

    if is_plat("windows") then
        --runtime statique
        set_runtimes("MT")

    elseif is_plat("linux") then
        -- pour charger libGrnGame.so
        add_rpathdirs("$ORIGIN")

    elseif is_plat("macosx") then
        -- pour charger libGrnGame.dylib
        add_rpathdirs("@executable_path")
    end