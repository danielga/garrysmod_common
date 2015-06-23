newoption({
	trigger = "sourcesdk",
	description = "Sets the path to the SourceSDK directory",
	value = "path to SourceSDK directory"
})

function IncludeSourceSDK(folder)
	folder = folder or _OPTIONS["sourcesdk"] or os.getenv("SOURCE_SDK") or DEFAULT_SOURCESDK_FOLDER

	if folder == nil then
		error("you didn't supply a path to your SourceSDK copy")
	end

	folder = CleanPath(folder)
	local dir = path.getabsolute(folder)
	if not os.isdir(dir) then
		error(dir .. " doesn't exist (SourceSDK)")
	end

	defines({"SUPPRESS_INVALID_PARAMETER_NO_INFO", _PROJECT_SERVERSIDE and "GAME_DLL" or "CLIENT_DLL"})
	includedirs({
		folder .. "/common",
		folder .. "/public",
		folder .. "/public/tier0",
		folder .. "/public/tier1"
	})
	if _PROJECT_SERVERSIDE then
		includedirs({
			folder .. "/game/server",
			folder .. "/game/shared"
		})
	else
		includedirs({
			folder .. "/game/client",
			folder .. "/game/shared"
		})
	end
	vpaths({["Source files"] = folder .. "/**.cpp"})

	local curfilter = GetFilter()
	local nosystem = curfilter.system == nil

	if nosystem or HasFilter(FILTER_WINDOWS) then
		filter({"system:windows", curfilter.configurations})
			files({folder .. "/public/tier0/memoverride.cpp"})
			libdirs({folder .. "/lib/public"})
			links({"ws2_32", "tier0", "tier1", "vstdlib"})

			if curfilter.configurations == nil or HasFilter(FILTER_DEBUG) then
				filter({"system:windows", "configurations:Debug"})
					linkoptions({"/NODEFAULTLIB:\"libcmt\""})
			end
	end

	if nosystem or HasFilter(FILTER_LINUX) then
		filter({"system:linux", curfilter.configurations})
			defines({"COMPILER_GCC", "POSIX", "LINUX", "_LINUX", "GNUC", "NO_MALLOC_OVERRIDE"})
			if _PROJECT_SERVERSIDE then
				libdirs({_SOLUTION_FOLDER})
				prelinkcommands({
					"mkdir -p bin",
					"cp -fn " .. folder .. "/lib/public/linux32/libtier0.so bin/libtier0_srv.so",
					"cp -fn " .. folder .. "/lib/public/linux32/libvstdlib.so bin/libvstdlib_srv.so"
				})
				linkoptions({"-l:bin/libtier0_srv.so", "-l:bin/libvstdlib_srv.so"})
			else
				libdirs({folder .. "/lib/public/linux32"})
				links({"tier0", "vstdlib"})
			end
			linkoptions({folder .. "/lib/public/linux32/tier1.a"})
	end

	if nosystem or HasFilter(FILTER_MACOSX) then
		filter({"system:macosx", curfilter.configurations}) -- should probably be similar to linux
			libdirs({folder .. "/lib/public/osx32"})
			links({"tier0", "tier1", "vstdlib"})
	end

	filter(curfilter.patterns)

	_SOURCE_SDK_INCLUDED = true
end