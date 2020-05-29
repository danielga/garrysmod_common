local current_dir = _SCRIPT_DIR

function IncludeLuaShared()
	local refcount = IncludePackage("luashared")

	local _project = project()

	sysincludedirs(current_dir .. "/../include")
	links("lua_shared")

	if refcount == 1 then
		include(current_dir .. "/premake5_create_project.lua")
		project(_project.name)
	end
end
