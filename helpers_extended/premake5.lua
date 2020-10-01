local current_dir = _SCRIPT_DIR

function IncludeHelpersExtended()
	local refcount = IncludePackage("helpers_extended")

	local _project = project()

	sysincludedirs(current_dir .. "/include")
	-- GCC requires manual ordering of links, where libraries on the left depend on libraries on the right
	-- Since helpers_extended depends on helpers and scanning, the latter must appear after
	links({"helpers_extended", "helpers", "scanning"})

	if refcount == 1 then
		dofile(current_dir .. "/premake5_create_project.lua")
		project(_project.name)
	end
end
