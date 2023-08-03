# garrysmod\_common

A repository of common bits for compilation projects based on Garry's Mod.

The `include` directory has all the required headers for building modules for Garry's Mod (LuaJIT and Garry's Mod headers) with C++.

There's common code for premake on the `premake` directory for faster development. premake5 is required to generate projects.

## Warning

As a rule of thumb, avoid diverging too much from these rules:

* On Windows, compile with **Visual Studio 2015**, **Visual Studio 2017** or **Visual Studio 2019**.
* On Linux, use **GCC** (the more up to date, the better, tested up to **GCC 9**, older versions like **GCC 4** and **GCC 5** might throw C++ compilation errors).
* On macOS, any **Xcode (using the GCC compiler)** version *MIGHT* work.

Do not use internal classes/structures (like the `GameDepot::System` class or the `IGamemodeSystem::Information` structure) unless **(these rules are in addition to the previous ones)**:

* On Windows and Linux, compile in **release** mode.
* On macOS, use the **Mac OSX 10.7 SDK**, in **release** mode.

These restrictions are not random; they exist because of ABI compatibility reasons.

If stuff starts erroring or fails to work, be sure to check the correct line endings (`\n` and such) are present in the files for each OS.

## Usage

In your project's `premake5.lua` (or whatever you named it) you should include your local copy of this repository, for example:

```lua
PROJECT_GENERATOR_VERSION = 2

newoption({
    trigger = "gmcommon",
    description = "Sets the path to the garrysmod_common (https://github.com/danielga/garrysmod_common) directory",
    value = "path to garrysmod_common directory"
})

local gmcommon = assert(_OPTIONS.gmcommon or os.getenv("GARRYSMOD_COMMON"),
	"you didn't provide a path to your garrysmod_common (https://github.com/danielga/garrysmod_common) directory")
include(gmcommon)
```

Creates the workspace with the provided `workspace_name`, optional `workspace_add_debug` for including a debug compilation mode (default is `true`) and optional `workspace_path` for files (can also be set by premake option (`--workspace=path`) and by default uses the value in the config file). Must be called at least once before the following functions.

```lua
CreateWorkspace({
    name = workspace_name,
    allow_debug = workspace_add_debug, -- optional
    path = workspace_path -- optional
})
```

Creates the project for the provided state on `project_serverside` (it's a boolean), optional `project_manual_files` (allows you to add the source/header files manually through the function `files` and default is `false`) and optional `project_source_path` for source files path (can also be set by premake option `--source=path` and by default uses the value in the config file, but beware it will be used by all projects).

```lua
CreateProject({
    serverside = project_serverside,
    manual_files = project_manual_files, -- optional
    source_path = project_source_path -- optional
})
```

Call the following functions as needed. The Source SDK based projects do not need a path to the SDK anymore, as it is provided through this git repository as a submodule.

```lua
IncludeHelpersExtended() -- uses this repo path
IncludeLuaShared() -- uses this repo path
IncludeDetouring() -- uses this repo detouring submodule
IncludeScanning() -- uses this repo scanning submodule

IncludeSDKCommon()
IncludeSDKTier0()
IncludeSDKTier1()
IncludeSDKTier2()
IncludeSDKTier3()
IncludeSDKMathlib()
IncludeSDKRaytrace()
IncludeSteamAPI()
```

You can also request the project to auto-install the compiled files to a directory selected by you or automatically found on your system. Use one of the following:

* Append the `--autoinstall` flag to your command to either use the `GARRYSMOD_LUA_BIN` environment var, automatic path finder or the `DEFAULT_GARRYSMOD_LUA_BIN_DIRECTORY` config (which you have to define yourself in `config.lua`).
* Append the `--autoinstall=path` config to your command to use the path you want.

Using specific MacOSX SDKs is also supported, by using either the premake config `--macosx_sdkroot` or the preferred `SDKROOT` environment variable when calling `make`.

## Relevant URLs

<https://github.com/ValveSoftware/source-sdk-2013>  
<https://github.com/danielga/sourcesdk-minimal>
