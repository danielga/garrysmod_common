assert(_ACTION ~= nil, "no action (vs20**, gmake or xcode for example) provided!")

include("config.lua")

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

includeexternal("premake/lua_shared.lua")
includeexternal("premake/detouring.lua")
includeexternal("premake/scanning.lua")
includeexternal("premake/sourcesdk.lua")
includeexternal("premake/pkg_config.lua")

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
		location(_workspace.directory)
		warnings("Extra")
		flags({"NoPCH", "MultiProcessorCompile"})
		staticruntime("On")
		characterset("MBCS")
		platforms({"x86", "x64"})

		if abi_compatible then
			configurations("Release")

			filter("system:linux or macosx")
				defines("_GLIBCXX_USE_CXX11_ABI=0")
		else
			configurations({"Release", "Debug"})
		end

		filter("platforms:x86")
			architecture("x86")

		filter("platforms:x64")
			architecture("x86_64")

		filter("configurations:Release")
			optimize("On")
			vectorextensions("SSE2")
			defines("NDEBUG")

			filter({"configurations:Release", "architecture:x86"})
				targetdir(_workspace.directory .. "/x86/release")
				debugdir(_workspace.directory .. "/x86/release")
				objdir("!" .. _workspace.directory .. "/x86/release/intermediate/%{prj.name}")

			filter({"configurations:Release", "architecture:x86_64"})
				targetdir(_workspace.directory .. "/x64/release")
				debugdir(_workspace.directory .. "/x64/release")
				objdir("!" .. _workspace.directory .. "/x64/release/intermediate/%{prj.name}")

		if not abi_compatible then
			filter("configurations:Debug")
				symbols("On")
				defines({"DEBUG", "_DEBUG"})

				filter({"configurations:Debug", "architecture:x86"})
					targetdir(_workspace.directory .. "/x86/debug")
					debugdir(_workspace.directory .. "/x86/debug")
					objdir("!" .. _workspace.directory .. "/x86/debug/intermediate/%{prj.name}")

				filter({"configurations:Debug", "architecture:x86_64"})
					targetdir(_workspace.directory .. "/x64/debug")
					debugdir(_workspace.directory .. "/x64/debug")
					objdir("!" .. _workspace.directory .. "/x64/debug/intermediate/%{prj.name}")
		end

		filter("system:windows")
			defines({
				"_CRT_NONSTDC_NO_WARNINGS",
				"_CRT_SECURE_NO_WARNINGS",
				"STRICT"
			})

		filter("system:windows")
			cppdialect("C++17")

		filter("system:linux or macosx")
			cppdialect("GNU++17")

		filter({})
end

newoption({
	trigger = "source",
	description = "Sets the path to the source directory",
	value = "path to source directory"
})

newoption({
	trigger = "autoinstall",
	description = "Automatically installs the module to GarrysMod/garrysmod/bin (works as a flag and a receiver for a path)"
})

local function GetSteamLibraryDirectories()
	local dir

	if os.istarget("windows") then
		if os.getWindowsRegistry("HKCU:\\Software\\Valve\\Steam\\SteamPath") then
			dir = os.getWindowsRegistry("HKCU:\\Software\\Valve\\Steam\\SteamPath") .. "/SteamApps/"
		else
			local p = io.popen("wmic logicaldisk get caption")

			for line in p:read("*a"):gmatch("%S+") do
				if line ~= "Caption" then
					local steamDir1 = string.format("%s\\Program Files (x86)\\Steam\\SteamApps\\", line)
					local steamDir2 = string.format("%s\\Program Files\\Steam\\SteamApps\\", line)

					if os.isdir(steamDir1) then
						dir = steamDir1
					elseif os.isdir(steamDir2) then
						dir = steamDir2
					end
				end
			end

			p:close()
		end
	elseif os.istarget("linux") then
		dir = path.join(os.getenv("HOME") or "~", ".local/share/Steam/SteamApps/")
	elseif os.istarget("macosx") then
		dir = path.join(os.getenv("HOME") or "~", "Library/Application Support/Steam/SteamApps/")
	end

	if dir then
		local dirs = {dir}

		if os.isfile(dir .. "libraryfolders.vdf") then
			local f = io.open(dir .. "libraryfolders.vdf","r")

			for _, libdir in f:read("*a"):gmatch("\n%s*\"(%d+)\"%s*\"(.-)\"") do
				if os.isdir(libdir) then
					if os.isdir(libdir .. "\\steamapps") then
						libdir = libdir .. "\\steamapps"
					end

					dirs[#dirs + 1] = libdir:gsub("\\\\","\\")

					local pathSep = dirs[#dirs]:match("[/\\]")
					if dirs[#dirs]:sub(-1, -1) ~= pathSep then
						dirs[#dirs] = dirs[#dirs] .. pathSep
					end
				end
			end

			f:close()
		end

		return dirs
	end

	return {}
end

local function FindGarrysModDirectory()
	local dirs = GetSteamLibraryDirectories()

	for _, dir in ipairs(dirs) do
		if os.isdir(dir .. "common/GarrysMod/") then
			return dir .. "common/GarrysMod/"
		elseif os.isdir(dir .. "common/garrysmod/") then
			return dir .. "common/garrysmod/"
		end
	end

	return
end

local function FindGarrysModLuaBinDirectory()
	local dir = FindGarrysModDirectory()
	if not dir then
		return
	end

	if not os.isdir(dir .. "garrysmod/lua/bin") then
		os.mkdir(dir .. "garrysmod/lua/bin")
	end

	return dir .. "garrysmod/lua/bin/"
end

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

	if abi_compatible and os.istarget("windows") and _ACTION ~= "vs2017" then
		error("The only supported compilation platform for this project (" .. name .. ") on Windows is Visual Studio 2017.")
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
		sysincludedirs(_GARRYSMOD_COMMON_DIRECTORY .. "/include")
		includedirs(_project.directory)

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
			files(_GARRYSMOD_COMMON_DIRECTORY .. "/source/ABICompatibility.cpp")
			vpaths({["Source files/garrysmod_common"] = _GARRYSMOD_COMMON_DIRECTORY .. "/source/ABICompatibility.cpp"})
		end

		targetprefix("")
		targetextension(".dll")

		filter("system:windows")
			targetsuffix("_win32")

		filter("system:linux")
			targetsuffix("_linux")
			linkoptions({"-static-libgcc", "-static-libstdc++"})

		filter("system:macosx")
			targetsuffix("_osx")

		if _OPTIONS["autoinstall"] then
			local binDir = _OPTIONS["autoinstall"] ~= "" and _OPTIONS["autoinstall"] or os.getenv("GARRYSMOD_LUA_BIN") or FindGarrysModLuaBinDirectory() or DEFAULT_GARRYSMOD_LUA_BIN_DIRECTORY
			assert(type(binDir) == "string", "The path to GarrysMod/garrysmod/lua/bin is not a string!")

			filter("system:windows")
				postbuildcommands({"{COPY} %{cfg.buildtarget.abspath} \"" .. binDir .. "\""})

			filter("system:not windows")
				postbuildcommands({"{COPY} %{cfg.buildtarget.abspath} \"" .. binDir .. "%{cfg.buildtarget.name}\""})
		end

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
