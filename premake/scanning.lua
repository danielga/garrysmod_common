function IncludeScanning()
	local folder = _GARRYSMOD_COMMON_FOLDER .. "/scanning"

	includedirs({folder})
	files({
		folder .. "/*.hpp",
		folder .. "/*.cpp"
	})
	vpaths({
		["Header files"] = folder .. "/**.hpp",
		["Source files"] = folder .. "/**.cpp"
	})

	local curfilter = GetFilter()
	local nosystem = curfilter.system == nil

	if nosystem or HasFilter(FILTER_LINUX) then
		filter(MergeFilters({"system:linux", curfilter.configurations}, curfilter.extra))
			links({"dl"})
	end

	if nosystem or HasFilter(FILTER_MACOSX) then
		filter(MergeFilters({"system:macosx", curfilter.configurations}, curfilter.extra))
			links({"dl"})
	end

	filter(curfilter.patterns)
end