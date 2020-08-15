-- Automatic generator version selector

assert(_ACTION ~= nil, "no action (vs20**, gmake or xcode for example) provided!")

include("premake/version.lua")

local default_project_generator_version = 1

local version, requested_version = GetRequestedProjectGeneratorVersionOrDefault(default_project_generator_version)

local path = string.format("%s/generator.v%d.lua", _SCRIPT_DIR, version)
if os.isfile(path) then
    if requested_version then
        print(string.format("Selected version %d of the project generator", version))
    else
        print("No version of the project generator specified, defaulting to version " .. default_project_generator_version)
    end

    include(path)
else
    error(string.format("requested an unknown version (%s) of the project generator", version))
end
