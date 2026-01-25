-- Dépendances globales
add_requires("libsdl2",        {configs={runtimes="MT", shared=false, pic=true}})
add_requires("libsdl2_image",  {configs={runtimes="MT", shared=false, png=true, jpg=false, tiff=false, webp=false, pic=true}})
add_requires("libsdl2_mixer",  {configs={runtimes="MT", shared=false, wav=true, mp3=false, flac=false, vorbis=false, pic=true}})
add_requires("zlib",           {configs={runtimes="MT", shared=false, pic=true}})
add_requires("quill",          {configs={runtimes="MT", shared=false, pic=true}})
add_requires("luajit",         {configs={runtimes="MT", kind="static", pic=true}})

-- Règle globale
add_rules("mode.release")

-- Option malloc
option("malloc_mode")
    set_default(false)
    set_description("Active le mode malloc (logs d'allocation mémoire) + Quill")
    set_showmenu(true)

-- Option debug / logs
option("debug_mode")
    set_default(false)
    set_description("Active les logs (Quill)")
    set_showmenu(true)

-- Dossier de sortie selon la config
local outdir = "bin/release"

if has_config("debug_mode") then 
    outdir = "bin/debug"
end

if has_config("malloc_mode") then
    outdir = "bin/debug_malloc"
end

-- Cible moteur (DLL / SO)
target("GrnGame")
    set_kind("shared")
    add_rules("utils.symbols.export_all")

    set_languages("c17", "cxx17")
    set_targetdir(outdir)

    -- Sources
    add_files("src/**.c")
    add_files("src/**.cpp")
    add_headerfiles("src/**.h")

    -- Packages
    add_packages(
        "libsdl2",
        "libsdl2_image",
        "libsdl2_mixer",
        "zlib",
        "quill",
        "luajit"
    )

    -- Defines
    if has_config("debug_mode") then
        add_defines("DEBUG_MODE")
    end

    if has_config("malloc_mode") then
        add_defines("MALLOC_MODE")
        add_defines("DEBUG_MODE")
    end

    -- Plateforme
    if is_plat("windows") then
        --runtime statique
        set_runtimes("MT")
        add_ldflags("/OPT:REF", "/OPT:ICF", {force=true})

    elseif is_plat("linux") then
        --set_prefixname("")
        --phtread pour sdl 
        add_syslinks("pthread", "dl", "m")
        --c++ en statique
        add_ldflags("-static-libgcc", "-static-libstdc++", {force=true})
    end

-- Application de test
target("App")
    set_kind("binary")
    set_targetdir(outdir)
    if has_config("debug_mode") then
        add_defines("DEBUG_MODE")
    end
    add_files("src/lua/lancer_moteur.c")
    -- Includes moteur
    add_includedirs("src/headers_sdl")
    add_includedirs("src/bindings_c")

    -- Dépendance moteur
    add_deps("GrnGame")

    if is_plat("windows") then
        --runtime statique
        set_runtimes("MT")

    elseif is_plat("linux") then
        -- pour charger grngame.dll
        add_rpathdirs("$ORIGIN")
    end