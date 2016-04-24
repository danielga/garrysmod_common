function IncludeDetouring()
	IncludePackage("detouring")

	local folder = _GARRYSMOD_COMMON_DIRECTORY .. "/detouring"

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
end
