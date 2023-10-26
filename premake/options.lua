newoption({
	trigger = "workspace",
	description = "Sets the path for the workspace directory",
	value = "projects/" .. os.target() .. "/" .. (_ACTION or "unknown")
})

newoption({
	trigger = "macosx_sdkroot",
	description = "Sets the path for the MacOSX SDK directory",
	value = "/Applications/Xcode.app/Contents/Developer/..."
})

newoption({
	trigger = "sourcesdk",
	description = "Sets the path to the SourceSDK directory (deprecated)",
	value = _GARRYSMOD_COMMON_DIRECTORY:sub(1, 64)
})

newoption({
	trigger = "source",
	description = "Sets the path to the source directory",
	value = "source"
})

newoption({
	trigger = "autoinstall",
	description = "Automatically installs the module",
	value = "/home/you/garrysmod_server/garrysmod/lua/bin"
})

newoption({
	trigger = "fix",
	description = "Automatically attempt to fix instances reported by the 'format' or 'lint' commands"
})

newoption({
	trigger = "format",
	description = "Reformat files on the 'lint' command (requires the '--fix' flag)"
})

newoption({
	trigger = "use-config-file",
	description = "Use configuration files on the 'lint' command"
})

newoption({
	trigger = "clang-tidy-binary",
	description = "clang-tidy binary to use on the 'lint' command",
	value = "clang-tidy"
})

newoption({
	trigger = "clang-apply-replacements-binary",
	description = "clang-apply-replacements binary to use on the 'lint' command",
	value = "clang-apply-replacements"
})

newoption({
	trigger = "clang-format-binary",
	description = "clang-format binary to use on the 'format' command",
	value = "clang-format"
})

newoption({
	trigger = "jobs",
	description = "Number of parallel jobs to run on the 'lint' command",
	value = "4"
})

newoption({
	trigger = "client-debug-path",
	description = "Root path to the game client to debug with",
	value = "/home/you/garrysmod"
})

newoption({
	trigger = "server-debug-path",
	description = "Root path to the game server to debug with",
	value = "/home/you/garrysmod"
})
