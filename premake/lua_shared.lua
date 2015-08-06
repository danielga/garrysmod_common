function IncludeLuaShared()
	local folder = _GARRYSMOD_COMMON_FOLDER .. "/lib/" .. os.get()
	
	local curfilter = GetFilter()
	local nosystem = curfilter.system == nil

	includedirs({_GARRYSMOD_COMMON_FOLDER .. "/include"})

	if nosystem or HasFilter(FILTER_WINDOWS) then
		filter(MergeFilters({"system:windows", curfilter.configurations}, curfilter.extra))
			libdirs({folder})
			links({"lua_shared"})
	end

	if nosystem or HasFilter(FILTER_LINUX) then
		filter(MergeFilters({"system:linux", curfilter.configurations}, curfilter.extra))
			libdirs({_SOLUTION_FOLDER})
			local lua_shared_name = _PROJECT_SERVERSIDE and "lua_shared_srv.so" or "lua_shared.so"
			prelinkcommands({
				"mkdir -p garrysmod/bin",
				"cp -fn " .. path.getabsolute(folder .. "/" .. lua_shared_name) .. " garrysmod/bin"
			})
			linkoptions({"-l:garrysmod/bin/" .. lua_shared_name})
	end

	if nosystem or HasFilter(FILTER_MACOSX) then
		filter(MergeFilters({"system:macosx", curfilter.configurations}, curfilter.extra)) -- should probably be similar to linux
			libdirs({folder})
			links({"lua_shared"})
	end

	filter(curfilter.patterns)
end
