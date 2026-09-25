-- official package from a pull request
includes("packages/s/soloud/xmake.lua")
includes("packages/w/wren/xmake.lua")
-- official package from my repo with custom settings
includes("packages/h/haclog/xmake.lua")

add_rules("mode.debug", "mode.release")


local is_desktop = is_plat("windows", "linux", "macosx")

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
	set_default(not is_desktop)
	set_showmenu(true)
	set_description("Generate embedded assets before building game (forced on for wasm/android/iOS)")
option_end()


local embedded = has_config("embed_assets") 
local dev_mode = is_desktop and not embedded 

local suffix = embedded and "embedded" or ""

if is_plat("android") then
	add_ldflags("-Wl,--export-dynamic", "-rdynamic", { force = true })
	add_ldflags("-u", "JNI_OnLoad", "-u", "SDL_main", { force = true })
end

if is_arch("x64") then
	add_defines("GRNGAME_X64")
end

if has_config("tracy") then
	add_requires("tracy")
end

if is_plat("wasm") then
	add_requireconfs("**", {
		configs = {
			cflags = { "-pthread", "-matomics", "-mbulk-memory" },
			cxxflags = { "-pthread", "-matomics", "-mbulk-memory" },
			ldflags = { "-pthread" },
		},
	})
end


-- render + input
-- if we update sdl version we need to update android-build because its based on sdl build
add_requires("libsdl3", { version = "3.4.12" }, { configs = { shared = false, threads = true } })
add_requires("libsdl3_image", { configs = { shared = false } })
add_requires("libsdl3_ttf", { configs = { shared = false, freetype = false }, system = false })

-- maths
add_requires("klib", { configs = { shared = false } })
add_requires("cglm", { configs = { shared = false } })

-- sound
add_requires("soloud pr402", { configs = { shared = false, cxflags = is_arch("arm64") and "-DDR_MP3_NO_SIMD" or nil } })

-- scripting
add_requires("wren Map-api", { version = "Map-api" }, { configs = { shared = false } })

-- simd
add_requires("highway", { configs = { shared = false } })

-- files
add_requires("sqlite3", { configs = { shared = false }, system = false })
add_requires("cjson", { configs = { shared = false } })


if dev_mode then
	add_requires("tinydir", { configs = { shared = false } })
	add_requires("efsw", { configs = { shared = false }, system = false })
end

if is_desktop then 
	add_requires("haclog add_colors_options", { configs = { shared = false } })
end 

set_warnings("all", "extra")

-- log in android
if is_plat("android") then
	add_syslinks("log")
end

target("GrnGame")
	if is_plat("android") then
		add_cflags("-fPIC")
	end

	add_defines("WITH_SDL3_STATIC")
	set_languages("c11", "cxx20")
	set_kind("static")

	add_files("grngame/**.c", "grngame/**.cpp")
	remove_files("grngame/assets/embedded_main.c")
	add_headerfiles("grngame/**.h")
	add_includedirs(".", { public = true })

	add_packages(
		"libsdl3",
		"libsdl3_image",
		"libsdl3_ttf",
		"klib",
		"cglm",
		"soloud",
		"wren",
		"freetype",
		"sqlite3",
		"highway",
		"Libimagequant",
		"cjson",
		{ public = true }
	)

	if dev_mode then
		add_packages("tinydir", "efsw")
		add_defines("GRNGAME_DEV_MODE", { public = true })
	end

	if is_desktop then 
		add_packages("haclog", { public = true })
		add_defines("GRNGAME_DESKTOP", { public = true })
	end

	if has_config("tracy") then
		add_packages("tracy", { public = true })
	end

	-- platform defines
	if is_plat("linux") then
		add_defines("GRNGAME_LINUX", "_GNU_SOURCE", { public = true })
		add_cxxflags("-frtti", "-fexceptions")
	elseif is_plat("windows") then
		add_defines("GRNGAME_WINDOWS", { public = true })
	elseif is_plat("macosx") then
		add_defines("GRNGAME_MACOS", { public = true })
	elseif is_plat("android") then
		add_defines("GRNGAME_ANDROID", { public = true })
	elseif is_plat("iphoneos") then
		add_defines("GRNGAME_IOS", { public = true })
	end

	-- mode defines
	if is_mode("debug") then
		add_defines("GRNGAME_DEBUG", { public = true })
	elseif is_mode("release") then
		add_defines("GRNGAME_RELEASE", { public = true })
		if not is_plat("macosx") then
			set_policy("build.optimization.lto", true)
		end
	end

	if embedded then
		add_defines("GRNGAME_EMBED_ASSETS", { public = true })
	end

	if dev_mode then
		add_defines("GRNGAME_HOT_RELOAD_ENABLE")
	end

	add_defines("CGLM_USE_ANONYMOUS_STRUCT=1", { public = true })

	if has_config("tracy") then
		add_defines("TRACY_ENABLE", { public = true })
	end

	if is_plat("wasm") then
		add_defines("GRNGAME_WASM", { public = true })
		add_ldflags(
			"--shell-file",
			"grngame/web/shell.html",
			"-sFORCE_FILESYSTEM=1",
			"-sASYNCIFY",
			"-sALLOW_MEMORY_GROWTH=0",
			"-sPTHREAD_POOL_SIZE=navigator.hardwareConcurrency",
			"-pthread",
			{ public = true, force = true }
		)
	end

