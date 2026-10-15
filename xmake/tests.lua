function add_test_target(name)
	local desktop = is_desktop_plat()

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
		if desktop then
			if not embedded then
				os.execv(python.program, {
					"scripts/test/dev_pipeline.py",
					target_name,
					build_dir,
				})
			else
				os.execv(python.program, {
					"scripts/test/embedded_pipeline.py",
					target_name,
					build_dir,
					os.projectdir(),
					path.join("build", "Embedded"),
				})
			end
		else
			print("You cant execute tests on mobile/web")
		end

		local plat = get_config("plat") or os.host()
		local arch = get_config("arch") or os.arch()
		local mode = get_config("mode") or "release"
		local suffix = embedded and "embedded" or ""
		local ext = is_host("windows") and ".exe" or ""

		local runtime_path = path.join(build_dir, "Runtime-" .. plat .. "-" .. arch .. "-" .. mode .. suffix .. ext)

		os.execv(runtime_path)
	end)
	target_end()
end

local function add_group_target(group, names)
	target(group)
	set_kind("phony")
	if #names > 0 then
		set_values(group, table.unpack(names))
	end
	on_run(function(target)
		for _, name in ipairs(target:values(group) or {}) do
			os.execv("xmake", { "run", "tests/" .. name })
		end
	end)
	target_end()
end

function setup_tests(target_tests, auto_tests)
	for _, name in ipairs(target_tests) do
		add_test_target(name)
	end
	for _, name in ipairs(auto_tests) do
		add_test_target(name)
	end

	add_group_target("tests", target_tests)
	add_group_target("auto_tests", auto_tests)
end
