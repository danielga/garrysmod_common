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

local function AddCommon(folder)
	folder = GetSDKPath(folder)

	local curfilter = GetFilter()
	local nosystem = curfilter.system == nil

	filter({})

	defines(_PROJECT_SERVERSIDE and "GAME_DLL" or "CLIENT_DLL")
	includedirs({
		folder .. "/common",
		folder .. "/public"
	})

	if _PROJECT_SERVERSIDE then
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

	if nosystem or HasFilter(FILTER_WINDOWS) then
		filter(MergeFilters({"system:windows", curfilter.configurations}, curfilter.extra))
			defines("WIN32")
			libdirs(folder .. "/lib/public")

			if curfilter.configurations == nil or HasFilter(FILTER_DEBUG) then
				filter(MergeFilters({"system:windows", "configurations:Debug"}, curfilter.extra))
					linkoptions("/NODEFAULTLIB:\"libcmt\"")
			end
	end

	if nosystem or HasFilter(FILTER_LINUX) then
		filter(MergeFilters({"system:linux", curfilter.configurations}, curfilter.extra))
			defines({"COMPILER_GCC", "POSIX", "_POSIX", "LINUX", "_LINUX", "GNUC", "NO_MALLOC_OVERRIDE"})
			libdirs(folder .. "/lib/public/linux32")
	end

	if nosystem or HasFilter(FILTER_MACOSX) then
		filter(MergeFilters({"system:macosx", curfilter.configurations}, curfilter.extra))
			defines({"COMPILER_GCC", "POSIX", "_POSIX", "OSX", "GNUC", "NO_MALLOC_OVERRIDE"})
			libdirs(folder .. "/lib/public/osx32")
	end

	filter(curfilter.patterns)

	_SOURCE_SDK_INCLUDED = true
end

local mkdir_bin = false
function IncludeSDKTier0(folder)
	folder = GetSDKPath(folder)

	AddCommon(folder)

	local curfilter = GetFilter()
	local nosystem = curfilter.system == nil

	filter({})

	includedirs(folder .. "/public/tier0")

	if nosystem or HasFilter(FILTER_WINDOWS) then
		filter(MergeFilters({"system:windows", curfilter.configurations}, curfilter.extra))
			links("tier0")
	end

	if nosystem or HasFilter(FILTER_LINUX) then
		filter(MergeFilters({"system:linux", curfilter.configurations}, curfilter.extra))
			prelinkcommands({
				not mkdir_bin and "mkdir -p " .. path.getabsolute(folder) .. "/lib/public/linux32/bin" or nil,
				"ln -f " .. path.getabsolute(folder) .. "/lib/public/linux32/libtier0.so " .. path.getabsolute(folder) .. "/lib/public/linux32/bin/libtier0.so",
				"ln -f " .. path.getabsolute(folder) .. "/lib/public/linux32/libtier0.so " .. path.getabsolute(folder) .. "/lib/public/linux32/bin/libtier0_srv.so"
			})
			mkdir_bin = true
			linkoptions("-l:bin/" .. (_PROJECT_SERVERSIDE and "libtier0_srv.so" or "libtier0.so"))
	end

	if nosystem or HasFilter(FILTER_MACOSX) then
		filter(MergeFilters({"system:macosx", curfilter.configurations}, curfilter.extra))
			links("tier0")
	end

	filter(curfilter.patterns)
end

