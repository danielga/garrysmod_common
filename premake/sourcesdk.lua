newoption({
	trigger = "sourcesdk",
	description = "Sets the path to the SourceSDK directory",
	value = "path to SourceSDK directory"
})

local function GetSDKPath(folder)
	folder = folder or _OPTIONS["sourcesdk"] or os.getenv("SOURCE_SDK") or DEFAULT_SOURCESDK_FOLDER

	if folder == nil then
		error("you didn't supply a path to your SourceSDK copy")
	end

	folder = CleanPath(folder)
	local dir = path.getabsolute(folder)
	if not os.isdir(dir) then
		error(dir .. " doesn't exist (SourceSDK)")
	end

	return folder
end

local function IncludeSDKCommonInternal(folder)
	filter({})

	defines(_PROJECT.serverside and "GAME_DLL" or "CLIENT_DLL")
	includedirs({
		folder .. "/common",
		folder .. "/public"
	})

	if _PROJECT.serverside then
		includedirs({
			folder .. "/game/server",
			folder .. "/game/shared"
		})
	else
		includedirs({
			folder .. "/game/client",
			folder .. "/game/shared"
		})
	end

	filter("system:windows")
		defines("WIN32")
		libdirs(folder .. "/lib/public")

		filter({"system:windows", "configurations:Debug"})
			linkoptions("/NODEFAULTLIB:\"libcmt\"")

	filter("system:linux")
		defines({"COMPILER_GCC", "POSIX", "_POSIX", "LINUX", "_LINUX", "GNUC", "NO_MALLOC_OVERRIDE"})
		libdirs(folder .. "/lib/public/linux32")
		prelinkcommands("mkdir -p " .. path.getabsolute(folder) .. "/lib/public/linux32/bin")

	filter("system:macosx")
		defines({"COMPILER_GCC", "POSIX", "_POSIX", "OSX", "GNUC", "NO_MALLOC_OVERRIDE"})
		libdirs(folder .. "/lib/public/osx32")

	filter({})
end

function IncludeSDKCommon(folder)
	IncludePackage("sdkcommon")

	folder = GetSDKPath(folder)

	filter({})

	defines(_PROJECT.serverside and "GAME_DLL" or "CLIENT_DLL")
	includedirs({
		folder .. "/common",
		folder .. "/public"
	})

	if _PROJECT.serverside then
		includedirs({
			folder .. "/game/server",
			folder .. "/game/shared"
		})
	else
		includedirs({
			folder .. "/game/client",
			folder .. "/game/shared"
		})
	end

	filter("system:windows")
		defines("WIN32")
		libdirs(folder .. "/lib/public")

		filter({"system:windows", "configurations:Debug"})
			linkoptions("/NODEFAULTLIB:\"libcmt\"")

	filter("system:linux")
		defines({"COMPILER_GCC", "POSIX", "_POSIX", "LINUX", "_LINUX", "GNUC", "NO_MALLOC_OVERRIDE"})
		libdirs(folder .. "/lib/public/linux32")
		prelinkcommands("mkdir -p " .. path.getabsolute(folder) .. "/lib/public/linux32/bin")

	filter("system:macosx")
		defines({"COMPILER_GCC", "POSIX", "_POSIX", "OSX", "GNUC", "NO_MALLOC_OVERRIDE"})
		libdirs(folder .. "/lib/public/osx32")

	filter({})
end

function IncludeSDKTier0(folder)
	IncludePackage("sdktier0")

	folder = GetSDKPath(folder)

	filter({})

	includedirs(folder .. "/public/tier0")

	filter("system:windows or macosx")
		links("tier0")

	filter("system:linux")
		prelinkcommands({
			"ln -f " .. path.getabsolute(folder) .. "/lib/public/linux32/libtier0.so " .. path.getabsolute(folder) .. "/lib/public/linux32/bin/libtier0.so",
			"ln -f " .. path.getabsolute(folder) .. "/lib/public/linux32/libtier0.so " .. path.getabsolute(folder) .. "/lib/public/linux32/bin/libtier0_srv.so"
		})
		linkoptions("-l:bin/" .. (_PROJECT.serverside and "libtier0_srv.so" or "libtier0.so"))

	filter({})
