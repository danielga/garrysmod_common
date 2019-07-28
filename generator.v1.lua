-- Version 1 of the project generator

include("premake/config.v1.default.lua")
dofileopt("config.v1.lua")
include("premake/generator.lua")
