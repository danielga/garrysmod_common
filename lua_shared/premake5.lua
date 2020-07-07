local current_dir = _SCRIPT_DIR

function IncludeLuaShared()
	local refcount = IncludePackage("lua_shared")

	local _project = project()

	links("lua_shared")

	if refcount == 1 then
		dofile(current_dir .. "/premake5_create_project.lua")
		project(_project.name)
	end
end
