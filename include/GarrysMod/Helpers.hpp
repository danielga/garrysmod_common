#pragma once

#include <string>
#include "../Platform.hpp"

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
	
inline std::wstring GetWideBinaryFileName(
	const std::wstring &name,
	bool libprefix = true,
	bool srvsuffix = true,
	const std::wstring &extraprefix = L""
)
{
	
#if defined SYSTEM_WINDOWS
	
	(void)libprefix;
	(void)srvsuffix;
	return extraprefix + name + L".dll";
	
#elif defined SYSTEM_LINUX
	
	return extraprefix + ( libprefix ? L"lib" : L"" ) + name + ( srvsuffix ? L"_srv.so" : L".so" );
	
#elif defined SYSTEM_MACOSX
	
	(void)libprefix;
	(void)srvsuffix;
	return extraprefix + name + L".dylib";
	
#endif
	
}

}
