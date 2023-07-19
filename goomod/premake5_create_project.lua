group("garrysmod_common")
	project("goomod")
		kind("StaticLib")
		location("../projects/" .. os.target() .. "/" .. _ACTION)
		targetdir("%{prj.location}/%{cfg.architecture}/%{cfg.buildcfg}")
		debugdir("%{prj.location}/%{cfg.architecture}/%{cfg.buildcfg}")
		objdir("!%{prj.location}/%{cfg.architecture}/%{cfg.buildcfg}/intermediate/%{prj.name}")
		defines({"IS_SERVERSIDE=true", "GMOD_ALLOW_LIGHTUSERDATA"})
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

		IncludeHelpers()
