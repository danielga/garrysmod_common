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
