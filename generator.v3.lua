-- Version 3 of the project generator
-- Enables x86-64 compilation

assert(_ACTION ~= nil, "no action (vs20**, gmake or xcode for example) provided!")

include("premake/generator_version.lua")

local default_project_generator_version = 3

local requested_version
PROJECT_GENERATOR_VERSION, requested_version = GetRequestedProjectGeneratorVersionOrDefault(default_project_generator_version)
if requested_version and PROJECT_GENERATOR_VERSION ~= default_project_generator_version then
    error(string.format("environment variable PROJECT_GENERATOR_VERSION (%d) doesn't match this generators version (%d)", PROJECT_GENERATOR_VERSION, default_project_generator_version))
end

include("premake/config.v3.default.lua")
dofileopt("config.v3.lua")
include("premake/generator.lua")
