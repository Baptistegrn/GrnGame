add_rules("mode.debug", "mode.release")

local msvcRuntime = is_mode("debug") and "MTd" or "MT"

package("daslang")
    set_homepage("https://github.com/GaijinEntertainment/daScript")
    set_description("daScript - high-performance statically strong typed scripting language")
    set_license("BSD-3-Clause")

    set_urls("https://github.com/GaijinEntertainment/daScript.git")
    add_versions("v0.6.0", "v0.6.0")

    add_deps("cmake")

    add_configs("runtime", {
        description = "MSVC runtime library (MT, MTd, MD, MDd)",
        default = "MT",
        type = "string",
        values = {"MT", "MTd", "MD", "MDd"}
    })

    on_install(function (package)
        local configs = {
            "-DDAS_TOOLS_DISABLED=ON",
            "-DDAS_TESTS_DISABLED=ON",
            "-DDAS_TUTORIAL_DISABLED=ON",
            "-DDAS_AOT_EXAMPLES_DISABLED=ON",
            "-DDAS_CLANG_BIND_DISABLED=ON",
            "-DDAS_LLVM_DISABLED=ON",
            "-DDAS_HV_DISABLED=ON",
            "-DDAS_IMGUI_DISABLED=ON",
            "-DDAS_MINFFT_DISABLED=ON",
            "-DDAS_AUDIO_DISABLED=ON",
            "-DDAS_PUGIXML_DISABLED=ON",
            "-DDAS_SQLITE_DISABLED=ON",
            "-DDAS_TREE_SITTER_DISABLED=ON",
            "-DDAS_FLEX_BISON_DISABLED=ON",
            "-DDAS_GLFW_DISABLED=ON",
            "-DDAS_STDDLG_DISABLED=ON",
            "-DDAS_STBIMAGE_DISABLED=ON",
            "-DDAS_STBTRUETYPE_DISABLED=ON",
        }

        table.insert(configs, "-DCMAKE_BUILD_TYPE=" .. (package:debug() and "Debug" or "Release"))
        table.insert(configs, "-DBUILD_SHARED_LIBS=" .. (package:config("shared") and "ON" or "OFF"))

        if package:config("lto") then
            table.insert(configs, "-DCMAKE_INTERPROCEDURAL_OPTIMIZATION=ON")
        end

        -- select runtime if this is windows
        if package:is_plat("windows") then
            local runtime = package:config("runtime")
            local runtime_flag_map = {
                MT  = "/MT",
                MTd = "/MTd",
                MD  = "/MD",
                MDd = "/MDd",
            }
            local flag = runtime_flag_map[runtime]
            if flag then
                table.insert(configs, "-DCMAKE_MSVC_RUNTIME_LIBRARY=" .. ({
                    ["/MT"]  = "MultiThreaded",
                    ["/MTd"] = "MultiThreadedDebug",
                    ["/MD"]  = "MultiThreadedDLL",
                    ["/MDd"] = "MultiThreadedDebugDLL",
                })[flag])
            end
        end

        import("package.tools.cmake").install(package, configs)
    end)
package_end()

add_requires("daslang v0.6.0", {
    configs = {
        lto = false,
        shared = false,
        runtime = msvcRuntime
    }
})
add_requires("libsdl3", {version = "3.4.0"},{configs={runtimes=msvcRuntime, shared=false}})
add_requires("libsdl3_image", {version = "3.2.0"},{configs={runtimes=msvcRuntime, shared=false}})
add_requires("libsdl3_ttf", {version = "3.2.2"},{configs={runtimes=msvcRuntime, shared=false}})
add_requires("quill", {version = "v11.0.2"},{configs={runtimes=msvcRuntime, shared=false}})
add_requires("klib", {version = "2024.06.03"},{configs={runtimes=msvcRuntime, shared=false}})
add_requires("cglm", {version = "v0.9.6"},{configs={runtimes=msvcRuntime, shared=false}})
add_requires("soloud",{configs={runtimes=msvcRuntime, shared=false}})
add_requires("tinydir",{configs={runtimes=msvcRuntime, shared=false}})
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
        "daslang",
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