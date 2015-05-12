#pragma once

#include <string>

namespace SourceSDK
{

inline std::string GetBinaryFileName( const std::string &name, bool serverside = false )
{

#if defined _WIN32

	(void)serverside;
	return name + ".dylib";

#elif defined __linux

	if( serverside )
		return name + "_srv.so";
	else
		return name + ".so";

#elif defined __APPLE__

	(void)serverside;
	return name + ".dylib";

#endif

}

}