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

	filter("system:linux")
		links({"dl"})

	filter("system:macosx")
		links({"dl"})

	filter({})
end