function IncludeScanning()
	IncludePackage("scanning")

	local directory = _GARRYSMOD_COMMON_DIRECTORY .. "/scanning"

	filter({})

	includedirs(directory)
	files({
		directory .. "/*.hpp",
		directory .. "/*.cpp"
	})
	vpaths({
		["Header files/*"] = directory .. "/*.hpp",
		["Source files/*"] = directory .. "/*.cpp"
	})

	filter("system:linux or macosx")
		links("dl")

	filter({})
end
