function IncludeLuaShared()
	IncludePackage("luashared")

	local _project = project()
	local _workspace = _project.workspace

	includedirs(_GARRYSMOD_COMMON_DIRECTORY .. "/include")

	filter("system:windows")
		libdirs(_GARRYSMOD_COMMON_DIRECTORY .. "/lib/windows")
		links("lua_shared")

	filter("system:linux")
		local library = _project.serverside and "lua_shared_srv.so" or "lua_shared.so"
		prelinkcommands({
			"mkdir -p " .. path.getabsolute(_workspace.directory) .. "/garrysmod/bin",
			"ln -f " .. _GARRYSMOD_COMMON_DIRECTORY .. "/lib/linux/" .. library .. " " .. path.getabsolute(_workspace.directory) .. "/garrysmod/bin/" .. library
		})
		linkoptions({
			"-Wl,--no-as-needed",
			"garrysmod/bin/" .. library,
			"-Wl,--as-needed"
		})

	filter("system:macosx")
		libdirs(_GARRYSMOD_COMMON_DIRECTORY .. "/lib/macosx")
		links("lua_shared")

	filter({})
end
