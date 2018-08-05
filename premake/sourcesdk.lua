newoption({
	trigger = "sourcesdk",
	description = "Sets the path to the SourceSDK directory",
	value = "path to SourceSDK directory"
})

local function GetSDKPath(directory)
	directory = directory or _OPTIONS["sourcesdk"] or os.getenv("SOURCE_SDK") or DEFAULT_SOURCESDK_DIRECTORY

	assert(type(directory) == "string", "Source SDK path is not a string!")

	directory = CleanPath(directory)

	local dir = path.getabsolute(directory)
	assert(os.isdir(dir), "'" .. dir .. "' doesn't exist (Source SDK)")

	return directory
end

local function IncludeSDKCommonInternal(directory)
	local _project = project()
	local _workspace = _project.workspace

	defines(_project.serverside and "GAME_DLL" or "CLIENT_DLL")
	sysincludedirs({
		directory .. "/common",
		directory .. "/public"
	})

	if _project.serverside then
		sysincludedirs({
			directory .. "/game/server",
			directory .. "/game/shared"
		})
	else
		sysincludedirs({
			directory .. "/game/client",
			directory .. "/game/shared"
		})
	end

	filter("system:windows")
		defines("WIN32")
		libdirs(directory .. "/lib/public")

		filter({"system:windows", "configurations:Debug"})
			linkoptions("/NODEFAULTLIB:\"libcmt\"")

	filter("system:linux")
		disablewarnings({
			"unused-local-typedefs",
			"unused-parameter",
			"strict-aliasing",
			"unknown-pragmas",
			"invalid-offsetof"
		})
		defines({"COMPILER_GCC", "POSIX", "_POSIX", "LINUX", "_LINUX", "GNUC", "NO_MALLOC_OVERRIDE"})
		libdirs(path.getabsolute(directory) .. "/lib/public/linux32")

	filter("system:macosx")
		disablewarnings({
			"unused-local-typedef",
			"unused-parameter",
			"unused-private-field",
			"overloaded-virtual",
			"unknown-pragmas",
			"unused-variable",
			"unknown-warning-option",
			"invalid-offsetof"
		})
		defines({"COMPILER_GCC", "POSIX", "_POSIX", "OSX", "GNUC", "NO_MALLOC_OVERRIDE"})
		libdirs(path.getabsolute(directory) .. "/lib/public/osx32")

	filter({})
end

function IncludeSDKCommon(directory)
	IncludePackage("sdkcommon")
	IncludeSDKCommonInternal(GetSDKPath(directory))
	defines("GMOD_USE_SOURCESDK")
end

function IncludeSDKTier0(directory)
	IncludePackage("sdktier0")

	local _project = project()
	local _workspace = _project.workspace

	directory = GetSDKPath(directory)

	sysincludedirs(directory .. "/public/tier0")

	filter("system:windows or macosx")
		links("tier0")

	filter("system:linux")
		links(_project.serverside and "tier0_srv" or "tier0")

	filter({})
end

