function IncludeLuaShared()
	IncludePackage("luashared")

	local _project = project()

	filter({})

	includedirs(_GARRYSMOD_COMMON_DIRECTORY .. "/include")

	filter("system:windows")
		libdirs(_GARRYSMOD_COMMON_DIRECTORY .. "/lib/windows")
		links("lua_shared")

	filter("system:linux")
		libdirs(_GARRYSMOD_COMMON_DIRECTORY .. "/lib/linux")
		linkoptions({
			"-Wl,--no-as-needed",
			"-l:garrysmod/bin/" .. (_project.serverside and "lua_shared_srv.so" or "lua_shared.so"),
			"-Wl,--as-needed"
		})

	filter("system:macosx")
		libdirs(_GARRYSMOD_COMMON_DIRECTORY .. "/lib/macosx")
		links("lua_shared")

	filter({})
end
