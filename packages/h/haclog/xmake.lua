package("haclog")
    set_homepage("https://github.com/MuggleWei/haclog")
    set_description("Haclog(Happy Aync C log) is an extremely fast plain C logging library")
    set_license("MIT")

    add_urls("https://github.com/Baptistegrn/haclog.git", {
        branch = "master"
    })
    add_versions("color_fix", "0ba7db67b12f6f237770281ba2a7d9c80e589c8e")

    if is_plat("linux", "bsd") then
        add_syslinks("pthread")
    end

    on_load(function (package)
        if package:is_plat("cross") then
            package:add("deps", "meson", "ninja")
        else
            package:add("deps", "cmake")
        end
    end)

    on_install("windows", "linux", "macosx", "android", "cross", function (package)
        local configs = {}
        if package:is_plat("cross") then
            table.insert(configs, "-Ddefault_library=" .. (package:config("shared") and "shared" or "static"))
            import("package.tools.meson").install(package, configs)
        else
            io.replace("CMakeLists.txt", [[message(FATAL_ERROR "-- building for unsupport platform")]], "", {plain = true})

            table.insert(configs, "-DCMAKE_BUILD_TYPE=" .. (package:is_debug() and "Debug" or "Release"))
            table.insert(configs, "-DBUILD_SHARED_LIBS=" .. (package:config("shared") and "ON" or "OFF"))
            import("package.tools.cmake").install(package, configs)
        end
    end)

    on_test(function (package)
        assert(package:has_cfuncs("haclog_console_handler_init", {includes = "haclog/haclog.h"}))
    end)