function IncludeSDKTier1(directory)
	IncludePackage("sdktier1")

	local _project = project()
	local _workspace = _project.workspace
	local _project_directory = _GARRYSMOD_COMMON_DIRECTORY .. "/projects/" .. os.target() .. "/" .. _ACTION

	directory = GetSDKPath(directory)

	sysincludedirs(directory .. "/public/tier1")
	links("tier1")

	filter("system:windows")
		links({"vstdlib", "ws2_32", "rpcrt4"})

	filter("system:linux")
		links(_project.serverside and "vstdlib_srv" or "vstdlib")

	filter("system:macosx")
		links({"vstdlib", "iconv"})

	project("tier1")
		kind("StaticLib")
		warnings("Default")
		language("C++")
		cppdialect("GNU++11")
		location(_GARRYSMOD_COMMON_DIRECTORY .. "/projects/" .. os.target() .. "/" .. _ACTION)
		defines({"TIER1_STATIC_LIB", "_CRT_SECURE_NO_WARNINGS"})
		sysincludedirs({
			directory .. "/public/tier0",
			directory .. "/public/tier1"
		})
		vpaths({["Source files/*"] = {
			directory .. "/tier1/*.cpp",
			directory .. "/utils/lzma/C/*.c"
		}})
		IncludeSDKCommonInternal(directory)
		files({
			directory .. "/tier1/bitbuf.cpp",
			directory .. "/tier1/byteswap.cpp",
			directory .. "/tier1/characterset.cpp",
			directory .. "/tier1/checksum_crc.cpp",
			directory .. "/tier1/checksum_md5.cpp",
			directory .. "/tier1/checksum_sha1.cpp",
			directory .. "/tier1/commandbuffer.cpp",
			directory .. "/tier1/convar.cpp",
			directory .. "/tier1/datamanager.cpp",
			directory .. "/tier1/diff.cpp",
			directory .. "/tier1/generichash.cpp",
			directory .. "/tier1/ilocalize.cpp",
			directory .. "/tier1/interface.cpp",
			directory .. "/tier1/KeyValues.cpp",
			directory .. "/tier1/kvpacker.cpp",
			directory .. "/tier1/lzmaDecoder.cpp",
			directory .. "/tier1/mempool.cpp",
			directory .. "/tier1/memstack.cpp",
			directory .. "/tier1/NetAdr.cpp",
			directory .. "/tier1/splitstring.cpp",
			directory .. "/tier1/rangecheckedvar.cpp",
			directory .. "/tier1/reliabletimer.cpp",
			directory .. "/tier1/stringpool.cpp",
			directory .. "/tier1/strtools.cpp",
			directory .. "/tier1/strtools_unicode.cpp",
			directory .. "/tier1/tier1.cpp",
			directory .. "/tier1/tokenreader.cpp",
			directory .. "/tier1/sparsematrix.cpp",
			directory .. "/tier1/uniqueid.cpp",
			directory .. "/tier1/utlbuffer.cpp",
			directory .. "/tier1/utlbufferutil.cpp",
			directory .. "/tier1/utlstring.cpp",
			directory .. "/tier1/utlsymbol.cpp",
			directory .. "/tier1/utlbinaryblock.cpp",
			directory .. "/tier1/snappy.cpp",
			directory .. "/tier1/snappy-sinksource.cpp",
			directory .. "/tier1/snappy-stubs-internal.cpp",
			directory .. "/utils/lzma/C/LzmaDec.c"
		})

		filter("configurations:Release")
			objdir(_project_directory .. "/intermediate")
			targetdir(_project_directory .. "/release")

		if not _workspace.abi_compatible then
			filter("configurations:Debug")
				objdir(_project_directory .. "/intermediate")
				targetdir(_project_directory .. "/debug")
		end

		filter("system:windows")
			defines({"_DLL_EXT=.dll", "WIN32"})
			files(directory .. "/tier1/processor_detect.cpp")

		filter("system:linux")
			disablewarnings("unused-result")
			defines({"_DLL_EXT=.so", "COMPILER_GCC", "POSIX", "_POSIX", "LINUX", "_LINUX", "GNUC", "NO_MALLOC_OVERRIDE"})
			files({
				directory .. "/tier1/processor_detect_linux.cpp",
				directory .. "/tier1/qsort_s.cpp",
				directory .. "/tier1/pathmatch.cpp"
			})
			linkoptions({
				"-Xlinker --wrap=fopen",
				"-Xlinker --wrap=freopen",
				"-Xlinker --wrap=open",
				"-Xlinker --wrap=creat",
				"-Xlinker --wrap=access",
				"-Xlinker --wrap=__xstat",
				"-Xlinker --wrap=stat",
				"-Xlinker --wrap=lstat",
				"-Xlinker --wrap=fopen64",
				"-Xlinker --wrap=open64",
				"-Xlinker --wrap=opendir",
				"-Xlinker --wrap=__lxstat",
				"-Xlinker --wrap=chmod",
				"-Xlinker --wrap=chown",
				"-Xlinker --wrap=lchown",
				"-Xlinker --wrap=symlink",
				"-Xlinker --wrap=link",
				"-Xlinker --wrap=__lxstat64",
				"-Xlinker --wrap=mknod",
				"-Xlinker --wrap=utimes",
				"-Xlinker --wrap=unlink",
				"-Xlinker --wrap=rename",
				"-Xlinker --wrap=utime",
				"-Xlinker --wrap=__xstat64",
				"-Xlinker --wrap=mount",
				"-Xlinker --wrap=mkfifo",
				"-Xlinker --wrap=mkdir",
				"-Xlinker --wrap=rmdir",
				"-Xlinker --wrap=scandir",
				"-Xlinker --wrap=realpath"
			})

		filter("system:macosx")
			defines({"_DLL_EXT=.dylib", "COMPILER_GCC", "POSIX", "_POSIX", "OSX", "GNUC", "NO_MALLOC_OVERRIDE"})
			files(directory .. "/tier1/processor_detect_linux.cpp")

	project(_project.name)
