local semver = require("semver")
local minimum_version = semver("5.0.0-beta2")
local current_version = semver(_PREMAKE_VERSION)

assert(
	current_version >= minimum_version,
	string.format(
		"Your premake binary is outdated! Minimum version is \"%s\" and your version is \"%s\".",
		minimum_version,
		current_version
	)
)
