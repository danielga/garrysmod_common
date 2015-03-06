include("premake/lua_shared.lua")
include("premake/sourcesdk.lua")
include("premake/detouring.lua")
include("premake/scanning.lua")

function CreateSolution(name)
	_SOLUTION_NAME = name
	_SOLUTION_FOLDER = os.get() .. "/" .. _ACTION

	solution(name)
		language("C++")
		location(_SOLUTION_FOLDER)
		warnings("Extra")
		flags({"NoPCH", "StaticRuntime"})
		platforms({"x86"})
		configurations({"Release", "Debug"})

		filter("platforms:x86")
			architecture("x32")

		filter("configurations:Release")
			optimize("On")
			vectorextensions("SSE2")
			objdir(_SOLUTION_FOLDER .. "/intermediate")
			targetdir(_SOLUTION_FOLDER .. "/release")

		filter("configurations:Debug")
			flags({"Symbols"})
			objdir(_SOLUTION_FOLDER .. "/intermediate")
			targetdir(_SOLUTION_FOLDER .. "/debug")

		filter({})
end

function CreateProject(sourcepath, is_server)
	local statedefine
	if _PROJECT_NAME then
		statedefine = _SOLUTION_NAME .. (is_server and "_SERVER" or "_CLIENT")
		project(_PROJECT_NAME)
			defines({string.upper(_SOLUTION_NAME) .. (_PROJECT_SERVERSIDE and "_SERVER" or "_CLIENT")})
	end

	_PROJECT_NAME = (is_server and "gmsv_" or "gmcl_") .. _SOLUTION_NAME
	_PROJECT_SERVERSIDE = is_server

	project(_PROJECT_NAME)
		kind("SharedLib")
		defines({"GMMODULE", statedefine})
		files({
			sourcepath .. "/**.h",
			sourcepath .. "/**.hpp",
			sourcepath .. "/**.hxx",
			sourcepath .. "/**.c",
			sourcepath .. "/**.cpp",
			sourcepath .. "/**.cxx"
		})
		vpaths({
			["Header files"] = {
				sourcepath .. "/**.h",
				sourcepath .. "/**.hpp",
				sourcepath .. "/**.hxx"
			},
			["Source files"] = {
				sourcepath .. "/**.c",
				sourcepath .. "/**.cpp",
				sourcepath .. "/**.cxx"
			}
		})

		targetprefix("")
		targetextension(".dll")

		filter("system:windows")
			targetsuffix("_win32")

		filter("system:linux")
			buildoptions({"-std=c++11"})
			targetsuffix("_linux")

		filter({"system:macosx"})
			buildoptions({"-std=c++11"})
			targetsuffix("_mac")

		filter({})
end