function IncludeScanning()
	IncludePackage("scanning")

	local directory = path.join(_GARRYSMOD_COMMON_DIRECTORY, "include/scanning")

	local _project = project()
	local _workspace = _project.workspace

	sysincludedirs(path.join(_GARRYSMOD_COMMON_DIRECTORY, "include"))
	links("scanning")

	filter("system:macosx")
		links("CoreServices.framework")

	project("scanning")
		kind("StaticLib")
		location(path.join(_GARRYSMOD_COMMON_DIRECTORY, "projects", os.target(), _ACTION))
		sysincludedirs(directory)
		files({
			path.join(directory, "*.hpp"),
			path.join(directory, "*.cpp")
		})
		vpaths({
			["Header files/*"] = path.join(directory, "*.hpp"),
			["Source files/*"] = path.join(directory, "*.cpp")
		})

		filter("configurations:Release")
			objdir("%{prj.location}/intermediate")
			targetdir("%{prj.location}/release")

		if not _workspace.abi_compatible then
			filter("configurations:Debug")
				objdir("%{prj.location}/intermediate")
				targetdir("%{prj.location}/debug")
		end

		filter("system:linux or macosx")
			links("dl")

	project(_project.name)
end
