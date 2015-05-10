newoption({
	trigger = "scanning",
	description = "Sets the path to the scanning directory",
	value = "path to scanning directory"
})

function IncludeScanning(folder)
	folder = folder or _OPTIONS["scanning"] or os.getenv("SCANNING") or "../scanning"

	local dir = path.getabsolute(folder)
	if not os.isdir(dir) then
		error(dir .. " doesn't exist (scanning)")
	end

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
		filter({"system:linux", curfilter.configurations})
			links({"dl"})
	end

	if nosystem or HasFilter(FILTER_MACOSX) then
		filter({"system:macosx", curfilter.configurations})
			links({"dl"})
	end

	filter(curfilter.patterns)
end