local plat = get_config("plat") or os.host()
local arch = get_config("arch") or os.arch()
local mode = get_config("mode") or "release"


if is_desktop then
	target("Embedded-" .. plat .. "-" .. arch .. "-" .. mode)
		set_languages("c11", "cxx20")
		set_kind("binary")
		set_targetdir(path.join("$(builddir)", "Embedded"))
		add_files("grngame/assets/embedded_main.c")
		add_headerfiles("grngame/**.h")
		add_deps("GrnGame")
end

if is_plat("android") then
	target("Runtime-" .. plat .. "-" .. arch .. "-" .. mode .. suffix)
		set_kind("shared")
		set_basename("GrnGame")
		set_languages("c17", "cxx20")
		set_targetdir(path.join("$(builddir)", "Runtime"))
		add_files("runtime/main.c")
		add_deps("GrnGame")
else
	target("Runtime-" .. plat .. "-" .. arch .. "-" .. mode .. suffix)
		set_kind("binary")
		set_languages("c17", "cxx20")
		set_targetdir(path.join("$(builddir)", "Runtime"))
		add_files("runtime/main.c")
		add_deps("GrnGame")
end

target("tests/json")
	set_kind("phony")
	on_run(function(target)
		import("lib.detect.find_tool")

		local python = find_tool("python") or find_tool("python3")
		assert(python, "Python not found!")

		local target_name = target:name()
		os.execv("xmake")

		if not embedded then
			os.execv(python.program, {
				"scripts/asset_pipeline.py",
				target_name,
				path.join("build", target_name),
			})
		end

		local plat = get_config("plat")
		local arch = get_config("arch")
		local mode = get_config("mode") or "release"
		local ext = is_host("windows") and ".exe" or ""

		local runtime_path =
			path.join("build", "tests", "json", "Runtime-" .. plat .. "-" .. arch .. "-" .. mode .. suffix .. ext)

		os.execv(runtime_path)
	end)

target("tests/pad_event")
	set_kind("phony")
	on_run(function(target)
		import("lib.detect.find_tool")

		local python = find_tool("python") or find_tool("python3")
		assert(python, "Python not found!")

		local target_name = target:name()
		os.execv("xmake")

		if not embedded then
			os.execv(python.program, {
				"scripts/asset_pipeline.py",
				target_name,
				path.join("build", target_name),
			})
		end

		local plat = get_config("plat")
		local arch = get_config("arch")
		local mode = get_config("mode") or "release"
		local ext = is_host("windows") and ".exe" or ""

		local runtime_path =
			path.join("build", "tests", "pad_event", "Runtime-" .. plat .. "-" .. arch .. "-" .. mode .. suffix .. ext)

		os.execv(runtime_path)
	end)