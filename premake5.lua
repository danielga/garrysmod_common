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

	local _workspace = workspace(name)
	assert(_workspace.directory == nil, "a workspace with the name '" .. name .. "' already exists!")

	local abi_compatible
	if config.allow_debug ~= nil then
		assert(type(config.allow_debug) == "boolean", "'allow_debug' is not a boolean!")
		print("WARNING: The 'allow_debug' option has been deprecated in favor of 'abi_compatible' (same functionality, better name, takes precedence over 'allow_debug', allows setting per project where the workspace setting takes precedence if set to true)")
		abi_compatible = not config.allow_debug
	end

	if config.abi_compatible ~= nil then
		abi_compatible = config.abi_compatible
		assert(type(abi_compatible) == "boolean", "'abi_compatible' is not a boolean!")
		_workspace.abi_compatible = abi_compatible
	end

	_workspace.directory = directory

		language("C++")
		cppdialect("C++11")
		location(_workspace.directory)
		warnings("Extra")
		flags({"NoPCH", "StaticRuntime"})
		characterset("MBCS")
		platforms("x86")
		architecture("x32")

		if abi_compatible then
			configurations("Release")
		else
			configurations({"Release", "Debug"})
		end

		filter("configurations:Release")
			optimize("On")
			vectorextensions("SSE2")
			defines("NDEBUG")
			objdir(_workspace.directory .. "/intermediate")
			targetdir(_workspace.directory .. "/release")

		if not abi_compatible then
			filter("configurations:Debug")
				symbols("On")
				defines({"DEBUG", "_DEBUG"})
				objdir(_workspace.directory .. "/intermediate")
				targetdir(_workspace.directory .. "/debug")
		end
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

	local abi_compatible = _workspace.abi_compatible
	if not abi_compatible then
		if config.abi_compatible ~= nil then
			abi_compatible = config.abi_compatible
			assert(type(abi_compatible) == "boolean", "'abi_compatible' is not a boolean!")
		else
			abi_compatible = false
		end
	end

	local name = (is_server and "gmsv_" or "gmcl_") .. _workspace.name

	if abi_compatible then
		if os.istarget("windows") and _ACTION ~= "vs2017" then
			error("The only supported compilation platform for this project (" .. name .. ") on Windows is Visual Studio 2017.")
		elseif os.istarget("linux") then
			print("WARNING: The only supported compilation platforms (tested) for this project (" .. name .. ") on Linux are GCC/G++ 4.8 or 4.9. However, any version between 4.4 and 4.9 *MIGHT* work.")
		elseif os.istarget("macosx") then
			print("WARNING: The only supported compilation platform (tested) for this project (" .. name .. ") on Mac OSX is Xcode 4.1 (GCC/G++ compiler). However, any Xcode version *MIGHT* work as long as the Mac OSX 10.5 SDK is used (-mmacosx-version-min=10.5).")
		end
	end

	local _project = project(name)

	assert(_project.directory == nil, "a project with the name '" .. name .. "' already exists!")

	_project.directory = CleanPath(sourcepath)
	_project.serverside = is_server

		if abi_compatible then
			removeconfigurations("Debug")
			configurations("Release")
		else
			configurations({"Release", "Debug"})
		end

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

		if abi_compatible then
			files(_GARRYSMOD_COMMON_DIRECTORY .. "/ABICompatibility.cpp")
			vpaths({["Source files/garrysmod_common"] = _GARRYSMOD_COMMON_DIRECTORY .. "/ABICompatibility.cpp"})
		end

		targetprefix("")
		targetextension(".dll")

		filter("system:windows")
			targetsuffix("_win32")

		filter("system:linux")
			targetsuffix("_linux")

		filter("system:macosx")
			targetsuffix("_osx")

			if abi_compatible then
				buildoptions("-mmacosx-version-min=10.5")
				linkoptions("-mmacosx-version-min=10.5")
			end

		filter("system:linux")
			linkoptions({"-static-libgcc", "-static-libstdc++"})

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
