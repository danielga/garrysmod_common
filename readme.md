# garrysmod\_common

A repository of common bits for compilation projects based on Garry's Mod.  
The include folder has all the required headers for building modules for Garry's Mod (LuaJIT and Garry's Mod headers) with C++.  
There's common code for premake on the premake folder for faster development. Include this folder or premake5.lua (including this folder runs the premake5.lua file by default).
As you can see by the filename, you need premake5.  

## Warning

Do not use internal classes/structures (like the GameDepot::System class or the IGamemodeSystem::Information structure) unless you compile with **Visual Studio 2017** on **release** mode for Windows.  
On Linux, everything should work fine as is, on **release** mode.  
For Mac OSX, any **Xcode (using the GCC compiler)** version *MIGHT* work as long as the **Mac OSX 10.7 SDK** is used, on **release** mode.  
These restrictions are not random; they exist because of ABI compatibility reasons.  
If stuff starts erroring or fails to work, be sure to check the correct line endings (\\n and such) are present in the files for each OS.  

## Usage

In your project's premake5.lua (or whatever you named it) you should include your local copy of this repository:  

```lua
include("path/to/this/repos/local/copy")
```

You don't need to place premake5.lua on the path as said on the first section.  

Example:

```lua
newoption({
    trigger = "gmcommon",
    description = "Sets the path to the garrysmod\_common (https://github.com/danielga/garrysmod\_common) directory",
    value = "path to garrysmod\_common directory"
})

local gmcommon = \_OPTIONS.gmcommon or os.getenv("GARRYSMOD\_COMMON")
assert(gmcommon ~= nil, "you didn't provide a path to your garrysmod\_common (https://github.com/danielga/garrysmod\_common) directory")
include(gmcommon)
```

Creates the workspace with the provided workspace\_name, optional workspace\_add\_debug for including a debug compilation mode (default is true) and optional workspace\_path for files (can also be set by premake option (--workspace=path) and by default uses the value in the config file). Must be called at least once before the next functions.  

```lua
CreateWorkspace({
    name = workspace\_name
    [, allow\_debug = workspace\_add\_debug]
    [, path = workspace\_path]
})
```

Creates the project for the provided state on project\_serverside (it's a boolean), optional project\_manual\_files (allows you to add the source/header files manually through the function files and default is false) and optional project\_source\_path for source files path (can also be set by premake option (--source=path, beware it will be used by all projects) and by default uses the value in the config file).  

```lua
CreateProject({
    serverside = project\_serverside
    [, manual\_files = project\_manual\_files]
    [, source\_path = project\_source\_path]
})
```

Call the next functions as needed. [directory] means it's optional because you can also use premake options, environments variables or the config file in this repo.  

```lua
IncludeLuaShared() -- uses this repo path
IncludeDetouring() -- uses this repo detouring submodule
IncludeScanning() -- uses this repo scanning submodule

IncludeSDKCommon([directory]) -- premake option: --sourcesdk=directory - env var: SOURCE\_SDK
IncludeSDKTier0([directory]) -- premake option: --sourcesdk=directory - env var: SOURCE\_SDK
IncludeSDKTier1([directory]) -- premake option: --sourcesdk=directory - env var: SOURCE\_SDK
IncludeSDKTier2([directory]) -- premake option: --sourcesdk=directory - env var: SOURCE\_SDK
IncludeSDKTier3([directory]) -- premake option: --sourcesdk=directory - env var: SOURCE\_SDK
IncludeSDKMathlib([directory]) -- premake option: --sourcesdk=directory - env var: SOURCE\_SDK
IncludeSDKRaytrace([directory]) -- premake option: --sourcesdk=directory - env var: SOURCE\_SDK
IncludeSteamAPI([directory]) -- premake option: --sourcesdk=directory - env var: SOURCE\_SDK
```

You can also request the project to auto-install the compiled files to a directory selected by you or automatically found on your system. Use one of the following:  

* Append the `--autoinstall` flag to your command to either use the GARRYSMOD\_LUA\_BIN env var, automatic path finder or the DEFAULT\_GARRYSMOD\_LUA\_BIN\_DIRECTORY config (which you have to define yourself in config.lua).  
* Append the `--autoinstall=path` config to your command to use the `path` you want.  

## Relevant URLs

<https://github.com/ValveSoftware/source-sdk-2013>  
<https://github.com/danielga/sourcesdk-minimal>  
