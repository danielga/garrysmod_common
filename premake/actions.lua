require("premake-export-compile-commands/export-compile-commands")

newaction({
	trigger = "install",
	description = "Install dependencies and make sure they're available",
	execute = function()
		-- Make sure we've got git
		local output, code = os.outputof("git --version")
		if code ~= 0 or output == nil or string.match(output, "^git version [^\n]+$") == nil then
			error("git is not installed or its version is too old")
		end

		-- Make sure our submodules are up-to-date
		if not os.execute("git submodule update --init --recursive") then
			error("Failed to init+update git submodules recursively")
		end
	end
})

newaction({
	trigger = "clean",
	description = "Remove project files",
	execute = function()
		local rmfmt = "rmdir /s /q \"" .. _MAIN_SCRIPT_DIR .. "/%s\""
		if not os.ishost("windows") then
			rmfmt = "rm -rf \"" .. _MAIN_SCRIPT_DIR .. "/%s\""
		end

		local dirs = os.matchdirs("projects/" .. os.target() .. "/*")
		for _, dir in pairs(dirs) do
			os.outputof(rmfmt:format(dir))
		end

		print("Removed project files!")
	end
})

local function CheckClangFormatIsInstalled()
	local output, code, what = os.outputof("clang-format --version")
	if output == nil or code ~= 0 or what ~= "exit" then
		error("clang-format is not available")
	end
end

local function CheckClangTidyIsInstalled()
	local output, code, what = os.outputof("clang-tidy --version")
	if output == nil or code ~= 0 or what ~= "exit" then
		error("clang-tidy is not available")
	end
end

local file_extensions = {"c", "h", "cc", "hh", "cpp", "hpp", "cxx", "hxx", "m", "mm"}

local function GetPathsFromArgs()
	local paths = {}

	for _, p in ipairs(_ARGS) do
		local absolute_path = path.getabsolute(p)
		if os.isdir(absolute_path) then
			for _, ext in ipairs(file_extensions) do
				table.insert(paths, path.join(absolute_path, "**." .. ext))
			end
		else
			table.insert(paths, absolute_path)
		end
	end

	if #paths == 0 then
		for _, ext in ipairs(file_extensions) do
			table.insert(paths, path.getabsolute("include/**." .. ext))
			table.insert(paths, path.getabsolute("source/**." .. ext))
		end
	end

	return paths
end

