function GetRequestedProjectGeneratorVersionOrDefault(default_project_generator_version)
	if default_project_generator_version ~= nil then
		local default_type = type(default_project_generator_version)
		if default_type ~= "number" then
			error(string.format("invalid default version for the project generator (type %s)", default_type))
		end
	end

	local version = os.getenv("PROJECT_GENERATOR_VERSION") or PROJECT_GENERATOR_VERSION
	local requested_version = true
	if version == nil then
		version = default_project_generator_version
		requested_version = false
	end

	if requested_version then
		local number_version = tonumber(version)
		if number_version == nil then
			error(string.format("requested an invalid version (%s) of the project generator", version))
		end

		version = number_version
	end

	return version, requested_version
end