function IncludeSDKTier1(folder)
	folder = GetSDKPath(folder)

	AddCommon(folder)

	local name = project().name
	local curfilter = GetFilter()
	local nosystem = curfilter.system == nil

	filter({})

	includedirs(folder .. "/public/tier1")
	links("tier1")

	if nosystem or HasFilter(FILTER_WINDOWS) then
		filter(MergeFilters({"system:windows", curfilter.configurations}, curfilter.extra))
			links({"vstdlib", "ws2_32", "rpcrt4"})
	end

	if nosystem or HasFilter(FILTER_LINUX) then
		filter(MergeFilters({"system:linux", curfilter.configurations}, curfilter.extra))
			prelinkcommands({
				not mkdir_bin and "mkdir -p " .. path.getabsolute(folder) .. "/lib/public/linux32/bin" or nil,
				"ln -f " .. path.getabsolute(folder) .. "/lib/public/linux32/libvstdlib.so " .. path.getabsolute(folder) .. "/lib/public/linux32/bin/libvstdlib.so",
				"ln -f " .. path.getabsolute(folder) .. "/lib/public/linux32/libvstdlib.so " .. path.getabsolute(folder) .. "/lib/public/linux32/bin/libvstdlib_srv.so"
			})
			mkdir_bin = true
			linkoptions("-l:bin/" .. (_PROJECT_SERVERSIDE and "libvstdlib_srv.so" or "libvstdlib.so"))
	end

	if nosystem or HasFilter(FILTER_MACOSX) then
		filter(MergeFilters({"system:macosx", curfilter.configurations}, curfilter.extra))
			links("vstdlib")
	end

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
		AddCommon(folder)
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

	project(name)

	filter(curfilter.patterns)
end

function IncludeSDKTier2(folder)
	folder = GetSDKPath(folder)

	AddCommon(folder)

	local curfilter = GetFilter()

	filter({})

	includedirs(folder .. "/public/tier2")
	links("tier2")

	filter(curfilter.patterns)
end

function IncludeSDKTier3(folder)
	folder = GetSDKPath(folder)

	AddCommon(folder)

	local curfilter = GetFilter()

	filter({})

	includedirs(folder .. "/public/tier3")
	links("tier3")

	filter(curfilter.patterns)
end

function IncludeSDKMathlib(folder)
	folder = GetSDKPath(folder)

	AddCommon(folder)

	local curfilter = GetFilter()

	filter({})

	includedirs(folder .. "/public/mathlib")
	links("mathlib")

	project("mathlib")
		kind("StaticLib")
		warnings("Default")
		defines("MATHLIB_LIB")
		includedirs(folder .. "/public/mathlib")
		vpaths({["Source files"] = folder .. "/mathlib/**.cpp"})
		AddCommon(folder)
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
			files({	folder .. "/mathlib/3dnow.cpp"})

	filter(curfilter.patterns)
end

function IncludeSDKRaytrace(folder)
	folder = GetSDKPath(folder)

	AddCommon(folder)

	local curfilter = GetFilter()

	filter({})

	links("raytrace")

	project("raytrace")
		kind("StaticLib")
		warnings("Default")
		includedirs(folder .. "/utils/common")
		vpaths({["Source files"] = folder .. "/raytrace/**.cpp"})
		AddCommon(folder)
		files({
			folder .. "/raytrace/raytrace.cpp",
			folder .. "/raytrace/trace2.cpp",
			folder .. "/raytrace/trace3.cpp"
		})

	filter(curfilter.patterns)
end

function IncludeSourceSDK(folder)
	folder = GetSDKPath(folder)
	IncludeSDKTier0(folder)
	IncludeSDKTier1(folder)
end

function IncludeSteamAPI(folder)
	folder = GetSDKPath(folder)

	AddCommon(folder)

	local curfilter = GetFilter()

	filter({})

	includedirs(folder .. "/public/steam")

	if nosystem or HasFilter(FILTER_WINDOWS) then
		filter(MergeFilters({"system:windows", curfilter.configurations}, curfilter.extra))
			links("steam_api")
	end

	if nosystem or HasFilter(FILTER_LINUX) then
		filter(MergeFilters({"system:linux", curfilter.configurations}, curfilter.extra))
			prelinkcommands({
				not mkdir_bin and "mkdir -p " .. path.getabsolute(folder) .. "/lib/public/linux32/bin" or nil,
				"ln -f " .. path.getabsolute(folder) .. "/lib/public/linux32/libsteam_api.so " .. path.getabsolute(folder) .. "/lib/public/linux32/bin/libsteam_api.so"
			})
			mkdir_bin = true
			linkoptions("-l:bin/libsteam_api.so")
	end

	if nosystem or HasFilter(FILTER_MACOSX) then
		filter(MergeFilters({"system:macosx", curfilter.configurations}, curfilter.extra))
			links("steam_api")
	end

	filter(curfilter.patterns)
end
