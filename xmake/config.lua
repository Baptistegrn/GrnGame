function is_desktop_plat()
	return is_plat("windows", "linux", "macosx")
end

function is_embedded()
	return has_config("embed_assets")
end

function is_dev_mode()
	return is_desktop_plat() and not is_embedded()
end
