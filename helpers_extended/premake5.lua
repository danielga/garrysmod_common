local current_dir = _SCRIPT_DIR

function IncludeHelpersExtended()
	local refcount = IncludePackage("helpers_extended")

	local _project = project()

	sysincludedirs(current_dir .. "/include")
	links("helpers_extended")

	if refcount == 1 then
		include(current_dir .. "/premake5_create_project.lua")
		project(_project.name)
	end
end
