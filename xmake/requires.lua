function setup_requires()
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
	add_requires("libsdl3 3.4.12", { configs = { shared = false, threads = true } })
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

	if is_dev_mode() then
		add_requires("tinydir", { configs = { shared = false } })
	end

	if is_desktop_plat() then
		add_requires("haclog add_colors_options", { configs = { shared = false } })
	end
end

function apply_game_packages()
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

	if is_dev_mode() then
		add_packages("tinydir")
	end

	if is_desktop_plat() then
		add_packages("haclog", { public = true })
	end

	if has_config("tracy") then
		add_packages("tracy", { public = true })
	end
end
