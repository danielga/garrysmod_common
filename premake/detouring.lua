function IncludeDetouring(path)
	path = path or "../detouring"

	includedirs({path})
	files({
		path .. "/*.hpp",
		path .. "/*.cpp"
	})
	vpaths({
		["Header files"] = path .. "/**.hpp",
		["Source files"] = path .. "/**.cpp"
	})
end