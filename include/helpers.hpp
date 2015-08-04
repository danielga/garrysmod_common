#pragma once

#include <string>

namespace helpers
{

inline std::string GetBinaryFileName(
	const std::string &name,
	bool libprefix = true,
	bool srvsuffix = true,
	const std::string &extraprefix = ""
)
{

#if defined _WIN32

	(void)libprefix;
	(void)srvsuffix;
	return extraprefix + name + ".dll";

#elif defined __linux

	return extraprefix + ( libprefix ? "lib" : "" ) + name + ( srvsuffix ? "_srv.so" : ".so" );

#elif defined __APPLE__

	(void)libprefix;
	(void)srvsuffix;
	return extraprefix + name + ".dylib";

#endif

}

}
