function IncludeLuaShared()
	IncludePackage("luashared")

	local _project = project()

	includedirs(_GARRYSMOD_COMMON_DIRECTORY .. "/include")

	filter("system:windows")
		libdirs(_GARRYSMOD_COMMON_DIRECTORY .. "/lib/windows")
		links("lua_shared")

	filter("system:linux")
		libdirs(_GARRYSMOD_COMMON_DIRECTORY .. "/lib/linux")
		linkoptions(_project.serverside and "-l:garrysmod/bin/lua_shared_srv.so" or "-l:garrysmod/bin/lua_shared.so")

	filter("system:macosx")
		libdirs(_GARRYSMOD_COMMON_DIRECTORY .. "/lib/macosx")
		linkoptions("-l:garrysmod/bin/lua_shared.dylib")

	filter({})
end
