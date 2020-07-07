local current_dir = _SCRIPT_DIR

function IncludeHelpers()
	local refcount = IncludePackage("helpers")

	local _project = project()

	sysincludedirs(current_dir .. "/include")
	links("helpers")

	if refcount == 1 then
		dofile(current_dir .. "/premake5_create_project.lua")
		project(_project.name)
	end
end
