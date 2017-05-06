newoption({
	trigger = "sourcesdk",
	description = "Sets the path to the SourceSDK directory",
	value = "path to SourceSDK directory"
})

local function GetSDKPath(directory)
	directory = directory or _OPTIONS["sourcesdk"] or os.getenv("SOURCE_SDK") or DEFAULT_SOURCESDK_DIRECTORY
	if directory == nil then
		error("you didn't supply a path to your SourceSDK copy")
	end

	directory = CleanPath(directory)
	local dir = path.getabsolute(directory)
	if not os.isdir(dir) then
		error(dir .. " doesn't exist (SourceSDK)")
	end

	return directory
end

local function IncludeSDKCommonInternal(directory)
	local _project = project()
	local _workspace = _project.workspace

	defines(_project.serverside and "GAME_DLL" or "CLIENT_DLL")
	includedirs({
		directory .. "/common",
		directory .. "/public"
	})

	if _project.serverside then
		includedirs({
			directory .. "/game/server",
			directory .. "/game/shared"
		})
	else
		includedirs({
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
		defines({"COMPILER_GCC", "POSIX", "_POSIX", "LINUX", "_LINUX", "GNUC", "NO_MALLOC_OVERRIDE"})
		prelinkcommands("mkdir -p " .. path.getabsolute(_workspace.directory) .. "/bin")

	filter("system:macosx")
		defines({"COMPILER_GCC", "POSIX", "_POSIX", "OSX", "GNUC", "NO_MALLOC_OVERRIDE"})
		libdirs(directory .. "/lib/public/osx32")

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

	includedirs(directory .. "/public/tier0")

	filter("system:windows or macosx")
		links("tier0")

	filter("system:linux")
		local library = _project.serverside and "libtier0_srv.so" or "libtier0.so"
		prelinkcommands("ln -f " .. path.getabsolute(directory) .. "/lib/public/linux32/libtier0.so " .. path.getabsolute(_workspace.directory) .. "/bin/" .. library)
		-- The flag --no-as-needed forces tier0 to be linked.
		-- This was added because certain modules (gm_luaerror for example)
		-- were having these binaries dropped when linking with SourceSDK.
		-- Hopefully only tier0 will need these.
		linkoptions({"-Wl,--no-as-needed", "bin/" .. library, "-Wl,--as-needed"})

	filter({})
end

function IncludeSDKTier1(directory)
	IncludePackage("sdktier1")

	local _project = project()
	local _workspace = _project.workspace

	directory = GetSDKPath(directory)

	includedirs(directory .. "/public/tier1")
	links("tier1")

	filter("system:windows")
		links({"vstdlib", "ws2_32", "rpcrt4"})

	filter("system:linux")
		local library = _project.serverside and "libvstdlib_srv.so" or "libvstdlib.so"
		prelinkcommands("ln -f " .. path.getabsolute(directory) .. "/lib/public/linux32/libvstdlib.so " .. path.getabsolute(_workspace.directory) .. "/bin/" .. library)
		linkoptions("bin/" .. library)

	filter("system:macosx")
		links({"vstdlib", "iconv"})

	project("tier1")
		kind("StaticLib")
		warnings("Default")
		flags("GNU++11")
		defines({"TIER1_STATIC_LIB", "_CRT_SECURE_NO_WARNINGS"})
		includedirs({
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

		filter("system:windows")
			defines({"_DLL_EXT=dll", "WIN32"})
			files(directory .. "/tier1/processor_detect.cpp")

		filter("system:linux")
			defines({"_DLL_EXT=so", "COMPILER_GCC", "POSIX", "_POSIX", "LINUX", "_LINUX", "GNUC", "NO_MALLOC_OVERRIDE"})
			files({
				directory .. "/tier1/processor_detect_linux.cpp",
				directory .. "/tier1/qsort_s.cpp",
				directory .. "/tier1/pathmatch.cpp"
			})

		filter("system:macosx")
			defines({"_DLL_EXT=dylib", "COMPILER_GCC", "POSIX", "_POSIX", "OSX", "GNUC", "NO_MALLOC_OVERRIDE"})
			files({
				directory .. "/tier1/processor_detect_linux.cpp",
				directory .. "/tier1/qsort_s.cpp",
				directory .. "/tier1/pathmatch.cpp"
			})

	project(_project.name)
end

function IncludeSDKTier2(directory)
	IncludePackage("sdktier2")

	directory = GetSDKPath(directory)

	includedirs(directory .. "/public/tier2")

	filter("system:windows")
		links("tier2")

	filter("system:macosx")
		linkoptions("-l:tier2.a")

	filter("system:linux")
		linkoptions(path.getabsolute(directory) .. "/lib/public/linux32/tier2.a")

	filter({})
end

function IncludeSDKTier3(directory)
	IncludePackage("sdktier3")

	directory = GetSDKPath(directory)

	includedirs(directory .. "/public/tier3")

	filter("system:windows")
		links("tier3")

	filter("system:macosx")
		linkoptions("-l:tier3.a")

	filter("system:linux")
		linkoptions(path.getabsolute(directory) .. "/lib/public/linux32/tier3.a")

	filter({})
end

function IncludeSDKMathlib(directory)
	IncludePackage("sdkmathlib")

	local _project = project()

	directory = GetSDKPath(directory)

	includedirs(directory .. "/public/mathlib")
	links("mathlib")

	project("mathlib")
		kind("StaticLib")
		warnings("Default")
		defines("MATHLIB_LIB")
		includedirs({
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

		filter("system:windows or linux")
			files(directory .. "/mathlib/3dnow.cpp")

		filter("system:windows")
			defines("WIN32")

		filter("system:linux")
			defines({"COMPILER_GCC", "POSIX", "_POSIX", "LINUX", "_LINUX", "GNUC", "NO_MALLOC_OVERRIDE"})

		filter("system:macosx")
			defines({"COMPILER_GCC", "POSIX", "_POSIX", "OSX", "GNUC", "NO_MALLOC_OVERRIDE"})

	project(_project.name)
end

function IncludeSDKRaytrace(directory)
	IncludePackage("sdkraytrace")

	local _project = project()

	directory = GetSDKPath(directory)

	links("raytrace")

	project("raytrace")
		kind("StaticLib")
		warnings("Default")
		includedirs({
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

		filter("system:windows")
			defines("WIN32")

		filter("system:linux")
			defines({"COMPILER_GCC", "POSIX", "_POSIX", "LINUX", "_LINUX", "GNUC", "NO_MALLOC_OVERRIDE"})

		filter("system:macosx")
			defines({"COMPILER_GCC", "POSIX", "_POSIX", "OSX", "GNUC", "NO_MALLOC_OVERRIDE"})

	project(_project.name)
end

function IncludeSteamAPI(directory)
	IncludePackage("steamapi")

	local _workspace = project().workspace

	directory = GetSDKPath(directory)

	includedirs(directory .. "/public/steam")

	filter("system:windows or macosx")
		links("steam_api")

	filter("system:linux")
		prelinkcommands("ln -f " .. path.getabsolute(directory) .. "/lib/public/linux32/libsteam_api.so " .. path.getabsolute(_workspace.directory) .. "/bin/libsteam_api.so")
		linkoptions("bin/libsteam_api.so")

	filter({})
end
