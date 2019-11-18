function IncludeDetouring()
	IncludePackage("detouring")

	local directory = path.join(_GARRYSMOD_COMMON_DIRECTORY, "include", "detouring")

	local _project = project()
	local _workspace = _project.workspace

	sysincludedirs(path.join(_GARRYSMOD_COMMON_DIRECTORY, "include"))
	links("detouring")

	filter("system:macosx")
		links("CoreServices.framework")

	group("garrysmod_common")
		project("detouring")
			kind("StaticLib")
			location(path.join(_GARRYSMOD_COMMON_DIRECTORY, "projects", os.target(), _ACTION))
			targetdir(path.join("%{prj.location}", "%{cfg.architecture}", "%{cfg.buildcfg}"))
			debugdir(path.join("%{prj.location}", "%{cfg.architecture}", "%{cfg.buildcfg}"))
			objdir(path.join("!%{prj.location}", "%{cfg.architecture}", "%{cfg.buildcfg}", "intermediate", "%{prj.name}"))
			includedirs(directory)
			files({
				path.join(directory, "*.hpp"),
				path.join(directory, "*.h"),
				path.join(directory, "*.cpp"),
				path.join(directory, "hde", "include", "*.h"),
				path.join(directory, "hde", "src", "hde.c"),
				path.join(directory, "minhook", "include", "*.h"),
				path.join(directory, "minhook", "src", "*.h"),
				path.join(directory, "minhook", "src", "*.c")
			})
			vpaths({
				["Header files"] = {
					path.join(directory, "*.hpp"),
					path.join(directory, "*.h")
				},
				["Header files/hde"] = path.join(directory, "hde", "include", "*.h"),
				["Header files/minhook"] = {
					path.join(directory, "minhook", "include", "*.h"),
					path.join(directory, "minhook", "src", "*.h")
				},
				["Source files"] = path.join(directory, "*.cpp"),
				["Source files/hde"] = path.join(directory, "hde", "src", "hde.c"),
				["Source files/minhook"] = path.join(directory, "minhook", "src", "*.c")
			})

			filter("files:**.c")
				language("C")

	group("")
	project(_project.name)
end
