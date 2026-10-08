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

option("software_renderer")
	set_default(false)
	set_showmenu(true)
	set_description("set renderer on software")
option_end()

local embedded = has_config("embed_assets") 
local dev_mode = is_desktop and not embedded 

if has_config("software_renderer") then 
	add_defines("GRNGAME_SOFTWARE", { public = true })
end

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
add_requires("libsdl3_mixer", { configs = { shared = false } })

-- scripting
add_requires("wren Map-api", { version = "Map-api" }, { configs = { shared = false } })

-- simd
add_requires("highway", { configs = { shared = false } })

-- files
add_requires("sqlite3", { configs = { shared = false }, system = false })
add_requires("cjson", { configs = { shared = false } })


if dev_mode then
	add_requires("tinydir", { configs = { shared = false } })
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
	set_languages("c17")
	set_kind("static")

	add_files("grngame/**.c")
	remove_files("grngame/embedded/embedded_file_generator_main.c")
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
		"libsdl3_mixer",
		{ public = true }
	)

	if dev_mode then
		add_packages("tinydir")
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
			"-sINITIAL_MEMORY=512MB",
			"-sALLOW_MEMORY_GROWTH=1",
			"-sPTHREAD_POOL_SIZE=navigator.hardwareConcurrency",
			"-pthread",
			{ public = true, force = true }
		)
	end

local plat = get_config("plat") or os.host()
local arch = get_config("arch") or os.arch()
local mode = get_config("mode") or "release"


if dev_mode then
	if get_config("plat") == os.host() and get_config("arch") == os.arch() then
		target("Embedded-" .. plat .. "-" .. arch .. "-" .. mode)
			set_languages("c17")
			set_kind("binary")
			set_targetdir(path.join("$(builddir)", "Embedded"))
			add_files("grngame/embedded/embedded_file_generator_main.c")
			add_headerfiles("grngame/**.h")
			add_deps("GrnGame")
	end
end

if is_plat("android") then
	target("Runtime-" .. plat .. "-" .. arch .. "-" .. mode .. suffix)
		set_kind("shared")
		set_basename("GrnGame")
		set_languages("c17")
		set_targetdir(path.join("$(builddir)", "Runtime"))
		add_files("runtime/main.c")
		add_deps("GrnGame")
else
	target("Runtime-" .. plat .. "-" .. arch .. "-" .. mode .. suffix)
		set_kind("binary")
		set_languages("c17")
		set_targetdir(path.join("$(builddir)", "Runtime"))
		add_files("runtime/main.c")
		add_deps("GrnGame")
end

function add_test_target(name)
	target("tests/" .. name)
		set_kind("phony")
		on_run(function(target)
			import("lib.detect.find_tool")
 

			local python = find_tool("python") or find_tool("python3")
			assert(python, "Python not found!")
 
			local target_name = target:name() 
			local build_dir = path.join("build", target_name)
 
			local embed = get_config("embed_assets")
			local embedded = (embed == true or embed == "true" or embed == "y")
 
			os.execv("xmake", { "build", "-y" })

			-- only tests on desktop 
			if is_desktop then 
				if not embedded then
					os.execv(python.program, {
						"scripts/asset_pipeline.py",
						target_name,
						build_dir,
					})
				else
					os.execv(python.program, {
						"scripts/embedded.py",
						target_name,
						build_dir,
						os.projectdir(),
						path.join("build", "Embedded"),
					})
				end
			else 

				print("You cant execute tests on mobile/web")

			end
 
			local plat = get_config("plat")
			local arch = get_config("arch")
			local mode = get_config("mode") or "release"
			local suffix = embedded and "-embedded" or "" -- à adapter à ton nommage réel
			local ext = is_host("windows") and ".exe" or ""
 
			local runtime_path =
				path.join(build_dir, "Runtime-" .. plat .. "-" .. arch .. "-" .. mode .. suffix .. ext)
 
			os.execv(runtime_path)
		end)
	target_end()
end


target_tests = { "json", "pad_event","engine_work","sound" }
auto_tests = {}

for _, name in ipairs(target_tests) do
	add_test_target(name)
end

for _, name in ipairs(auto_tests) do
	add_test_target(name)
end

target("tests")
	set_kind("phony")
	set_values("tests", table.unpack(target_tests))
	on_run(function(target)
		for _, name in ipairs(target:values("tests") or {}) do
			os.execv("xmake", { "run", "tests/" .. name })
		end
	end)
target_end()

target("auto_tests")
	set_kind("phony")
	if #auto_tests > 0 then
		set_values("auto_tests", table.unpack(auto_tests))
	end
	on_run(function(target)
		for _, name in ipairs(target:values("auto_tests") or {}) do
			os.execv("xmake", { "run", "tests/" .. name })
		end
	end)
target_end()


includes("@builtin/xpack")

local stage   = os.getenv("GRNGAME_STAGE") or ""
local version = os.getenv("GRNGAME_VERSION") or "0.0.0"

xpack("grngame")
    set_title("GrnGame")
    set_author("Baptiste GUERIN")
    set_description("GrnGame platformer 2d")
    set_version(version)
    set_basename("grngame-$(plat)-$(arch)-$(version)")

    if is_plat("windows") then
        set_formats("wix")
        add_installfiles(stage .. "/(**)")
        before_installcmd(function (package, batchcmds)
            batchcmds:rawcmd("wix", [[
<Environment Id="GrnGamePath" Name="PATH" Value="[INSTALLFOLDER]scripts"
             Part="last" Action="set" System="yes" Permanent="no" />
]])
        end)
    elseif is_plat("linux") then
        set_formats("deb")
        add_installfiles(stage .. "/(**)", {prefixdir = "lib/grngame"})
        add_installfiles("launchers/(*)",  {prefixdir = "bin"})
    end