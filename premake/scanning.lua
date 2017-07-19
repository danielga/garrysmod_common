function IncludeScanning()
	IncludePackage("scanning")

	local directory = _GARRYSMOD_COMMON_DIRECTORY .. "/scanning"

	filter({})

	includedirs(directory)
	files({directory .. "/*.hpp", directory .. "/*.cpp"})
	vpaths({
		["Header files/garrysmod_common/scanning"] = directory .. "/*.hpp",
		["Source files/garrysmod_common/scanning"] = directory .. "/*.cpp"
	})

	filter("system:linux or macosx")
		links("dl")

	filter("system:macosx")
		links("CoreServices.framework")

	filter({})
end
