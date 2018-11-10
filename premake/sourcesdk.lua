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

	defines({_project.serverside and "GAME_DLL" or "CLIENT_DLL", "RAD_TELEMETRY_DISABLED"})
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
		defines({"_DLL_EXT=.dll", "WIN32", "COMPILER_MSVC"})

		filter({"system:windows", "architecture:x86"})
			defines("COMPILER_MSVC32")
			libdirs(directory .. "/lib/public")

		filter({"system:windows", "architecture:x86_64"})
			defines("COMPILER_MSVC64")
			libdirs(directory .. "/lib/public/x64")

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
		defines({"_DLL_EXT=.so", "COMPILER_GCC", "POSIX", "_POSIX", "LINUX", "_LINUX", "GNUC", "NO_MALLOC_OVERRIDE"})

		filter({"system:linux", "architecture:x86"})
			libdirs(path.getabsolute(directory) .. "/lib/public/linux32")

		filter({"system:linux", "architecture:x86_64"})
			libdirs(path.getabsolute(directory) .. "/lib/public/linux64")

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
		defines({"_DLL_EXT=.dylib", "COMPILER_GCC", "POSIX", "_POSIX", "OSX", "GNUC", "NO_MALLOC_OVERRIDE"})

		filter({"system:macosx", "architecture:x86"})
			libdirs(path.getabsolute(directory) .. "/lib/public/osx32")

		filter({"system:macosx", "architecture:x86_64"})
			libdirs(path.getabsolute(directory) .. "/lib/public/osx64")

	filter({})
end

function IncludeSDKCommon(directory)
	IncludePackage("sdkcommon")
	IncludeSDKCommonInternal(GetSDKPath(directory))
	defines("GMOD_USE_SOURCESDK")
end