end

function IncludeSDKTier1(folder)
	IncludePackage("sdktier1")

	folder = GetSDKPath(folder)

	filter({})

	includedirs(folder .. "/public/tier1")
	links("tier1")

	filter("system:windows")
		links({"vstdlib", "ws2_32", "rpcrt4"})

	filter("system:linux")
		prelinkcommands({
			"ln -f " .. path.getabsolute(folder) .. "/lib/public/linux32/libvstdlib.so " .. path.getabsolute(folder) .. "/lib/public/linux32/bin/libvstdlib.so",
			"ln -f " .. path.getabsolute(folder) .. "/lib/public/linux32/libvstdlib.so " .. path.getabsolute(folder) .. "/lib/public/linux32/bin/libvstdlib_srv.so"
		})
		linkoptions("-l:bin/" .. (_PROJECT.serverside and "libvstdlib_srv.so" or "libvstdlib.so"))

	filter("system:macosx")
		links("vstdlib")

	project("tier1")
		kind("StaticLib")
		warnings("Default")
		defines("TIER1_STATIC_LIB")
		includedirs({
			folder .. "/public/tier0",
			folder .. "/public/tier1"
		})
		vpaths({["Source files"] = {
			folder .. "/tier1/**.cpp",
			folder .. "/utils/lzma/C/**.c"
		}})
		IncludeSDKCommonInternal(folder)
		files({
			folder .. "/tier1/bitbuf.cpp",
			folder .. "/tier1/byteswap.cpp",
			folder .. "/tier1/characterset.cpp",
			folder .. "/tier1/checksum_crc.cpp",
			folder .. "/tier1/checksum_md5.cpp",
			folder .. "/tier1/checksum_sha1.cpp",
			folder .. "/tier1/commandbuffer.cpp",
			folder .. "/tier1/convar.cpp",
			folder .. "/tier1/datamanager.cpp",
			folder .. "/tier1/diff.cpp",
			folder .. "/tier1/generichash.cpp",
			folder .. "/tier1/ilocalize.cpp",
			folder .. "/tier1/interface.cpp",
			folder .. "/tier1/KeyValues.cpp",
			folder .. "/tier1/kvpacker.cpp",
			folder .. "/tier1/lzmaDecoder.cpp",
			folder .. "/tier1/mempool.cpp",
			folder .. "/tier1/memstack.cpp",
			folder .. "/tier1/NetAdr.cpp",
			folder .. "/tier1/splitstring.cpp",
			folder .. "/tier1/rangecheckedvar.cpp",
			folder .. "/tier1/reliabletimer.cpp",
			folder .. "/tier1/stringpool.cpp",
			folder .. "/tier1/strtools.cpp",
			folder .. "/tier1/strtools_unicode.cpp",
			folder .. "/tier1/tier1.cpp",
			folder .. "/tier1/tokenreader.cpp",
			folder .. "/tier1/sparsematrix.cpp",
			folder .. "/tier1/uniqueid.cpp",
			folder .. "/tier1/utlbuffer.cpp",
			folder .. "/tier1/utlbufferutil.cpp",
			folder .. "/tier1/utlstring.cpp",
			folder .. "/tier1/utlsymbol.cpp",
			folder .. "/tier1/utlbinaryblock.cpp",
			folder .. "/tier1/snappy.cpp",
			folder .. "/tier1/snappy-sinksource.cpp",
			folder .. "/tier1/snappy-stubs-internal.cpp",
			folder .. "/utils/lzma/C/LzmaDec.c"
		})

		filter("system:windows")
			defines("_DLL_EXT=dll")
			files(folder .. "/tier1/processor_detect.cpp")

		filter("system:linux")
			defines("_DLL_EXT=so")
			files({
				folder .. "/tier1/processor_detect_linux.cpp",
				folder .. "/tier1/qsort_s.cpp",
				folder .. "/tier1/pathmatch.cpp"
			})

		filter("system:macosx")
			defines("_DLL_EXT=dylib")
			files({
				folder .. "/tier1/processor_detect_linux.cpp",
				folder .. "/tier1/qsort_s.cpp",
				folder .. "/tier1/pathmatch.cpp"
			})

		filter("action:gmake")
			buildoptions("-std=gnu++11")

	project(_PROJECT.name)
