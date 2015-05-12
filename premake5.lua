if _ACTION == nil then
	error("no action (vs20**, gmake or xcode for example) provided")
end

SERVERSIDE = true
CLIENTSIDE = false

FILTER_WINDOWS = "windows"
FILTER_LINUX = "linux"
FILTER_MACOSX = "macosx"

FILTER_DEBUG = "debug"
FILTER_RELEASE = "release"

include("config.lua")
include("premake/lua_shared.lua")
include("premake/detouring.lua")
include("premake/scanning.lua")
include("premake/sourcesdk.lua")

newoption({
	trigger = "solution",
	description = "Sets the path for the solution directory",
	value = "path for solution directory"
})

function CleanPath(p)
	if p == nil then
		return
	end

	local last = p:sub(-1)
	if last == "/" or last == "\\" then
		p = p:sub(1, -2)
	end

	return p
end

_GARRYSMOD_COMMON_FOLDER = CleanPath(path.getrelative(_MAIN_SCRIPT_DIR, _SCRIPT_DIR))

function CreateSolution(name, solutionpath)
	SetFilter()

	_SOLUTION_NAME = name
	_SOLUTION_FOLDER = solutionpath or _OPTIONS["solution"] or DEFAULT_SOLUTION_FOLDER

	if _SOLUTION_FOLDER == nil then
		error("you didn't supply a path for your solution folder")
	end

	_SOLUTION_FOLDER = CleanPath(_SOLUTION_FOLDER)

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
	SetFilter()

	sourcepath = sourcepath or _OPTIONS["source"] or DEFAULT_SOURCE_FOLDER

	if sourcepath == nil then
		error("you didn't supply a path to your source folder")
	end

	sourcepath = CleanPath(sourcepath)

	_PROJECT_NAME = (is_server and "gmsv_" or "gmcl_") .. _SOLUTION_NAME
	_PROJECT_SERVERSIDE = is_server

	project(_PROJECT_NAME)
		kind("SharedLib")
		defines({
			"GMMODULE",
			string.upper(_SOLUTION_NAME) .. (_PROJECT_SERVERSIDE and "_SERVER" or "_CLIENT"),
			"IS_SERVERSIDE=" .. tostring(is_server)
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
			["Header files/*"] = {
				sourcepath .. "/**.h",
				sourcepath .. "/**.hpp",
				sourcepath .. "/**.hxx"
			},
			["Source files/*"] = {
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