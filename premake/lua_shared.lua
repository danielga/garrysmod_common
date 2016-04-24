function IncludeLuaShared()
	IncludePackage("luashared")

	local folder = _GARRYSMOD_COMMON_DIRECTORY .. "/lib/" .. os.get()

	filter({})

	includedirs(_GARRYSMOD_COMMON_DIRECTORY .. "/include")

	filter("system:windows")
		libdirs(folder)
		links("lua_shared")

	filter("system:linux")
		libdirs(folder)
		linkoptions({
			"-Wl,--no-as-needed",
			"-l:garrysmod/bin/" .. (_PROJECT.serverside and "lua_shared_srv.so" or "lua_shared.so"),
			"-Wl,--as-needed"
		})

	filter("system:macosx")
		libdirs(folder)
		links("lua_shared")

	filter({})
end
