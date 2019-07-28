newoption({
	trigger = "sourcesdk",
	description = "Sets the path to the SourceSDK directory",
	value = "path to SourceSDK directory"
})

local function GetSDKPath(directory)
	directory = directory or _OPTIONS["sourcesdk"] or os.getenv("SOURCE_SDK") or SOURCESDK_DIRECTORY or --[[deprecated]] DEFAULT_SOURCESDK_DIRECTORY

	assert(type(directory) == "string", "Source SDK path is not a string!")

	local dir = path.getabsolute(directory)
	assert(os.isdir(dir), "'" .. dir .. "' doesn't exist (Source SDK)")

	return directory
end

local function IncludeSDKCommonInternal(directory)
	local _project = project()
	local _workspace = _project.workspace

	defines(_project.serverside and "GAME_DLL" or "CLIENT_DLL")
	sysincludedirs({
		path.join(directory, "common"),
		path.join(directory, "public")
	})

	if _project.serverside then
		sysincludedirs({
			path.join(directory, "game/server"),
			path.join(directory, "game/shared")
		})
	else
		sysincludedirs({
			path.join(directory, "game/client"),
			path.join(directory, "game/shared")
		})
	end

	filter("system:windows")
		defines("WIN32")
		libdirs(path.join(directory, "lib/public"))

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
		libdirs(path.join(path.getabsolute(directory), "lib/public/linux32"))

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
		libdirs(path.join(path.getabsolute(directory), "lib/public/osx32"))

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

	sysincludedirs(path.join(directory, "public/tier0"))

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

	directory = GetSDKPath(directory)

	sysincludedirs(path.join(directory, "public/tier1"))
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
		location(path.join(_GARRYSMOD_COMMON_DIRECTORY, "projects", os.target(), _ACTION))
		defines({"TIER1_STATIC_LIB", "_CRT_SECURE_NO_WARNINGS"})
		sysincludedirs({
			path.join(directory, "public/tier0"),
			path.join(directory, "public/tier1")
		})
		vpaths({["Source files/*"] = {
			path.join(directory, "tier1/*.cpp"),
			path.join(directory, "utils/lzma/C/*.c")
		}})
		IncludeSDKCommonInternal(directory)
		files({
			path.join(directory, "tier1/bitbuf.cpp"),
			path.join(directory, "tier1/byteswap.cpp"),
			path.join(directory, "tier1/characterset.cpp"),
			path.join(directory, "tier1/checksum_crc.cpp"),
			path.join(directory, "tier1/checksum_md5.cpp"),
			path.join(directory, "tier1/checksum_sha1.cpp"),
			path.join(directory, "tier1/commandbuffer.cpp"),
			path.join(directory, "tier1/convar.cpp"),
			path.join(directory, "tier1/datamanager.cpp"),
			path.join(directory, "tier1/diff.cpp"),
			path.join(directory, "tier1/generichash.cpp"),
			path.join(directory, "tier1/ilocalize.cpp"),
			path.join(directory, "tier1/interface.cpp"),
			path.join(directory, "tier1/KeyValues.cpp"),
			path.join(directory, "tier1/kvpacker.cpp"),
			path.join(directory, "tier1/lzmaDecoder.cpp"),
			path.join(directory, "tier1/mempool.cpp"),
			path.join(directory, "tier1/memstack.cpp"),
			path.join(directory, "tier1/NetAdr.cpp"),
			path.join(directory, "tier1/splitstring.cpp"),
			path.join(directory, "tier1/rangecheckedvar.cpp"),
			path.join(directory, "tier1/reliabletimer.cpp"),
			path.join(directory, "tier1/stringpool.cpp"),
			path.join(directory, "tier1/strtools.cpp"),
			path.join(directory, "tier1/strtools_unicode.cpp"),
			path.join(directory, "tier1/tier1.cpp"),
			path.join(directory, "tier1/tokenreader.cpp"),
			path.join(directory, "tier1/sparsematrix.cpp"),
			path.join(directory, "tier1/uniqueid.cpp"),
			path.join(directory, "tier1/utlbuffer.cpp"),
			path.join(directory, "tier1/utlbufferutil.cpp"),
			path.join(directory, "tier1/utlstring.cpp"),
			path.join(directory, "tier1/utlsymbol.cpp"),
			path.join(directory, "tier1/utlbinaryblock.cpp"),
			path.join(directory, "tier1/snappy.cpp"),
			path.join(directory, "tier1/snappy-sinksource.cpp"),
			path.join(directory, "tier1/snappy-stubs-internal.cpp"),
			path.join(directory, "utils/lzma/C/LzmaDec.c")
		})
		targetdir("%{prj.location}/%{cfg.architecture}/%{cfg.buildcfg}")
		debugdir("%{prj.location}/%{cfg.architecture}/%{cfg.buildcfg}")
		objdir("!%{prj.location}/%{cfg.architecture}/%{cfg.buildcfg}/intermediate/%{prj.name}")

		filter("system:windows")
			defines({"_DLL_EXT=.dll", "WIN32"})
			files(path.join(directory, "tier1/processor_detect.cpp"))

		filter("system:linux")
			disablewarnings("unused-result")
			defines({"_DLL_EXT=.so", "COMPILER_GCC", "POSIX", "_POSIX", "LINUX", "_LINUX", "GNUC", "NO_MALLOC_OVERRIDE"})
			files({
				path.join(directory, "tier1/processor_detect_linux.cpp"),
				path.join(directory, "tier1/qsort_s.cpp"),
				path.join(directory, "tier1/pathmatch.cpp")
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
			files(path.join(directory, "tier1/processor_detect_linux.cpp"))

	project(_project.name)
end

function IncludeSDKTier2(directory)
	IncludePackage("sdktier2")

	directory = GetSDKPath(directory)

	sysincludedirs(path.join(directory, "public/tier2"))

	filter("system:windows")
		links("tier2")

	filter("system:macosx")
		linkoptions(path.join(path.getabsolute(directory), "lib/public/osx32/tier2.a"))

	filter("system:linux")
		linkoptions(path.join(path.getabsolute(directory), "lib/public/linux32/tier2.a"))

	filter({})
end

function IncludeSDKTier3(directory)
	IncludePackage("sdktier3")

	directory = GetSDKPath(directory)

	sysincludedirs(path.join(directory, "public/tier3"))

	filter("system:windows")
		links("tier3")

	filter("system:macosx")
		linkoptions(path.join(path.getabsolute(directory), "lib/public/osx32/tier3.a"))

	filter("system:linux")
		linkoptions(path.join(path.getabsolute(directory), "lib/public/linux32/tier3.a"))

	filter({})
end

function IncludeSDKMathlib(directory)
	IncludePackage("sdkmathlib")

	local _project = project()
	local _workspace = _project.workspace

	directory = GetSDKPath(directory)

	sysincludedirs(path.join(directory, "public/mathlib"))
	links("mathlib")

	project("mathlib")
		kind("StaticLib")
		warnings("Default")
		location(path.join(_GARRYSMOD_COMMON_DIRECTORY, "projects", os.target(), _ACTION))
		defines("MATHLIB_LIB")
		sysincludedirs({
			path.join(directory, "public/mathlib"),
			path.join(directory, "public/tier0"),
		})
		vpaths({["Source files/*"] = path.join(directory, "mathlib/*.cpp")})
		IncludeSDKCommonInternal(directory)
		files({
			path.join(directory, "mathlib/color_conversion.cpp"),
			path.join(directory, "mathlib/halton.cpp"),
			path.join(directory, "mathlib/lightdesc.cpp"),
			path.join(directory, "mathlib/mathlib_base.cpp"),
			path.join(directory, "mathlib/powsse.cpp"),
			path.join(directory, "mathlib/sparse_convolution_noise.cpp"),
			path.join(directory, "mathlib/sseconst.cpp"),
			path.join(directory, "mathlib/sse.cpp"),
			path.join(directory, "mathlib/ssenoise.cpp"),
			path.join(directory, "mathlib/anorms.cpp"),
			path.join(directory, "mathlib/bumpvects.cpp"),
			path.join(directory, "mathlib/IceKey.cpp"),
			path.join(directory, "mathlib/imagequant.cpp"),
			path.join(directory, "mathlib/polyhedron.cpp"),
			path.join(directory, "mathlib/quantize.cpp"),
			path.join(directory, "mathlib/randsse.cpp"),
			path.join(directory, "mathlib/spherical.cpp"),
			path.join(directory, "mathlib/simdvectormatrix.cpp"),
			path.join(directory, "mathlib/vector.cpp"),
			path.join(directory, "mathlib/vmatrix.cpp"),
			path.join(directory, "mathlib/almostequal.cpp")
		})
		targetdir("%{prj.location}/%{cfg.architecture}/%{cfg.buildcfg}")
		debugdir("%{prj.location}/%{cfg.architecture}/%{cfg.buildcfg}")
		objdir("!%{prj.location}/%{cfg.architecture}/%{cfg.buildcfg}/intermediate/%{prj.name}")

		filter("system:windows or linux")
			files(path.join(directory, "mathlib/3dnow.cpp"))

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

	directory = GetSDKPath(directory)

	links("raytrace")

	project("raytrace")
		kind("StaticLib")
		warnings("Default")
		location(path.join(_GARRYSMOD_COMMON_DIRECTORY, "projects", os.target(), _ACTION))
		sysincludedirs({
			path.join(directory, "utils/common"),
			path.join(directory, "public/tier0"),
			path.join(directory, "public/tier1"),
		})
		vpaths({["Source files/*"] = path.join(directory, "raytrace/*.cpp")})
		IncludeSDKCommonInternal(directory)
		files({
			path.join(directory, "raytrace/raytrace.cpp"),
			path.join(directory, "raytrace/trace2.cpp"),
			path.join(directory, "raytrace/trace3.cpp")
		})
		targetdir("%{prj.location}/%{cfg.architecture}/%{cfg.buildcfg}")
		debugdir("%{prj.location}/%{cfg.architecture}/%{cfg.buildcfg}")
		objdir("!%{prj.location}/%{cfg.architecture}/%{cfg.buildcfg}/intermediate/%{prj.name}")

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

	directory = GetSDKPath(directory)

	links("bitmap")

	project("bitmap")
		kind("StaticLib")
		warnings("Default")
		location(path.join(_GARRYSMOD_COMMON_DIRECTORY, "projects", os.target(), _ACTION))
		includedirs({
			path.join(directory, "utils/common"),
			path.join(directory, "public/tier0"),
			path.join(directory, "public/tier1")
		})
		vpaths({["Source files/*"] = path.join(directory, "bitmap/*.cpp")})
		IncludeSDKCommonInternal(directory)
		files({
			path.join(directory, "bitmap/colorconversion.cpp"),
			path.join(directory, "bitmap/float_bm_bilateral_filter.cpp"),
			path.join(directory, "bitmap/float_bm.cpp"),
			path.join(directory, "bitmap/float_bm2.cpp"),
			path.join(directory, "bitmap/float_bm3.cpp"),
			path.join(directory, "bitmap/float_bm4.cpp"),
			path.join(directory, "bitmap/float_cube.cpp"),
			path.join(directory, "bitmap/imageformat.cpp"),
			path.join(directory, "bitmap/psd.cpp"),
			path.join(directory, "bitmap/resample.cpp"),
			path.join(directory, "bitmap/tgaloader.cpp"),
			path.join(directory, "bitmap/tgawriter.cpp")
		})
		targetdir("%{prj.location}/%{cfg.architecture}/%{cfg.buildcfg}")
		debugdir("%{prj.location}/%{cfg.architecture}/%{cfg.buildcfg}")
		objdir("!%{prj.location}/%{cfg.architecture}/%{cfg.buildcfg}/intermediate/%{prj.name}")

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

	directory = GetSDKPath(directory)

	links("vtf")

	project("vtf")
		kind("StaticLib")
		warnings("Default")
		links("bitmap")
		location(path.join(_GARRYSMOD_COMMON_DIRECTORY, "projects", os.target(), _ACTION))
		includedirs({
			path.join(directory, "utils/common"),
			path.join(directory, "public/tier0"),
			path.join(directory, "public/tier1")
		})
		vpaths({["Source files/*"] = path.join(directory, "vtf/*.cpp")})
		IncludeSDKCommonInternal(directory)
		files({
			path.join(directory, "vtf/cvtf.h"),
			path.join(directory, "vtf/vtf.cpp"),
			path.join(directory, "vtf/s3tc_decode.h"),
			path.join(directory, "vtf/s3tc_decode.cpp")
		})
		targetdir("%{prj.location}/%{cfg.architecture}/%{cfg.buildcfg}")
		debugdir("%{prj.location}/%{cfg.architecture}/%{cfg.buildcfg}")
		objdir("!%{prj.location}/%{cfg.architecture}/%{cfg.buildcfg}/intermediate/%{prj.name}")

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

	sysincludedirs(path.join(directory, "public/steam"))

	links("steam_api")
end
