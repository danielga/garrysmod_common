function IncludeSourceSDK(path)
	if not path then
		error("you didn't supply a path to your SourceSDK copy")
	end

	defines({"SUPPRESS_INVALID_PARAMETER_NO_INFO", _PROJECT_SERVERSIDE and "GAME_DLL" or "CLIENT_DLL"})
	includedirs({
		path .. "/common",
		path .. "/public",
		path .. "/public/tier0",
		path .. "/public/tier1"
	})
	vpaths({["Source files"] = path .. "/**.cpp"})

	local curfilter = GetFilter()
	local nosystem = curfilter.system == nil

	if nosystem or HasFilter(FILTER_WINDOWS) then
		filter({"system:windows", curfilter.configurations})
			files({path .. "/public/tier0/memoverride.cpp"})
			libdirs({path .. "/lib/public"})
			links({"ws2_32", "tier0", "tier1", "vstdlib"})

			if curfilter.configurations == nil or HasFilter(FILTER_DEBUG) then
				filter({"system:windows", "configurations:Debug"})
					linkoptions({"/NODEFAULTLIB:\"libcmt\""})
			end
	end

	if nosystem or HasFilter(FILTER_LINUX) then
		filter({"system:linux", curfilter.configurations})
			defines({"COMPILER_GCC", "POSIX", "LINUX", "_LINUX", "GNUC", "NO_MALLOC_OVERRIDE"})
			libdirs({path .. "/lib/public/linux32"})
			if _PROJECT_SERVERSIDE then
				links({"tier0_srv", "vstdlib_srv"})
			else
				links({"tier0", "vstdlib"})
			end
			linkoptions({path .. "/lib/public/linux32/tier1.a"})
	end

	if nosystem or HasFilter(FILTER_MACOSX) then
		filter({"system:macosx", curfilter.configurations}) -- should probably be similar to linux
			libdirs({path .. "/lib/public/osx32"})
			links({"tier0", "tier1", "vstdlib"})
	end

	filter(curfilter.patterns)
end