end

function IncludeSDKTier2(folder)
	IncludePackage("sdktier2")

	folder = GetSDKPath(folder)

	filter({})

	includedirs(folder .. "/public/tier2")
	links("tier2")
end

function IncludeSDKTier3(folder)
	IncludePackage("sdktier3")

	folder = GetSDKPath(folder)

	filter({})

	includedirs(folder .. "/public/tier3")
	links("tier3")
end

function IncludeSDKMathlib(folder)
	IncludePackage("sdkmathlib")

	folder = GetSDKPath(folder)

	filter({})

	includedirs(folder .. "/public/mathlib")
	links("mathlib")

	project("mathlib")
		kind("StaticLib")
		warnings("Default")
		defines("MATHLIB_LIB")
		includedirs(folder .. "/public/mathlib")
		vpaths({["Source files"] = folder .. "/mathlib/**.cpp"})
		IncludeSDKCommonInternal(folder)
		files({
			folder .. "/mathlib/color_conversion.cpp",
			folder .. "/mathlib/halton.cpp",
			folder .. "/mathlib/lightdesc.cpp",
			folder .. "/mathlib/mathlib_base.cpp",
			folder .. "/mathlib/powsse.cpp",
			folder .. "/mathlib/sparse_convolution_noise.cpp",
			folder .. "/mathlib/sseconst.cpp",
			folder .. "/mathlib/sse.cpp",
			folder .. "/mathlib/ssenoise.cpp",
			folder .. "/mathlib/anorms.cpp",
			folder .. "/mathlib/bumpvects.cpp",
			folder .. "/mathlib/IceKey.cpp",
			folder .. "/mathlib/imagequant.cpp",
			folder .. "/mathlib/polyhedron.cpp",
			folder .. "/mathlib/quantize.cpp",
			folder .. "/mathlib/randsse.cpp",
			folder .. "/mathlib/spherical.cpp",
			folder .. "/mathlib/simdvectormatrix.cpp",
			folder .. "/mathlib/vector.cpp",
			folder .. "/mathlib/vmatrix.cpp",
			folder .. "/mathlib/almostequal.cpp"
		})

		filter("system:windows or linux")
			files(folder .. "/mathlib/3dnow.cpp")

	project(_PROJECT.name)
end

function IncludeSDKRaytrace(folder)
	IncludePackage("sdkraytrace")

	folder = GetSDKPath(folder)

	filter({})

	links("raytrace")

	project("raytrace")
		kind("StaticLib")
		warnings("Default")
		includedirs(folder .. "/utils/common")
		vpaths({["Source files"] = folder .. "/raytrace/**.cpp"})
		IncludeSDKCommonInternal(folder)
		files({
			folder .. "/raytrace/raytrace.cpp",
			folder .. "/raytrace/trace2.cpp",
			folder .. "/raytrace/trace3.cpp"
		})

	project(_PROJECT.name)
end

function IncludeSteamAPI(folder)
	IncludePackage("steamapi")

	folder = GetSDKPath(folder)

	filter({})

	includedirs(folder .. "/public/steam")

	filter("system:windows or macosx")
		links("steam_api")

	filter("system:linux")
		prelinkcommands("ln -f " .. path.getabsolute(folder) .. "/lib/public/linux32/libsteam_api.so " .. path.getabsolute(folder) .. "/lib/public/linux32/bin/libsteam_api.so")
		linkoptions("-l:bin/libsteam_api.so")

	filter({})
end
