function IncludeLuaShared()
	IncludePackage("luashared")

	local _project = project()
	local _workspace = _project.workspace
	local _project_directory = _GARRYSMOD_COMMON_DIRECTORY .. "/projects/" .. os.target() .. "/" .. _ACTION

	includedirs(_GARRYSMOD_COMMON_DIRECTORY .. "/include")
	links("lua_shared")

	project("lua_shared")
		kind("StaticLib")
		language("C++")
		cppdialect("C++11")
		location(_GARRYSMOD_COMMON_DIRECTORY .. "/projects/" .. os.target() .. "/" .. _ACTION)
		includedirs(_GARRYSMOD_COMMON_DIRECTORY .. "/include")
		files({
			_GARRYSMOD_COMMON_DIRECTORY .. "/include/**.h",
			_GARRYSMOD_COMMON_DIRECTORY .. "/include/**.hpp",
			_GARRYSMOD_COMMON_DIRECTORY .. "/source/LuaShared.cpp"
		})
		vpaths({
			["Header files/*"] = {
				_GARRYSMOD_COMMON_DIRECTORY .. "/include/**.h",
				_GARRYSMOD_COMMON_DIRECTORY .. "/include/**.hpp"
			},
			["Source files/*"] = _GARRYSMOD_COMMON_DIRECTORY .. "/source/LuaShared.cpp"
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

	project(_project.name)
end
