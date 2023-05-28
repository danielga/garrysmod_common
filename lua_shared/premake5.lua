local current_dir = _SCRIPT_DIR

function IncludeLuaShared()
	local refcount = IncludePackage("lua_shared")

	local _project = project()

	links("lua_shared")

	filter("system:linux or macosx")
		links("dl")

	if refcount == 1 then
		dofile(current_dir .. "/premake5_create_project.lua")
	end

	project(_project.name)
end
