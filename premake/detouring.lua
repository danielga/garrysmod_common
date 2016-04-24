function IncludeDetouring()
	IncludePackage("detouring")

	local directory = _GARRYSMOD_COMMON_DIRECTORY .. "/detouring"

	filter({})

	includedirs(directory)
	files({
		directory .. "/*.hpp",
		directory .. "/*.cpp"
	})
	vpaths({
		["Header files/*"] = directory .. "/*.hpp",
		["Source files/*"] = directory .. "/*.cpp"
	})
end
