function IncludeLuaShared()
	IncludePackage("luashared")

	local _project = project()

	includedirs(_GARRYSMOD_COMMON_DIRECTORY .. "/include")
	links("lua_shared")

	project("lua_shared")
		kind("StaticLib")
		language("C++")
		cppdialect("C++11")
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

		filter("system:linux or macosx")
			links("dl")

	project(_project.name)
end