end

function IncludeSDKTier2(directory)
	IncludePackage("sdktier2")

	directory = GetSDKPath(directory)

	sysincludedirs(directory .. "/public/tier2")

	filter("system:windows")
		links("tier2")

	filter("system:macosx")
		linkoptions(path.getabsolute(directory) .. "/lib/public/osx32/tier2.a")

	filter("system:linux")
		linkoptions(path.getabsolute(directory) .. "/lib/public/linux32/tier2.a")

	filter({})
end

function IncludeSDKTier3(directory)
	IncludePackage("sdktier3")

	directory = GetSDKPath(directory)

	sysincludedirs(directory .. "/public/tier3")

	filter("system:windows")
		links("tier3")

	filter("system:macosx")
		linkoptions(path.getabsolute(directory) .. "/lib/public/osx32/tier3.a")

	filter("system:linux")
		linkoptions(path.getabsolute(directory) .. "/lib/public/linux32/tier3.a")

	filter({})
end

function IncludeSDKMathlib(directory)
	IncludePackage("sdkmathlib")

	local _project = project()
	local _workspace = _project.workspace
	local _project_directory = _GARRYSMOD_COMMON_DIRECTORY .. "/projects/" .. os.target() .. "/" .. _ACTION

	directory = GetSDKPath(directory)

	sysincludedirs(directory .. "/public/mathlib")
	links("mathlib")

	project("mathlib")
		kind("StaticLib")
		warnings("Default")
		location(_GARRYSMOD_COMMON_DIRECTORY .. "/projects/" .. os.target() .. "/" .. _ACTION)
		defines("MATHLIB_LIB")
		sysincludedirs({
			directory .. "/public/mathlib",
			directory .. "/public/tier0",
		})
		vpaths({["Source files/*"] = directory .. "/mathlib/*.cpp"})
		IncludeSDKCommonInternal(directory)
		files({
			directory .. "/mathlib/color_conversion.cpp",
			directory .. "/mathlib/halton.cpp",
			directory .. "/mathlib/lightdesc.cpp",
			directory .. "/mathlib/mathlib_base.cpp",
			directory .. "/mathlib/powsse.cpp",
			directory .. "/mathlib/sparse_convolution_noise.cpp",
			directory .. "/mathlib/sseconst.cpp",
			directory .. "/mathlib/sse.cpp",
			directory .. "/mathlib/ssenoise.cpp",
			directory .. "/mathlib/anorms.cpp",
			directory .. "/mathlib/bumpvects.cpp",
			directory .. "/mathlib/IceKey.cpp",
			directory .. "/mathlib/imagequant.cpp",
			directory .. "/mathlib/polyhedron.cpp",
			directory .. "/mathlib/quantize.cpp",
			directory .. "/mathlib/randsse.cpp",
			directory .. "/mathlib/spherical.cpp",
			directory .. "/mathlib/simdvectormatrix.cpp",
			directory .. "/mathlib/vector.cpp",
			directory .. "/mathlib/vmatrix.cpp",
			directory .. "/mathlib/almostequal.cpp"
		})

		filter("configurations:Release")
			objdir(_project_directory .. "/intermediate")
			targetdir(_project_directory .. "/release")

		if not _workspace.abi_compatible then
			filter("configurations:Debug")
				objdir(_project_directory .. "/intermediate")
				targetdir(_project_directory .. "/debug")
		end

		filter("system:windows or linux")
			files(directory .. "/mathlib/3dnow.cpp")

		filter("system:windows")
			defines("WIN32")

		filter("system:linux")
			disablewarnings("ignored-attributes")
			defines({"COMPILER_GCC", "POSIX", "_POSIX", "LINUX", "_LINUX", "GNUC", "NO_MALLOC_OVERRIDE"})

		filter("system:macosx")
			defines({"COMPILER_GCC", "POSIX", "_POSIX", "OSX", "GNUC", "NO_MALLOC_OVERRIDE"})

	project(_project.name)
