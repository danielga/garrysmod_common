function IncludeLuaShared(folder)
	folder = folder or "../garrysmod_common"

	includedirs({folder .. "/include"})

	local oslibdir = folder .. "/lib/" .. os.get()
	local curfilter = GetFilter()
	local nosystem = curfilter.system == nil

	if nosystem or HasFilter(FILTER_WINDOWS) then
		filter({"system:windows", curfilter.configurations})
			libdirs({oslibdir})
			links({"lua_shared"})
	end

	if nosystem or HasFilter(FILTER_LINUX) then
		filter({"system:linux", curfilter.configurations})
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
	end

	if nosystem or HasFilter(FILTER_MACOSX) then
		filter({"system:macosx", curfilter.configurations}) -- should probably be similar to linux
			libdirs({oslibdir})
			links({"lua_shared"})
	end

	filter(curfilter.patterns)
end