newaction({
	trigger = "lint",
	description = "Lint C++ files using clang-tidy (optionally reformatting them)",
	valid_languages = {"C", "C++"},
	valid_kinds = {"ConsoleApp", "WindowedApp", "SharedLib", "StaticLib"},
	onStart = function()
		CheckClangTidyIsInstalled()
	end,
	onProject = function(prj)
		local relative = path.getrelative(_GARRYSMOD_COMMON_DIRECTORY, prj.location)
		if relative:sub(1, 2) ~= ".." then
			print("Skipping \"" .. prj.name .. "\" because it's part of the \"garrysmod_common\" framework!")
			return
		end

		local clang_tidy_binary = _OPTIONS["clang-tidy-binary"] or "clang-tidy"

		local script_path
		if os.istarget("windows") then
			local where = os.outputof("where " .. clang_tidy_binary)
			if not where then
				error("unable to find the location of " .. clang_tidy_binary)
			end

			local exe_directory = path.getdirectory(path.normalize(where))
			local share_clang_directory = path.join(path.getdirectory(exe_directory), "share", "clang")

			if os.isfile(path.join(exe_directory, "run-clang-tidy")) then
				script_path = path.join(exe_directory, "run-clang-tidy")
			elseif os.isfile(path.join(share_clang_directory, "run-clang-tidy.py")) then
				script_path = path.join(share_clang_directory, "run-clang-tidy.py")
			else
				error("unable to find the location of run-clang-tidy")
			end

			script_path = string.format("python \"%s\"", script_path)
		else
			script_path = "run-clang-tidy"
		end

		local clang_apply_replacements_binary = _OPTIONS["clang-apply-replacements-binary"] or "clang-apply-replacements"
		local maximum_parallel_jobs = tonumber(_OPTIONS["jobs"]) or 4
		local fix_flag = _OPTIONS["fix"] and "-fix" or ""
		local format_flag = _OPTIONS["format"] and "-format" or ""
		local style_flag = _OPTIONS["format"] and (_OPTIONS["use-config-file"] and "-style file" or "-style llvm") or ""
		local clang_tidy_checks = _OPTIONS["use-config-file"] and "" or ("-checks=\"" .. table.concat({
			"*",
			"-altera-id-dependent-backward-branch",
			"-altera-struct-pack-align",
			"-altera-unroll-loops",
			"-bugprone-easily-swappable-parameters",
			"-cppcoreguidelines-macro-usage",
			"-cppcoreguidelines-pro-bounds-array-to-pointer-decay",
			"-cppcoreguidelines-pro-bounds-pointer-arithmetic",
			"-cppcoreguidelines-pro-type-reinterpret-cast",
			"-cppcoreguidelines-pro-type-union-access",
			"-cppcoreguidelines-pro-type-vararg",
			"-fuchsia-default-arguments-calls",
			"-fuchsia-default-arguments-declarations",
			"-fuchsia-overloaded-operator",
			"-fuchsia-statically-constructed-objects",
			"-google-default-arguments",
			"-hicpp-no-array-decay",
			"-hicpp-signed-bitwise",
			"-hicpp-vararg",
			"-llvm-header-guard",
			"-llvmlibc-callee-namespace",
			"-llvmlibc-implementation-in-namespace",
			"-llvmlibc-restrict-system-libc-headers",
			"-misc-non-private-member-variables-in-classes",
			"-modernize-use-trailing-return-type"
		}, ",") .. "\"")

		for _, config in pairs(prj.configs) do
			local compile_commands_path = path.getdirectory(prj.location) .. "/export-compile-commands/" .. prj.name .. "/" .. config.shortname
			os.execute(string.format(
				"%s -clang-tidy-binary \"%s\" -clang-apply-replacements-binary \"%s\" -j %d -quiet -header-filter=\".*\" %s %s %s %s -p \"%s\"",
				script_path,
				clang_tidy_binary,
				clang_apply_replacements_binary,
				maximum_parallel_jobs,
				fix_flag,
				format_flag,
				style_flag,
				clang_tidy_checks,
				compile_commands_path
			))
		end
	end
})

local function ApplyCommandToPathsFromArgs(command_format)
	local paths = GetPathsFromArgs()
	local failed_paths, nonexistent_paths = {}, {}
	for _, p in ipairs(paths) do
		local files = os.matchfiles(p)
		if #files == 0 and not string.find(p, "%*") then
			table.insert(nonexistent_paths, p)
		end

		for _, file in ipairs(files) do
			local success, what, code = os.execute(string.format(command_format, file))
			if not success or what ~= "exit" or code ~= 0 then
				table.insert(failed_paths, file)
			end
		end
	end

	local error_message = ""

	if #failed_paths ~= 0 then
		error_message = error_message .. "\n\n\tPaths that failed:\n\t\t" .. table.concat(failed_paths, "\n\t\t")
	end

	if #nonexistent_paths ~= 0 then
		error_message = error_message .. "\n\n\tPaths that are invalid:\n\t\t" .. table.concat(nonexistent_paths, "\n\t\t")
	end

	if #error_message ~= 0 then
		error(error_message)
	end
end

newaction({
	trigger = "format",
	description = "Format C++ files using clang-format (uses configuration file '.clang-format' and falls back to LLVM style guides)",
	valid_languages = {"C", "C++"},
	execute = function()
		CheckClangFormatIsInstalled()
		ApplyCommandToPathsFromArgs(string.format(
			"%s --style=file --fallback-style=llvm --verbose %s \"%%s\"",
			_OPTIONS["clang-format-binary"] or "clang-format",
			_OPTIONS["fix"] and "-i" or "--dry-run --Werror"
		))
	end
})
