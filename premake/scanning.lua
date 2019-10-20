function IncludeScanning()
	IncludePackage("scanning")

	local directory = path.join(_GARRYSMOD_COMMON_DIRECTORY, "include/scanning")

	local _project = project()
	local _workspace = _project.workspace

	sysincludedirs(path.join(_GARRYSMOD_COMMON_DIRECTORY, "include"))
	links("scanning")

	filter("system:macosx")
		links("CoreServices.framework")

	group("garrysmod_common")

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
		targetdir("%{prj.location}/%{cfg.architecture}/%{cfg.buildcfg}")
		debugdir("%{prj.location}/%{cfg.architecture}/%{cfg.buildcfg}")
		objdir("!%{prj.location}/%{cfg.architecture}/%{cfg.buildcfg}/intermediate/%{prj.name}")

		filter("system:linux or macosx")
			links("dl")

	group("")

	project(_project.name)
end
