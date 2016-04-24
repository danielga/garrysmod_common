function IncludeScanning()
	IncludePackage("scanning")

	local folder = _GARRYSMOD_COMMON_DIRECTORY .. "/scanning"

	filter({})

	includedirs(folder)
	files({
		folder .. "/*.hpp",
		folder .. "/*.cpp"
	})
	vpaths({
		["Header files"] = folder .. "/**.hpp",
		["Source files"] = folder .. "/**.cpp"
	})

	filter("system:linux or macosx")
		links("dl")

	filter({})
end
