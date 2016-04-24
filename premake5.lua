if _ACTION == nil then
	error("no action (vs20**, gmake or xcode for example) provided")
end

SERVERSIDE = true
CLIENTSIDE = false

SOURCES_MANUAL = true
SOURCES_ALL = false

include("config.lua")
include("premake/lua_shared.lua")
include("premake/detouring.lua")
include("premake/scanning.lua")
include("premake/sourcesdk.lua")
include("premake/pkg_config.lua")

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

_GARRYSMOD_COMMON_DIRECTORY = CleanPath(path.getrelative(_MAIN_SCRIPT_DIR, _SCRIPT_DIR))

function CreateSolution(name, solutionpath)
	local directory = solutionpath or _OPTIONS["solution"] or DEFAULT_SOLUTION_DIRECTORY
	if directory == nil then
		error("you didn't supply a path for your solution directory")
	end

	directory = CleanPath(directory)

	local solution = solution(name)
	if solution.gmcommon ~= nil then
		error("a solution with this name ('" .. name .. "') already exists")
	end

	solution.gmcommon = {}
	_SOLUTION = solution.gmcommon
	_SOLUTION.name = name
	_SOLUTION.directory = directory

		language("C++")
		location(_SOLUTION.directory)
		warnings("Extra")
		flags({"NoPCH", "StaticRuntime"})
		platforms("x86")
		configurations({"Release", "Debug"})

		filter("platforms:x86")
			architecture("x32")

		filter("configurations:Release")
			optimize("On")
			vectorextensions("SSE2")
			objdir(_SOLUTION.directory .. "/intermediate")
			targetdir(_SOLUTION.directory .. "/release")

		filter("configurations:Debug")
			flags("Symbols")
			defines({"DEBUG", "_DEBUG"})
			objdir(_SOLUTION.directory .. "/intermediate")
			targetdir(_SOLUTION.directory .. "/debug")

		filter("system:linux or macosx")
			linkoptions({"-static-libgcc", "-static-libstdc++"})

		filter({})
end

newoption({
	trigger = "source",
	description = "Sets the path to the source directory",
	value = "path to source directory"
})

function CreateProject(is_server, manual_files, sourcepath)
	sourcepath = sourcepath or _OPTIONS["source"] or DEFAULT_SOURCE_DIRECTORY
	if sourcepath == nil then
		error("you didn't supply a path to your source folder")
	end

	local name = (is_server and "gmsv_" or "gmcl_") .. _SOLUTION.name
	local project = project(name)
	if project.gmcommon ~= nil then
		error("a project with this name ('" .. name .. "') already exists")
	end

	project.gmcommon = {}
	_PROJECT = project.gmcommon
	_PROJECT.name = name
	_PROJECT.directory = CleanPath(sourcepath)
	_PROJECT.serverside = is_server
	_PROJECT.packages = {}

		kind("SharedLib")
		defines({
			"GMMODULE",
			string.upper(string.gsub(_SOLUTION.name, "%.", "_")) .. (_PROJECT.serverside and "_SERVER" or "_CLIENT"),
			"IS_SERVERSIDE=" .. tostring(is_server)
		})
		includedirs({
			_PROJECT.directory,
			_GARRYSMOD_COMMON_DIRECTORY .. "/include"
		})

		if not manual_files then
			files({
				_PROJECT.directory .. "/*.h",
				_PROJECT.directory .. "/*.hpp",
				_PROJECT.directory .. "/*.hxx",
				_PROJECT.directory .. "/*.c",
				_PROJECT.directory .. "/*.cpp",
				_PROJECT.directory .. "/*.cxx"
			})
		end

		vpaths({
			["Header files/*"] = {
				_PROJECT.directory .. "/**.h",
				_PROJECT.directory .. "/**.hpp",
				_PROJECT.directory .. "/**.hxx"
			},
			["Source files/*"] = {
				_PROJECT.directory .. "/**.c",
				_PROJECT.directory .. "/**.cpp",
				_PROJECT.directory .. "/**.cxx"
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
			buildoptions("-std=c++11")

		filter({})
end

function HasIncludedPackage(name)
	local project = project()
	project.packages = project.packages or {}
	return project.packages[name] == true
end

function IncludePackage(name)
	if HasIncludedPackage(name) then
		error("this package ('" .. name .. "') was already included")
	end

	project().packages[name] = true
end
