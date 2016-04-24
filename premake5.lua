if _ACTION == nil then
	error("no action (vs20**, gmake or xcode for example) provided")
end

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

function CreateSolution(config)
	if type(config) ~= "table" then
		error("supplied argument is not a table")
	end

	local name = config.name
	if name == nil then
		error("you didn't supply a name for your solution")
	end

	local directory = config.path or _OPTIONS["solution"] or DEFAULT_SOLUTION_DIRECTORY
	if directory == nil then
		error("you didn't supply a path for your solution directory")
	end

	directory = CleanPath(directory)

	local allowdebug = config.allow_debug
	if allowdebug == nil then
		allowdebug = true
	end

	local _solution = solution(name)
	if _solution.directory ~= nil then
		error("a solution with this name ('" .. name .. "') already exists")
	end

	_solution.directory = directory

		language("C++")
		location(_solution.directory)
		warnings("Extra")
		flags({"NoPCH", "StaticRuntime"})
		platforms("x86")

		if allowdebug then
			configurations({"Release", "Debug"})
		else
			configurations("Release")
		end

		filter("platforms:x86")
			architecture("x32")

		filter("configurations:Release")
			optimize("On")
			vectorextensions("SSE2")
			objdir(_solution.directory .. "/intermediate")
			targetdir(_solution.directory .. "/release")

		filter("configurations:Debug")
			flags("Symbols")
			defines({"DEBUG", "_DEBUG"})
			objdir(_solution.directory .. "/intermediate")
			targetdir(_solution.directory .. "/debug")

		filter("system:linux or macosx")
			linkoptions({"-static-libgcc", "-static-libstdc++"})

		filter({})
end

newoption({
	trigger = "source",
	description = "Sets the path to the source directory",
	value = "path to source directory"
})

function CreateProject(config)
	if type(config) ~= "table" then
		error("supplied argument is not a table")
	end

	local is_server = config.serverside
	if is_server == nil then
		error("you didn't specify if the project is for a serverside module or not")
	end

	local manual_files = config.manual_files
	if manual_files == nil then
		manual_files = false
	end

	local sourcepath = config.source_path or _OPTIONS["source"] or DEFAULT_SOURCE_DIRECTORY
	if sourcepath == nil then
		error("you didn't supply a path to your source directory")
	end

	local _solution = solution()

	local name = (is_server and "gmsv_" or "gmcl_") .. _solution.name
	local _project = project(name)
	if _project.directory ~= nil then
		error("a project with this name ('" .. name .. "') already exists")
	end

	_project.directory = CleanPath(sourcepath)
	_project.serverside = is_server

		kind("SharedLib")
		defines({
			"GMMODULE",
			string.upper(string.gsub(_solution.name, "%.", "_")) .. (_project.serverside and "_SERVER" or "_CLIENT"),
			"IS_SERVERSIDE=" .. tostring(is_server)
		})
		includedirs({
			_project.directory,
			_GARRYSMOD_COMMON_DIRECTORY .. "/include"
		})

		if not manual_files then
			files({
				_project.directory .. "/*.h",
				_project.directory .. "/*.hpp",
				_project.directory .. "/*.hxx",
				_project.directory .. "/*.c",
				_project.directory .. "/*.cpp",
				_project.directory .. "/*.cxx"
			})
		end

		vpaths({
			["Header files/*"] = {
				_project.directory .. "/**.h",
				_project.directory .. "/**.hpp",
				_project.directory .. "/**.hxx"
			},
			["Source files/*"] = {
				_project.directory .. "/**.c",
				_project.directory .. "/**.cpp",
				_project.directory .. "/**.cxx"
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
	local _project = project()
	_project.packages = _project.packages or {}
	return _project.packages[name] == true
end

function IncludePackage(name)
	if HasIncludedPackage(name) then
		error("this package ('" .. name .. "') was already included")
	end

	project().packages[name] = true
end