end

function IncludeSDKRaytrace(directory)
	IncludePackage("sdkraytrace")

	local _project = project()
	local _workspace = _project.workspace
	local _project_directory = _GARRYSMOD_COMMON_DIRECTORY .. "/projects/" .. os.target() .. "/" .. _ACTION

	directory = GetSDKPath(directory)

	links("raytrace")

	project("raytrace")
		kind("StaticLib")
		warnings("Default")
		location(_GARRYSMOD_COMMON_DIRECTORY .. "/projects/" .. os.target() .. "/" .. _ACTION)
		sysincludedirs({
			directory .. "/utils/common",
			directory .. "/public/tier0",
			directory .. "/public/tier1",
		})
		vpaths({["Source files/*"] = directory .. "/raytrace/*.cpp"})
		IncludeSDKCommonInternal(directory)
		files({
			directory .. "/raytrace/raytrace.cpp",
			directory .. "/raytrace/trace2.cpp",
			directory .. "/raytrace/trace3.cpp"
		})

		filter("configurations:Release")
			objdir(_project_directory .. "/intermediate")
			targetdir(_project_directory .. "/release")

		if not _workspace.abi_compatible then
			filter("configurations:Debug")
				objdir(_project_directory .. "/intermediate")
				targetdir(_project_directory .. "/debug")
		end

		filter("system:windows")
			defines("WIN32")

		filter("system:linux")
			defines({"COMPILER_GCC", "POSIX", "_POSIX", "LINUX", "_LINUX", "GNUC", "NO_MALLOC_OVERRIDE"})

		filter("system:macosx")
			defines({"COMPILER_GCC", "POSIX", "_POSIX", "OSX", "GNUC", "NO_MALLOC_OVERRIDE"})

	project(_project.name)
end

