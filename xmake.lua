add_rules("mode.debug", "mode.release")

add_requires("libsdl3", {version = "3.4.0"})
add_requires("libsdl3_image", {version = "3.2.0"})
add_requires("libsdl3_ttf", {version = "3.2.2"})
add_requires("quill", {version = "v11.0.2"})
add_requires("klib", {version = "2024.06.03"})
add_requires("cglm", {version = "v0.9.6"})

target("GrnGame")
    set_languages("c17", "cxx17")
    set_kind("shared")

    add_files("grngame/**.c")
    add_files("grngame/**.cpp")
    add_headerfiles("grngame/**.h")
    add_headerfiles("grngame/**.hpp")

    add_includedirs("." , { public = true }) -- pour pouvoir #include <grngame/*.h> et pas "../../*.h"

    add_packages(
        "libsdl3",
        "libsdl3_image",
        "libsdl3_ttf",
        "quill",
        "klib",
        "cglm",
        { public = true }
    )

target("App")    
    set_languages("c17", "cxx17")
    set_kind("binary")

    add_files("app/main.cpp")
    add_deps("GrnGame")

target("Tests")    
    set_languages("c17", "cxx17")
    set_kind("binary")

    add_files("tests/main.cpp")
    add_deps("GrnGame")