function IncludeSDKInterfaces(directory)
	IncludePackage("sdkinterfaces")

	local _project = project()
	local _workspace = _project.workspace
	local _project_directory = _GARRYSMOD_COMMON_DIRECTORY .. "/projects/" .. os.target() .. "/" .. _ACTION

	directory = GetSDKPath(directory)

	sysincludedirs(directory .. "/public/interfaces")
	links("interfaces")

	filter("system:windows")
		links({"vstdlib", "ws2_32", "rpcrt4"})

	filter("system:linux")
		links(_project.serverside and "vstdlib_srv" or "vstdlib")

	filter("system:macosx")
		links("vstdlib")

	project("interfaces")
		kind("StaticLib")
		warnings("Default")
		location(_GARRYSMOD_COMMON_DIRECTORY .. "/projects/" .. os.target() .. "/" .. _ACTION)
		defines("_CRT_SECURE_NO_WARNINGS")
		sysincludedirs(directory .. "/public/tier0")
		vpaths({
			["Source files/*"] = directory .. "/interfaces/*.cpp",
			["Header files/*"] = directory .. "/public/interfaces/*.h",
		})
		IncludeSDKCommonInternal(directory)
		files({
			directory .. "/interfaces/interfaces.cpp",
			directory .. "/public/interfaces/interfaces.h"
		})

	project(_project.name)
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
		links("vstdlib")

	project("tier1")
		kind("StaticLib")
		warnings("Default")
		location(_GARRYSMOD_COMMON_DIRECTORY .. "/projects/" .. os.target() .. "/" .. _ACTION)
		defines({"TIER1_STATIC_LIB", "_CRT_SECURE_NO_WARNINGS"})
		sysincludedirs({
			directory .. "/public/tier0",
			directory .. "/public/tier1"
		})
		vpaths({
			["Source files/*"] = {
				directory .. "/tier1/*.cpp",
				directory .. "/utils/lzma/C/*.c"
			},
			["Header files/*"] = {
				directory .. "/public/tier1/*.h",
				directory .. "/public/*.h",
				directory .. "/common/xbox/*.h"
			}
		})
		IncludeSDKCommonInternal(directory)
		files({
			directory .. "/tier1/appinstance.cpp",
			directory .. "/tier1/bitbuf.cpp",
			directory .. "/tier1/newbitbuf.cpp",
			directory .. "/tier1/byteswap.cpp",
			directory .. "/tier1/characterset.cpp",
			directory .. "/tier1/checksum_crc.cpp",
			directory .. "/tier1/checksum_md5.cpp",
			directory .. "/tier1/checksum_sha1.cpp",
			directory .. "/tier1/circularbuffer.cpp",
			directory .. "/tier1/commandbuffer.cpp",
			directory .. "/tier1/convar.cpp",
			directory .. "/tier1/datamanager.cpp",
			directory .. "/tier1/diff.cpp",
			directory .. "/tier1/exprevaluator.cpp",
			directory .. "/tier1/generichash.cpp",
			directory .. "/tier1/interface.cpp",
			directory .. "/tier1/keyvalues.cpp",
			directory .. "/tier1/keyvaluesjson.cpp",
			directory .. "/tier1/kvpacker.cpp",
			directory .. "/tier1/lzmaDecoder.cpp",
			directory .. "/tier1/lzss.cpp",
			directory .. "/tier1/mempool.cpp",
			directory .. "/tier1/memstack.cpp",
			directory .. "/tier1/NetAdr.cpp",
			directory .. "/tier1/splitstring.cpp",
			directory .. "/tier1/rangecheckedvar.cpp",
			directory .. "/tier1/stringpool.cpp",
			directory .. "/tier1/strtools.cpp",
			directory .. "/tier1/strtools_unicode.cpp",
			directory .. "/tier1/tier1.cpp",
			directory .. "/tier1/tier1_logging.cpp",
			directory .. "/tier1/timeutils.cpp",
			directory .. "/tier1/uniqueid.cpp",
			directory .. "/tier1/utlbuffer.cpp",
			directory .. "/tier1/utlbufferutil.cpp",
			directory .. "/tier1/utlsoacontainer.cpp",
			directory .. "/tier1/utlstring.cpp",
			directory .. "/tier1/utlsymbol.cpp",
			directory .. "/tier1/miniprofiler_hash.cpp",
			directory .. "/tier1/sparsematrix.cpp",
			directory .. "/tier1/memoverride_dummy.cpp",
			directory .. "/public/tier1/appinstance.h",
			directory .. "/public/tier1/bitbuf.h",
			directory .. "/public/tier1/byteswap.h",
			directory .. "/public/tier1/callqueue.h",
			directory .. "/public/tier1/characterset.h",
			directory .. "/public/tier1/checksum_crc.h",
			directory .. "/public/tier1/checksum_md5.h",
			directory .. "/public/tier1/checksum_sha1.h",
			directory .. "/public/tier1/circularbuffer.h",
			directory .. "/public/tier1/commandbuffer.h",
			directory .. "/public/tier1/convar.h",
			directory .. "/public/tier1/datamanager.h",
			directory .. "/public/tier1/delegates.h",
			directory .. "/public/tier1/diff.h",
			directory .. "/public/tier1/exprevaluator.h",
			directory .. "/public/tier1/fmtstr.h",
			directory .. "/public/tier1/functors.h",
			directory .. "/public/tier1/generichash.h",
			directory .. "/public/tier1/iconvar.h",
			directory .. "/public/tier1/interface.h",
			directory .. "/public/tier1/interpolatedvar.h",
			directory .. "/public/tier1/keyvalues.h",
			directory .. "/public/tier1/keyvaluesjson.h",
			directory .. "/public/tier1/kvpacker.h",
			directory .. "/public/tier1/lzmaDecoder.h",
			directory .. "/public/tier1/lerp_functions.h",
			directory .. "/public/tier1/lzss.h",
			directory .. "/public/tier1/mempool.h",
			directory .. "/public/tier1/memstack.h",
			directory .. "/public/tier1/netadr.h",
			directory .. "/public/tier1/processor_detect.h",
			directory .. "/public/tier1/rangecheckedvar.h",
			directory .. "/public/tier1/refcount.h",
			directory .. "/public/tier1/smartptr.h",
			directory .. "/public/tier1/sparsematrix.h",
			directory .. "/public/tier1/stringpool.h",
			directory .. "/public/tier1/strtools.h",
			directory .. "/public/tier1/tier1.h",
			directory .. "/public/tier1/tier1_logging.h",
			directory .. "/public/tier1/timeutils.h",
			directory .. "/public/tier1/tokenset.h",
			directory .. "/public/tier1/utlbidirectionalset.h",
			directory .. "/public/tier1/utlblockmemory.h",
			directory .. "/public/tier1/utlbuffer.h",
			directory .. "/public/tier1/utlbufferutil.h",
			directory .. "/public/tier1/utlcommon.h",
			directory .. "/public/tier1/utldict.h",
			directory .. "/public/tier1/utlenvelope.h",
			directory .. "/public/tier1/utlfixedmemory.h",
			directory .. "/public/tier1/utlhandletable.h",
			directory .. "/public/tier1/utlhash.h",
			directory .. "/public/tier1/utlhashtable.h",
			directory .. "/public/tier1/utllinkedlist.h",
			directory .. "/public/tier1/utlmap.h",
			directory .. "/public/tier1/utlmemory.h",
			directory .. "/public/tier1/utlmultilist.h",
			directory .. "/public/tier1/utlpriorityqueue.h",
			directory .. "/public/tier1/utlqueue.h",
			directory .. "/public/tier1/utlrbtree.h",
			directory .. "/public/tier1/utlsoacontainer.h",
			directory .. "/public/tier1/utlsortvector.h",
			directory .. "/public/tier1/utlstack.h",
			directory .. "/public/tier1/utlstring.h",
			directory .. "/public/tier1/utlstringtoken.h",
			directory .. "/public/tier1/utlstringmap.h",
			directory .. "/public/tier1/utlsymbol.h",
			directory .. "/public/tier1/utltscache.h",
			directory .. "/public/tier1/utlvector.h",
			directory .. "/public/tier1/miniprofiler_hash.h",
			directory .. "/public/datamap.h",
			directory .. "/common/xbox/xboxstubs.h",
			directory .. "/utils/lzma/C/LzmaDec.c"
		})

		filter("system:windows")
			files({
				directory .. "/tier1/processor_detect.cpp",
				directory .. "/public/tier1/uniqueid.h"
			})

		filter("system:linux")
			disablewarnings("unused-result")
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
		vpaths({
			["Source files/*"] = directory .. "/mathlib/*.cpp",
			["Header files/*"] = {
				directory .. "/mathlib/*.h",
				directory .. "/public/mathlib/*.h"
			}
		})
		IncludeSDKCommonInternal(directory)
		files({
			directory .. "/mathlib/expressioncalculator.cpp",
			directory .. "/mathlib/color_conversion.cpp",
			directory .. "/mathlib/cholesky.cpp",
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
			directory .. "/mathlib/kdop.cpp",
			directory .. "/mathlib/imagequant.cpp",
			directory .. "/mathlib/spherical.cpp",
			directory .. "/mathlib/polyhedron.cpp",
			directory .. "/mathlib/quantize.cpp",
			directory .. "/mathlib/randsse.cpp",
			directory .. "/mathlib/simdvectormatrix.cpp",
			directory .. "/mathlib/vmatrix.cpp",
			directory .. "/mathlib/almostequal.cpp",
			directory .. "/mathlib/simplex.cpp",
			directory .. "/mathlib/eigen.cpp",
			directory .. "/mathlib/box_buoyancy.cpp",
			directory .. "/mathlib/camera.cpp",
			directory .. "/mathlib/planefit.cpp",
			directory .. "/mathlib/polygon.cpp",
			directory .. "/mathlib/volumeculler.cpp",
			directory .. "/mathlib/transform.cpp",
			directory .. "/mathlib/sphere.cpp",
			directory .. "/mathlib/capsule.cpp",
			directory .. "/mathlib/noisedata.h",
			directory .. "/mathlib/sse.h",
			directory .. "/public/mathlib/anorms.h",
			directory .. "/public/mathlib/bumpvects.h",
			directory .. "/public/mathlib/beziercurve.h",
			directory .. "/public/mathlib/camera.h",
			directory .. "/public/mathlib/compressed_3d_unitvec.h",
			directory .. "/public/mathlib/compressed_light_cube.h",
			directory .. "/public/mathlib/compressed_vector.h",
			directory .. "/public/mathlib/expressioncalculator.h",
			directory .. "/public/mathlib/halton.h",
			directory .. "/public/mathlib/IceKey.H",
			directory .. "/public/mathlib/lightdesc.h",
			directory .. "/public/mathlib/math_pfns.h",
			directory .. "/public/mathlib/mathlib.h",
			directory .. "/public/mathlib/noise.h",
			directory .. "/public/mathlib/polyhedron.h",
			directory .. "/public/mathlib/quantize.h",
			directory .. "/public/mathlib/simdvectormatrix.h",
			directory .. "/public/mathlib/spherical_geometry.h",
			directory .. "/public/mathlib/ssemath.h",
			directory .. "/public/mathlib/ssequaternion.h",
			directory .. "/public/mathlib/vector.h",
			directory .. "/public/mathlib/vector2d.h",
			directory .. "/public/mathlib/vector4d.h",
			directory .. "/public/mathlib/vmatrix.h",
			directory .. "/public/mathlib/vplane.h",
			directory .. "/public/mathlib/simplex.h",
			directory .. "/public/mathlib/eigen.h",
			directory .. "/public/mathlib/box_buoyancy.h",
			directory .. "/public/mathlib/cholesky.h",
			directory .. "/public/mathlib/planefit.h",
			directory .. "/public/mathlib/intvector3d.h",
			directory .. "/public/mathlib/polygon.h",
			directory .. "/public/mathlib/quadric.h",
			directory .. "/public/mathlib/volumeculler.h",
			directory .. "/public/mathlib/transform.h",
			directory .. "/public/mathlib/sphere.h",
			directory .. "/public/mathlib/capsule.h"
		})

		filter("system:linux")
			disablewarnings("ignored-attributes")

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
			directory .. "/public/tier1"
		})
		vpaths({["Source files/*"] = directory .. "/raytrace/*.cpp"})
		IncludeSDKCommonInternal(directory)
		files({
			directory .. "/raytrace/raytrace.cpp",
			directory .. "/raytrace/trace2.cpp",
			directory .. "/raytrace/trace3.cpp"
		})

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
			directory .. "/public/tier1"
		})
		vpaths({
			["Source files/*"] = directory .. "/bitmap/*.cpp",
			["Header files/*"] = directory .. "/public/bitmap/*.h"
		})
		IncludeSDKCommonInternal(directory)
		files({
			directory .. "/bitmap/bitmap.cpp",
			directory .. "/bitmap/colorconversion.cpp",
			directory .. "/bitmap/floatbitmap.cpp",
			directory .. "/bitmap/floatbitmap2.cpp",
			directory .. "/bitmap/floatbitmap3.cpp",
			directory .. "/bitmap/floatbitmap_bilateralfilter.cpp",
			directory .. "/bitmap/floatcubemap.cpp",
			directory .. "/bitmap/imageformat.cpp",
			directory .. "/bitmap/psd.cpp",
			directory .. "/bitmap/psheet.cpp",
			directory .. "/bitmap/resample.cpp",
			directory .. "/bitmap/tgaloader.cpp",
			directory .. "/bitmap/texturepacker.cpp",
			directory .. "/bitmap/tgawriter.cpp",
			directory .. "/public/bitmap/bitmap.h",
			directory .. "/public/bitmap/floatbitmap.h",
			directory .. "/public/bitmap/imageformat.h",
			directory .. "/public/bitmap/imageformat_declarations.h",
			directory .. "/public/bitmap/psd.h",
			directory .. "/public/bitmap/psheet.h",
			directory .. "/public/bitmap/texturepacker.h",
			directory .. "/public/bitmap/tgaloader.h",
			directory .. "/public/bitmap/tgawriter.h",
			directory .. "/public/bitmap/stb_dxt.h"
		})

		filter("system:windows")
			files(directory .. "/bitmap/floatbitmap4.cpp")

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
			directory .. "/public/tier1"
		})
		vpaths({
			["Source files/*"] = directory .. "/vtf/*.cpp",
			["Header files/*"] = {
				directory .. "/vtf/*.h",
				directory .. "/public/vtf/*.h",
			}
		})
		IncludeSDKCommonInternal(directory)
		files({
			directory .. "/vtf/vtf.cpp",
			directory .. "/vtf/convert_x360.cpp",
			directory .. "/vtf/cvtf.h",
			directory .. "/public/vtf/vtf.h"
		})

		filter("system:windows")
			files({
				directory .. "/vtf/s3tc_decode.cpp",
				directory .. "/vtf/s3tc_decode.h"
			})

	project(_project.name)
end

function IncludeSteamAPI(directory)
	IncludePackage("steamapi")

	local _workspace = project().workspace

	directory = GetSDKPath(directory)

	sysincludedirs(directory .. "/public/steam")

	links("steam_api")
end
