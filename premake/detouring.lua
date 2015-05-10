newoption({
	trigger = "detouring",
	description = "Sets the path to the detouring directory",
	value = "path to detouring directory"
})

function IncludeDetouring(folder)
	folder = folder or _OPTIONS["detouring"] or os.getenv("DETOURING") or "../detouring"

	local dir = path.getabsolute(folder)
	if not os.isdir(dir) then
		error(dir .. " doesn't exist (detouring)")
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
end