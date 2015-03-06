function IncludeLuaShared(folder)
	folder = folder or "../garrysmod_common"

	includedirs({folder .. "/include"})

	local oslibdir = folder .. "/lib/" .. os.get()

	filter({"system:windows"})
		libdirs({oslibdir})
		links({"lua_shared"})

	filter({"system:linux"})
		local lua_shared_name = _PROJECT_SERVERSIDE and "lua_shared_srv.so" or "lua_shared.so"
		local relpath = path.getrelative(
			path.getabsolute(_SOLUTION_FOLDER),
			path.getabsolute(oslibdir .. "/" .. lua_shared_name)
		)
		prelinkcommands({
			"mkdir -p garrysmod/bin",
			"cp -f " .. relpath .. " garrysmod/bin"
		})
		linkoptions({"-l:garrysmod/bin/" .. lua_shared_name})

	filter({"system:macosx"}) -- should probably be similar to linux
		libdirs({oslibdir})
		links({"lua_shared"})

	filter({})
end