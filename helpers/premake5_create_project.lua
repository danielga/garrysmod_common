group("garrysmod_common")
	project("helpers")
		kind("StaticLib")
		location("../projects/" .. os.target() .. "/" .. _ACTION)
		targetdir("%{prj.location}/%{cfg.architecture}/%{cfg.buildcfg}")
		debugdir("%{prj.location}/%{cfg.architecture}/%{cfg.buildcfg}")
		objdir("!%{prj.location}/%{cfg.architecture}/%{cfg.buildcfg}/intermediate/%{prj.name}")
		includedirs({"../include", "include", "include/GarrysMod"})
		files({
			"../include/Platform.hpp",
			"include/**.h",
			"include/**.hpp",
			"source/*.cpp"
		})
		vpaths({
			["Header files/*"] = {"../include/**.hpp", "include/**.h", "include/**.hpp"},
			["Source files/*"] = "source/*.cpp"
		})
