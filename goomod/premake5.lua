local current_dir = _SCRIPT_DIR

function IncludeGOOMod()
	local refcount = IncludePackage("goomod")

	local _project = project()

	externalincludedirs(current_dir .. "/include")
	-- GCC requires manual ordering of links, where libraries on the left depend on libraries on the right
	-- Since goomod depends on helpers, the latter must appear after
	links({"goomod", "helpers"})

	if refcount == 1 then
		dofile(current_dir .. "/premake5_create_project.lua")
		project(_project.name)
	end
end
