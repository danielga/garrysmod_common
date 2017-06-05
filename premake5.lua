assert(_ACTION ~= nil, "no action (vs20**, gmake or xcode for example) provided!")

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
	assert(type(config) == "table", "supplied argument is not a table!")

	local name = config.name
	assert(type(name) == "string", "'name' is not a string!")

	local directory = config.path or _OPTIONS["workspace"] or DEFAULT_WORKSPACE_DIRECTORY
	assert(type(directory) == "string", "workspace path is not a string!")

	directory = CleanPath(directory)

	local allow_debug = config.allow_debug
	if allow_debug == nil then
		allow_debug = true
	else
		assert(type(allow_debug) == "boolean", "'allow_debug' is not a boolean!")
		print("WARNING: The 'allow_debug' option has been deprecated in favor of 'abi_compatible' (same functionality, better name, takes precedence over 'allow_debug')")
		abi_compatible = not allow_debug
	end

	local abi_compatible = config.abi_compatible
	if abi_compatible == nil then
		abi_compatible = false
	else
		assert(type(abi_compatible) == "boolean", "'abi_compatible' is not a boolean!")
		allow_debug = not abi_compatible
	end

	if abi_compatible then
		if os.istarget("windows") and _ACTION ~= "vs2010" then
			error("The only supported compilation platform for this project on Windows is Visual Studio 2010.")
		elseif os.istarget("linux") then
			print("WARNING: The only supported compilation platforms (tested) for this project on Linux are GCC/G++ 4.8 or 4.9. However, any version between 4.4 and 4.9 *MIGHT* work.")
		elseif os.istarget("macosx") then
			print("WARNING: The only supported compilation platform (tested) for this project on Mac OSX is Xcode 4.1 (GCC/G++ compiler). However, any Xcode version *MIGHT* work as long as the Mac OSX 10.5 SDK is used (-mmacosx-version-min=10.5).")
		end
	end

	local _workspace = workspace(name)
	assert(_workspace.directory == nil, "a workspace with the name '" .. name .. "' already exists!")

	_workspace.directory = directory

		language("C++")
		location(_workspace.directory)
		warnings("Extra")
		flags({"NoPCH", "StaticRuntime"})
		characterset("MBCS")
		platforms("x86")

		if allow_debug then
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

		if abi_compatible then
			filter("system:macosx")
				buildoptions("-mmacosx-version-min=10.5")
				linkoptions("-mmacosx-version-min=10.5")
		end

		filter({})
end

newoption({
	trigger = "source",
	description = "Sets the path to the source directory",
	value = "path to source directory"
})

function CreateProject(config)
	assert(type(config) == "table", "supplied argument is not a table!")

	local is_server = config.serverside
	assert(type(is_server) == "boolean", "'serverside' option is not a boolean!")

	local sourcepath = config.source_path or _OPTIONS["source"] or DEFAULT_SOURCE_DIRECTORY
	assert(type(sourcepath) == "string", "source code path is not a string!")

	local manual_files = config.manual_files
	if manual_files == nil then
		manual_files = false
	else
		assert(type(manual_files) == "boolean", "'manual_files' is not a boolean!")
	end

	local _workspace = workspace()

	local name = (is_server and "gmsv_" or "gmcl_") .. _workspace.name
	local _project = project(name)

	assert(_project.directory == nil, "a project with the name '" .. name .. "' already exists!")

	_project.directory = CleanPath(sourcepath)
	_project.serverside = is_server

		kind("SharedLib")
		language("C++")
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
	assert(not HasIncludedPackage(name), "a project with the name '" .. name .. "' already exists!")
	project().packages[name] = true
end
