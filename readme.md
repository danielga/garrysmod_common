# garrysmod_common

A repository of common bits for compilation projects based on Garry's Mod.

The include folder has all the required headers for building modules for Garry's Mod (LuaJIT and Garry's Mod headers) with C++.

The lib folder provides files for linking on each platform supported by Garry's Mod.

There's common code for premake on the premake folder for faster development. Include this folder or premake5.lua (including this folder runs the premake5.lua file by default).
As you can see by the filename, you need premake5.

## Usage

In your project's premake5.lua (or whatever you named it) you should include your local copy of this repository:

	include("path/to/this/repos/local/copy")

You don't need to place premake5.lua on the path as said on the previous section.

Example:

	newoption({
		trigger = "gmcommon",
		description = "Sets the path to the garrysmod_common (https://bitbucket.org/danielga/garrysmod_common) directory",
		value = "path to garrysmod_common dir"
	})

	local gmcommon = _OPTIONS.gmcommon or os.getenv("GARRYSMOD_COMMON")
	if gmcommon == nil then
		error("you didn't provide a path to your garrysmod_common (https://bitbucket.org/danielga/garrysmod_common) directory")
	end

	include(gmcommon)
<br />

	CreateSolution(name [, solutionpath])
Creates the solution with the provided name and optional path for files (can also be set by premake option (--solution=path) and by default uses the value in the config file). Must be called at least once before the next functions.

	CreateProject(is_server [, manual_files [, sourcepath]])
Creates the project with the provided name and optional path for source files (can also be set by premake option (--source=path, beware it will be used by all projects) and by default uses the value in the config file). is_server receives a boolean saying if the module is serverside or not (also accepts the globals SERVERSIDE or CLIENTSIDE). manual_files allows you to add the source/header files manually through AddFiles. Must be called at least once before the next functions.

	SERVERSIDE -- Serverside module
	CLIENTSIDE -- Clientside module

	SOURCES_MANUAL -- add source/header files manually
	SOURCES_ALL -- adds all files in the sourcepath automatically

	-- These filters work as a OR operation
	FILTER_WINDOWS -- Windows filter
	FILTER_LINUX -- Linux filter
	FILTER_MACOSX -- Mac OSX filter

	FILTER_DEBUG -- Debug filter
	FILTER_RELEASE -- Release filter

	AddFiles(files) -- a string/a table of strings which are paths, they can also contain wildcards

Call the next functions as needed. [path] means it's optional because you can also use premake options, environments variables or the config file in this repo.

	IncludeLuaShared() -- uses this repo path
	IncludeDetouring() -- uses this repo detouring submodule
	IncludeScanning() -- uses this repo scanning submodule
	IncludeSourceSDK([path]) -- premake option: --sourcesdk=path - env var: SOURCE_SDK

There are also filters for operating systems (currently only Windows, Mac OSX and Linux are supported) and other things.

	SetFilter(...) -- a variable number of strings/constants which work as filters
	GetFilter() -- get the current filter as a table
	HasFilter(filter) -- check if filter is set

## Relevant URLs

https://github.com/ValveSoftware/source-sdk-2013
