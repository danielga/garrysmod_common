function IncludeLuaShared()
	IncludePackage("luashared")

	local _project = project()
	local _workspace = _project.workspace

	sysincludedirs(path.join(_GARRYSMOD_COMMON_DIRECTORY, "include"))
	links("lua_shared")

	project("lua_shared")
		kind("StaticLib")
		location(path.join(_GARRYSMOD_COMMON_DIRECTORY, "projects", os.target(), _ACTION))
		sysincludedirs(path.join(_GARRYSMOD_COMMON_DIRECTORY, "include"))
		files({
			path.join(_GARRYSMOD_COMMON_DIRECTORY, "include/**.h"),
			path.join(_GARRYSMOD_COMMON_DIRECTORY, "include/**.hpp"),
			path.join(_GARRYSMOD_COMMON_DIRECTORY, "source/LuaShared.cpp")
		})
		vpaths({
			["Header files/*"] = {
				path.join(_GARRYSMOD_COMMON_DIRECTORY, "include/**.h"),
				path.join(_GARRYSMOD_COMMON_DIRECTORY, "include/**.hpp")
			},
			["Source files/*"] = path.join(_GARRYSMOD_COMMON_DIRECTORY, "source/LuaShared.cpp")
		})
		targetdir("%{prj.location}/%{cfg.architecture}/%{cfg.buildcfg}")
		debugdir("%{prj.location}/%{cfg.architecture}/%{cfg.buildcfg}")
		objdir("!%{prj.location}/%{cfg.architecture}/%{cfg.buildcfg}/intermediate/%{prj.name}")

		filter("system:linux or macosx")
			links("dl")

	project(_project.name)
end
