function IncludeDetouring()
	IncludePackage("detouring")

	local directory = _GARRYSMOD_COMMON_DIRECTORY .. "/detouring"

	local _project = project()
	local _workspace = _project.workspace
	local _project_directory = _GARRYSMOD_COMMON_DIRECTORY .. "/projects/" .. os.target() .. "/" .. _ACTION

	includedirs(directory)
	links("detouring")

	project("detouring")
		kind("StaticLib")
		language("C")
		location(_project_directory)
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

		filter("configurations:Release")
			objdir(_project_directory .. "/intermediate")
			targetdir(_project_directory .. "/release")

		if not _workspace.abi_compatible then
			filter("configurations:Debug")
				objdir(_project_directory .. "/intermediate")
				targetdir(_project_directory .. "/debug")
		end

	project(_project.name)
end
