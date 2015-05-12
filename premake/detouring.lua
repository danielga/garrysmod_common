function IncludeDetouring()
	local folder = _GARRYSMOD_COMMON_FOLDER .. "/detouring"

	includedirs({folder})
	files({
		folder .. "/*.hpp",
		folder .. "/*.cpp"
	})
	vpaths({
		["Header files"] = folder .. "/**.hpp",
		["Source files"] = folder .. "/**.cpp"
	})
end