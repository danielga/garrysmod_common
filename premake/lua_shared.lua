function IncludeLuaShared()
	IncludePackage("luashared")

	local _project = project()
	local _workspace = _project.workspace

	sysincludedirs(path.join(_GARRYSMOD_COMMON_DIRECTORY, "include"))

	filter("system:windows or macosx")
		links("lua_shared")

	filter("system:linux")
		local libpath = path.join(_GARRYSMOD_COMMON_DIRECTORY, "projects", os.target(), _ACTION, "%{cfg.architecture}", "%{cfg.buildcfg}", "liblua_shared.a")
		linkoptions({"-Wl,--whole-archive", path.getrelative(_project.location, libpath), "-Wl,--no-whole-archive"})

	group("garrysmod_common")
		project("lua_shared")
			kind("StaticLib")
			location(path.join(_GARRYSMOD_COMMON_DIRECTORY, "projects", os.target(), _ACTION))
			targetdir(path.join("%{prj.location}", "%{cfg.architecture}", "%{cfg.buildcfg}"))
			debugdir(path.join("%{prj.location}", "%{cfg.architecture}", "%{cfg.buildcfg}"))
			objdir(path.join("!%{prj.location}", "%{cfg.architecture}", "%{cfg.buildcfg}", "intermediate", "%{prj.name}"))
			includedirs(path.join(_GARRYSMOD_COMMON_DIRECTORY, "include"))
			files({
				path.join(_GARRYSMOD_COMMON_DIRECTORY, "include", "*.h"),
				path.join(_GARRYSMOD_COMMON_DIRECTORY, "include", "*.hpp"),
				path.join(_GARRYSMOD_COMMON_DIRECTORY, "include", "GarrysMod", "**.h"),
				path.join(_GARRYSMOD_COMMON_DIRECTORY, "include", "GarrysMod", "**.hpp"),
				path.join(_GARRYSMOD_COMMON_DIRECTORY, "source", "LuaShared.cpp"),
				path.join(_GARRYSMOD_COMMON_DIRECTORY, "source", "ModuleLoader.cpp")
			})
			vpaths({
				["Header files/*"] = {
					path.join(_GARRYSMOD_COMMON_DIRECTORY, "include", "**.h"),
					path.join(_GARRYSMOD_COMMON_DIRECTORY, "include", "**.hpp")
				},
				["Source files/*"] = path.join(_GARRYSMOD_COMMON_DIRECTORY, "source", "**.cpp")
			})

			filter("system:linux or macosx")
				links("dl")

	group("")
	project(_project.name)
end