function IncludeSDKBitmap(directory)
	IncludePackage("sdkbitmap")

	local _project = project()
	local _workspace = _project.workspace
	local _project_directory = _GARRYSMOD_COMMON_DIRECTORY .. "/projects/" .. os.target() .. "/" .. _ACTION

	directory = GetSDKPath(directory)

	links("bitmap")

	project("bitmap")
		kind("StaticLib")
		warnings("Default")
		location(_GARRYSMOD_COMMON_DIRECTORY .. "/projects/" .. os.target() .. "/" .. _ACTION)
		includedirs({
			directory .. "/utils/common",
			directory .. "/public/tier0",
			directory .. "/public/tier1",
		})
		vpaths({["Source files/*"] = directory .. "/bitmap/*.cpp"})
		IncludeSDKCommonInternal(directory)
		files({
			directory .. "/bitmap/colorconversion.cpp",
			directory .. "/bitmap/float_bm_bilateral_filter.cpp",
			directory .. "/bitmap/float_bm.cpp",
			directory .. "/bitmap/float_bm2.cpp",
			directory .. "/bitmap/float_bm3.cpp",
			directory .. "/bitmap/float_bm4.cpp",
			directory .. "/bitmap/float_cube.cpp",
			directory .. "/bitmap/imageformat.cpp",
			directory .. "/bitmap/psd.cpp",
			directory .. "/bitmap/resample.cpp",
			directory .. "/bitmap/tgaloader.cpp",
			directory .. "/bitmap/tgawriter.cpp",
		})

		filter("configurations:Release")
			objdir(_project_directory .. "/intermediate")
			targetdir(_project_directory .. "/release")

		if not _workspace.abi_compatible then
			filter("configurations:Debug")
				objdir(_project_directory .. "/intermediate")
				targetdir(_project_directory .. "/debug")
		end

		filter("system:windows")
			defines("WIN32")

		filter("system:linux")
			defines({"COMPILER_GCC", "POSIX", "_POSIX", "LINUX", "_LINUX", "GNUC", "NO_MALLOC_OVERRIDE"})

		filter("system:macosx")
			defines({"COMPILER_GCC", "POSIX", "_POSIX", "OSX", "GNUC", "NO_MALLOC_OVERRIDE"})

			if _workspace.abi_compatible then
				buildoptions("-mmacosx-version-min=10.5")
				linkoptions("-mmacosx-version-min=10.5")
			end

	project(_project.name)
end

function IncludeSDKVTF(directory)
	IncludePackage("sdkvtf")

	local _project = project()
	local _workspace = _project.workspace
	local _project_directory = _GARRYSMOD_COMMON_DIRECTORY .. "/projects/" .. os.target() .. "/" .. _ACTION

	directory = GetSDKPath(directory)

	links("vtf")

	project("vtf")
		kind("StaticLib")
		warnings("Default")
		links("bitmap")
		location(_GARRYSMOD_COMMON_DIRECTORY .. "/projects/" .. os.target() .. "/" .. _ACTION)
		includedirs({
			directory .. "/utils/common",
			directory .. "/public/tier0",
			directory .. "/public/tier1",
		})
		vpaths({["Source files/*"] = directory .. "/vtf/*.cpp"})
		IncludeSDKCommonInternal(directory)
		files({
			directory .. "/vtf/cvtf.h",
			directory .. "/vtf/vtf.cpp",
			directory .. "/vtf/s3tc_decode.h",
			directory .. "/vtf/s3tc_decode.cpp",
		})

		filter("configurations:Release")
			objdir(_project_directory .. "/intermediate")
			targetdir(_project_directory .. "/release")

		if not _workspace.abi_compatible then
			filter("configurations:Debug")
				objdir(_project_directory .. "/intermediate")
				targetdir(_project_directory .. "/debug")
		end

		filter("system:windows")
			defines("WIN32")

		filter("system:linux")
			defines({"COMPILER_GCC", "POSIX", "_POSIX", "LINUX", "_LINUX", "GNUC", "NO_MALLOC_OVERRIDE"})

		filter("system:macosx")
			defines({"COMPILER_GCC", "POSIX", "_POSIX", "OSX", "GNUC", "NO_MALLOC_OVERRIDE"})

			if _workspace.abi_compatible then
				buildoptions("-mmacosx-version-min=10.5")
				linkoptions("-mmacosx-version-min=10.5")
			end

	project(_project.name)
end

function IncludeSteamAPI(directory)
	IncludePackage("steamapi")

	local _workspace = project().workspace

	directory = GetSDKPath(directory)

	sysincludedirs(directory .. "/public/steam")

	links("steam_api")
end
