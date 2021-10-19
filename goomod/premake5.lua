local current_dir = _SCRIPT_DIR

function IncludeGOOMod()
	sysincludedirs(current_dir .. "/include")
	files({
		current_dir .. "/include/GarrysMod/*.hpp",
		current_dir .. "/source/*.cpp"
	})
	vpaths({
		["Header files/goomod/*"] = current_dir .. "/include/GarrysMod/*.hpp",
		["Source files/goomod/*"] = current_dir .. "/source/*.cpp"
	})
end
