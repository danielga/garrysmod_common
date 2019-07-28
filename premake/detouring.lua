function IncludeDetouring()
	IncludePackage("detouring")

	local directory = path.join(_GARRYSMOD_COMMON_DIRECTORY, "include/detouring")

	local _project = project()
	local _workspace = _project.workspace

	sysincludedirs(path.join(_GARRYSMOD_COMMON_DIRECTORY, "include"))
	links("detouring")

	filter("system:macosx")
		links("CoreServices.framework")

	project("detouring")
		kind("StaticLib")
		location(path.join(_GARRYSMOD_COMMON_DIRECTORY, "projects", os.target(), _ACTION))
		sysincludedirs(directory)
		files({
			path.join(directory, "*.hpp"),
			path.join(directory, "*.h"),
			path.join(directory, "*.cpp"),
			path.join(directory, "hde/include/*.h"),
			path.join(directory, "hde/src/hde.c"),
			path.join(directory, "minhook/include/*.h"),
			path.join(directory, "minhook/src/*.h"),
			path.join(directory, "minhook/src/*.c")
		})
		vpaths({
			["Header files"] = {
				path.join(directory, "*.hpp"),
				path.join(directory, "*.h")
			},
			["Header files/hde"] = path.join(directory, "hde/include/*.h"),
			["Header files/minhook"] = {
				path.join(directory, "minhook/include/*.h"),
				path.join(directory, "minhook/src/*.h")
			},
			["Source files"] = path.join(directory, "*.cpp"),
			["Source files/hde"] = path.join(directory, "hde/src/hde.c"),
			["Source files/minhook"] = path.join(directory, "minhook/src/*.c")
		})

		filter("files:**.c")
			language("C")

		filter("configurations:Release")
			objdir("%{prj.location}/intermediate")
			targetdir("%{prj.location}/release")

		if not _workspace.abi_compatible then
			filter("configurations:Debug")
				objdir("%{prj.location}/intermediate")
				targetdir("%{prj.location}/debug")
		end

	project(_project.name)
end
