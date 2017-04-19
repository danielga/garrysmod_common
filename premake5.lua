if _ACTION == nil then
	error("no action (vs20**, gmake or xcode for example) provided")
end

-- START OF NEW FLAGS BLOCK (C11, GNU11, GNU++11, GNU++14)

table.insert(premake.field._list.flags.allowed, "C11")
table.insert(premake.field._list.flags.allowed, "GNU11")
table.insert(premake.field._list.flags.allowed, "GNU++11")
table.insert(premake.field._list.flags.allowed, "GNU++14")
premake.field._list.flags.allowed["c11"] = "C11"
premake.field._list.flags.allowed["gnu11"] = "GNU11"
premake.field._list.flags.allowed["gnu++11"] = "GNU++11"
premake.field._list.flags.allowed["gnu++14"] = "GNU++14"

table.insert(premake.field._loweredList.flags.allowed, "C11")
table.insert(premake.field._loweredList.flags.allowed, "GNU11")
table.insert(premake.field._loweredList.flags.allowed, "GNU++11")
table.insert(premake.field._loweredList.flags.allowed, "GNU++14")
premake.field._loweredList.flags.allowed["c11"] = "C11"
premake.field._loweredList.flags.allowed["gnu11"] = "GNU11"
premake.field._loweredList.flags.allowed["gnu++11"] = "GNU++11"
premake.field._loweredList.flags.allowed["gnu++14"] = "GNU++14"

table.insert(premake.fields.flags.allowed, "C11")
table.insert(premake.fields.flags.allowed, "GNU11")
table.insert(premake.fields.flags.allowed, "GNU++11")
table.insert(premake.fields.flags.allowed, "GNU++14")
premake.fields.flags.allowed["c11"] = "C11"
premake.fields.flags.allowed["gnu11"] = "GNU11"
premake.fields.flags.allowed["gnu++11"] = "GNU++11"
premake.fields.flags.allowed["gnu++14"] = "GNU++14"

if premake.tools.clang.cflags and premake.tools.clang.cflags.flags then
	premake.tools.clang.cflags.flags["C11"] = "-std=c11"
end

if premake.tools.clang.cflags and premake.tools.clang.cflags.flags then
	premake.tools.clang.cflags.flags["GNU11"] = "-std=gnu11"
end

if premake.tools.clang.cxxflags and premake.tools.clang.cxxflags.flags then
	premake.tools.clang.cxxflags.flags["GNU++11"] = "-std=gnu++11"
end

if premake.tools.clang.cxxflags and premake.tools.clang.cxxflags.flags then
	premake.tools.clang.cxxflags.flags["GNU++14"] = "-std=gnu++14"
end

if premake.tools.gcc.cflags and premake.tools.gcc.cflags.flags then
	premake.tools.gcc.cflags.flags["C11"] = "-std=c11"
end

if premake.tools.gcc.cflags and premake.tools.gcc.cflags.flags then
	premake.tools.gcc.cflags.flags["GNU11"] = "-std=gnu11"
end

if premake.tools.gcc.cxxflags and premake.tools.gcc.cxxflags.flags then
	premake.tools.gcc.cxxflags.flags["GNU++11"] = "-std=gnu++11"
end

if premake.tools.gcc.cxxflags and premake.tools.gcc.cxxflags.flags then
	premake.tools.gcc.cxxflags.flags["GNU++14"] = "-std=gnu++14"
end

-- END OF NEW FLAGS BLOCK (C11, GNU11, GNU++11, GNU++14)

include("config.lua")
include("premake/lua_shared.lua")
include("premake/detouring.lua")
include("premake/scanning.lua")
include("premake/sourcesdk.lua")
include("premake/pkg_config.lua")

newoption({
	trigger = "workspace",
	description = "Sets the path for the workspace directory",
	value = "path for workspace directory"
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

_GARRYSMOD_COMMON_DIRECTORY = CleanPath(_SCRIPT_DIR)

function CreateWorkspace(config)
	if type(config) ~= "table" then
		error("supplied argument is not a table")
	end

	local name = config.name
	if name == nil then
		error("you didn't supply a name for your workspace")
	end

	local directory = config.path or _OPTIONS["workspace"] or DEFAULT_WORKSPACE_DIRECTORY
	if directory == nil then
		error("you didn't supply a path for your workspace directory")
	end

	directory = CleanPath(directory)

	local allowdebug = config.allow_debug
	if allowdebug == nil then
		allowdebug = true
	end

	local _workspace = workspace(name)
	if _workspace.directory ~= nil then
		error("a workspace with this name ('" .. name .. "') already exists")
	end

	_workspace.directory = directory

		language("C++")
		location(_workspace.directory)
		warnings("Extra")
		flags({"NoPCH", "StaticRuntime"})
		characterset("MBCS")
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
			objdir(_workspace.directory .. "/intermediate")
			targetdir(_workspace.directory .. "/release")

		filter("configurations:Debug")
			symbols("On")
			defines({"DEBUG", "_DEBUG"})
			objdir(_workspace.directory .. "/intermediate")
			targetdir(_workspace.directory .. "/debug")

		filter("system:linux")
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

	local _workspace = workspace()

	local name = (is_server and "gmsv_" or "gmcl_") .. _workspace.name
	local _project = project(name)
	if _project.directory ~= nil then
		error("a project with this name ('" .. name .. "') already exists")
	end

	_project.directory = CleanPath(sourcepath)
	_project.serverside = is_server

		kind("SharedLib")
		language("C++11")
		defines({
			"GMMODULE",
			string.upper(string.gsub(_workspace.name, "%.", "_")) .. (_project.serverside and "_SERVER" or "_CLIENT"),
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
			targetsuffix("_osx")

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
