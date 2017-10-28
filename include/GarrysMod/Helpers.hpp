#pragma once

#include <string>
#include <Platform.hpp>

namespace Helpers
{

inline std::string GetBinaryFileName(
	const std::string &name,
	bool libprefix = true,
	bool srvsuffix = true,
	const std::string &extraprefix = ""
)
{

#if defined SYSTEM_WINDOWS

	(void)libprefix;
	(void)srvsuffix;
	return extraprefix + name + ".dll";

#elif defined SYSTEM_LINUX

	return extraprefix + ( libprefix ? "lib" : "" ) + name + ( srvsuffix ? "_srv.so" : ".so" );

#elif defined SYSTEM_MACOSX

	(void)libprefix;
	(void)srvsuffix;
	return extraprefix + name + ".dylib";

#endif

}

}
