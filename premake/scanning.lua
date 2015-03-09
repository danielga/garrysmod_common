function IncludeScanning(path)
	path = path or "../scanning"

	includedirs({path})
	files({
		path .. "/*.hpp",
		path .. "/*.cpp"
	})
	vpaths({
		["Header files"] = path .. "/**.hpp",
		["Source files"] = path .. "/**.cpp"
	})

	local curfilter = GetFilter()
	local nosystem = curfilter.system == nil

	if nosystem or HasFilter(FILTER_LINUX) then
		filter({"system:linux", curfilter.configurations})
			links({"dl"})
	end

	if nosystem or HasFilter(FILTER_MACOSX) then
		filter({"system:macosx", curfilter.configurations})
			links({"dl"})
	end

	filter(curfilter.patterns)
end