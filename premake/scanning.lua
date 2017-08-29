function IncludeScanning()
	IncludePackage("scanning")

	local directory = _GARRYSMOD_COMMON_DIRECTORY .. "/scanning"

	local _project = project()

	includedirs(directory)
	links("scanning")

	project("scanning")
		kind("StaticLib")
		language("C++")
		cppdialect("C++11")
		includedirs(directory)
		files({directory .. "/*.hpp", directory .. "/*.cpp"})
		vpaths({
			["Header files/*"] = directory .. "/*.hpp",
			["Source files/*"] = directory .. "/*.cpp"
		})

		filter("system:linux or macosx")
			links("dl")

		filter("system:macosx")
			links("CoreServices.framework")

	project(_project.name)
end
