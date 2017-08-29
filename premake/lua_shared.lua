function IncludeLuaShared()
	IncludePackage("luashared")

	local _project = project()

	includedirs(_GARRYSMOD_COMMON_DIRECTORY .. "/include")
	files(_GARRYSMOD_COMMON_DIRECTORY .. "/source/LuaShared.cpp")
	vpaths({["Source files/garrysmod_common"] = _GARRYSMOD_COMMON_DIRECTORY .. "/source/LuaShared.cpp"})

	filter("system:linux or macosx")
		links("dl")

	filter({})
end
