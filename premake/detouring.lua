function IncludeDetouring()
	IncludePackage("detouring")

	local directory = _GARRYSMOD_COMMON_DIRECTORY .. "/detouring"

	filter({})

	includedirs(directory)
	files({directory .. "/*.h", directory .. "/*.cpp"})
	vpaths({
		["Header files/garrysmod_common/detouring"] = directory .. "/*.h",
		["Source files/garrysmod_common/detouring"] = directory .. "/*.cpp"
	})
end
