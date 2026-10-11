local function bool_option(name, default, description)
	option(name)
	set_default(default)
	set_showmenu(true)
	set_description(description)
	option_end()
end

function setup_options()
	bool_option("tracy", false, "Enable Tracy profiler instrumentation")

	bool_option(
		"embed_assets",
		not is_desktop_plat(),
		"Generate embedded assets before building game (forced on for wasm/android/iOS)"
	)

	bool_option("software_renderer", false, "set renderer on software")
	bool_option("pixel_int_position", true, "allow only int positions for textures and sprites")
	bool_option("big_buffer", false, "set every stack buffer bigger x5")
end
