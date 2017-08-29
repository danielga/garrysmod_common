function IncludeDetouring()
	IncludePackage("detouring")

	local directory = _GARRYSMOD_COMMON_DIRECTORY .. "/detouring"

	local _project = project()

	includedirs(directory)
	links("detouring")

	project("detouring")
		kind("StaticLib")
		language("C")
		includedirs(directory)
		files({
			directory .. "/**.hpp",
			directory .. "/**.h",
			directory .. "/hde/src/hde.c",
			directory .. "/minhook/src/*.h",
			directory .. "/minhook/src/*.c"
		})
		vpaths({
			["Header files"] = {
				directory .. "/*.hpp",
				directory .. "/*.h"
			},
			["Header files/hde"] = directory .. "/hde/include/*.h",
			["Header files/minhook"] = {
				directory .. "/minhook/include/*.h",
				directory .. "/minhook/src/*.h"
			},
			["Source files/hde"] = directory .. "/hde/src/hde.c",
			["Source files/minhook"] = directory .. "/minhook/src/*.c"
		})

	project(_project.name)
end
