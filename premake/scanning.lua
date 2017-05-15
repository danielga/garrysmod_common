function IncludeScanning()
	IncludePackage("scanning")

	local directory = _GARRYSMOD_COMMON_DIRECTORY .. "/scanning"

	filter({})

	includedirs(directory)
	files({
		directory .. "/*.hpp",
		directory .. "/*.cpp"
	})
	vpaths({
		["Header files/*"] = directory .. "/*.hpp",
		["Source files/*"] = directory .. "/*.cpp"
	})

	filter("system:linux or macosx")
		links("dl")

	filter("system:macosx")
		links("CoreServices.framework")

	filter({})
end

function IncludeDisassembler(full)
	IncludePackage("disassembler")

	local directory = _GARRYSMOD_COMMON_DIRECTORY .. "/scanning/distorm"

	filter({})

	if full then
		defines("DISTORM_STATIC")
	else
		defines({"DISTORM_STATIC", "DISTORM_LIGHT"})
	end

	includedirs(directory .. "/include")
	files({
		directory .. "/include/*.h",
		directory .. "/src/*.h",
		directory .. "/src/*.c"
	})
	vpaths({
		["Header files/distorm"] = {directory .. "/include/*.h", directory .. "/src/*.h"},
		["Source files/distorm"] = directory .. "/src/*.c"
	})

	filter({})
end
