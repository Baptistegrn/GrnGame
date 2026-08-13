package("soloud")

    set_description("SoLoud is an easy to use, free, portable c/c++ audio engine for games.")
    set_homepage("https://github.com/jarikomppa/soloud")
    set_license("zlib")

    add_urls("https://github.com/samgubernick/soloud.git", {
        branch = "main"
    })
    add_versions("pr402", "e9e8aea38d65a6f3a565ccf6103a4883ae8bb9c6")

    add_deps("miniaudio")

    if is_plat("linux") then
        add_syslinks("pthread", "dl")
    end

    on_install(function (package)
        os.rm("src/backend/miniaudio/miniaudio.h")

        if package:is_plat("macosx", "iphoneos") then
            os.mv("src/backend/miniaudio/soloud_miniaudio.cpp",
                  "src/backend/miniaudio/soloud_miniaudio.mm")
        end

        io.writefile("xmake.lua", [[
            add_rules("mode.debug", "mode.release")

            add_requires("miniaudio")

            target("soloud")
                set_kind("$(kind)")
                set_languages("cxx11")
                add_headerfiles("include/(**.h)")
                add_includedirs("include", {public = true})

                add_defines("WITH_MINIAUDIO")
                add_packages("miniaudio")

                add_files("src/**.cpp|tools/**.cpp|backend/**.cpp|backend/**.mm")
                add_files("src/**.c|tools/**.c|backend/**.c")

                if is_plat("iphoneos", "macosx") then
                    add_files("src/backend/miniaudio/*.mm")
                else
                    add_files("src/backend/miniaudio/*.cpp")
                end
        ]])

        import("package.tools.xmake").install(package)
    end)

    on_test(function (package)
        assert(package:check_cxxsnippets({test = [[
            void test(int args, char** argv) {
                SoLoud::Soloud soloud;
            }
        ]]}, {includes = "soloud.h"}))
    end)

