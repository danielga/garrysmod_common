function IncludeScanning()
	IncludePackage("scanning")

	local directory = _GARRYSMOD_COMMON_DIRECTORY .. "/scanning"

	local _project = project()
	local _workspace = _project.workspace
	local _project_directory = _GARRYSMOD_COMMON_DIRECTORY .. "/projects/" .. os.target() .. "/" .. _ACTION

	includedirs(directory)
	links("scanning")

	project("scanning")
		kind("StaticLib")
		language("C++")
		cppdialect("C++11")
		location(_GARRYSMOD_COMMON_DIRECTORY .. "/projects/" .. os.target() .. "/" .. _ACTION)
		includedirs(directory)
		files({directory .. "/*.hpp", directory .. "/*.cpp"})
		vpaths({
			["Header files/*"] = directory .. "/*.hpp",
			["Source files/*"] = directory .. "/*.cpp"
		})

		filter("configurations:Release")
			objdir(_project_directory .. "/intermediate")
			targetdir(_project_directory .. "/release")

		if not _workspace.abi_compatible then
			filter("configurations:Debug")
				objdir(_project_directory .. "/intermediate")
				targetdir(_project_directory .. "/debug")
		end

		filter("system:linux or macosx")
			links("dl")

		filter("system:macosx")
			links("CoreServices.framework")

	project(_project.name)
end
