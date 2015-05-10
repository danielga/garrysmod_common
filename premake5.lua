SERVERSIDE = true
CLIENTSIDE = false

FILTER_WINDOWS = "windows"
FILTER_LINUX = "linux"
FILTER_MACOSX = "macosx"

FILTER_DEBUG = "debug"
FILTER_RELEASE = "release"

_GARRYSMOD_COMMON_FOLDER = path.getrelative(_MAIN_SCRIPT_DIR, _SCRIPT_DIR)

include("premake/lua_shared.lua")
include("premake/sourcesdk.lua")
include("premake/detouring.lua")
include("premake/scanning.lua")

newoption({
	trigger = "solution",
	description = "Sets the path for the solution directory",
	value = "path for solution directory"
})

function CreateSolution(name, solutionpath)
	SetFilter()
	
	_SOLUTION_NAME = name
	_SOLUTION_FOLDER = solutionpath or _OPTIONS["solution"] or (os.get() .. "/" .. _ACTION)

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

newoption({
	trigger = "source",
	description = "Sets the path to the source directory",
	value = "path to source directory"
})

function CreateProject(is_server, sourcepath)
	sourcepath = sourcepath or _OPTIONS["source"] or "../source"

	SetFilter()

	_PROJECT_NAME = (is_server and "gmsv_" or "gmcl_") .. _SOLUTION_NAME
	_PROJECT_SERVERSIDE = is_server

	project(_PROJECT_NAME)
		kind("SharedLib")
		defines({
			"GMMODULE",
			string.upper(_SOLUTION_NAME) .. (_PROJECT_SERVERSIDE and "_SERVER" or "_CLIENT")
		})
		includedirs({_GARRYSMOD_COMMON_FOLDER .. "/include"})
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
			targetsuffix("_linux")

		filter("system:macosx")
			targetsuffix("_mac")

		filter("action:gmake")
			buildoptions({"-std=c++11"})

		filter({})
end

function HasFilter(filter)
	if _CURRENT_FILTER ~= nil then
		for i = 1, #_CURRENT_FILTER.list do
			if _CURRENT_FILTER.list[i] == filter then
				return true
			end
		end
	end

	return false
end

function GetFilter()
	return _CURRENT_FILTER or {}
end

function SetFilter(...)
	local list = {...}
	local sys, config
	for i = 1, #list do
		if list[i] == FILTER_WINDOWS or list[i] == FILTER_LINUX or list[i] == FILTER_MACOSX then
			if sys ~= nil then
				sys = sys .. " or " .. list[i]
			else
				sys = "system:" .. list[i]
			end
		elseif list[i] == FILTER_DEBUG or list[i] == FILTER_RELEASE then
			if config ~= nil then
				config = config .. " or " .. list[i]
			else
				config = "configurations:" .. list[i]
			end
		end
	end

	local patterns = {sys, config}
	_CURRENT_FILTER = {system = sys, configurations = config, list = list, patterns = patterns}
	filter(patterns)
end