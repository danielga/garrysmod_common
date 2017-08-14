function IncludeLuaShared()
	IncludePackage("luashared")

	local _project = project()

	includedirs(_GARRYSMOD_COMMON_DIRECTORY .. "/include")
	files(_GARRYSMOD_COMMON_DIRECTORY .. "/source/LuaShared.cpp")
	vpaths({["Source files/garrysmod_common"] = _GARRYSMOD_COMMON_DIRECTORY .. "/source/LuaShared.cpp"})

	filter("system:windows")
		libdirs(_GARRYSMOD_COMMON_DIRECTORY .. "/lib/windows")

	filter("system:linux")
		libdirs(_GARRYSMOD_COMMON_DIRECTORY .. "/lib/linux")
		links("dl")

	filter("system:macosx")
		libdirs(_GARRYSMOD_COMMON_DIRECTORY .. "/lib/macosx")
		links("dl")

	filter({})
end
