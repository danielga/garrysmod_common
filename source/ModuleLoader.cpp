#include "GarrysMod/ModuleLoader.hpp"
#include "Platform.hpp"

#if defined SYSTEM_WINDOWS

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>

#elif defined SYSTEM_LINUX || defined SYSTEM_MACOSX

#include <dlfcn.h>

#endif

#include <vector>

namespace SourceSDK
{

void *ModuleLoader::LoadModule( const std::string &name )
{

#if defined SYSTEM_WINDOWS

	const std::vector<std::string> search_paths = {

#if defined ARCHITECTURE_X86_64

		"bin/win64/%s_s.dll",
		"bin/win64/%s.dll",

#elif defined ARCHITECTURE_X86

		"garrysmod/bin/%s.dll",
		"bin/%s_s.dll",
		"bin/%s.dll"

#endif

	};

	HMODULE module = nullptr;
	for( const std::string &search_path : search_paths )
	{
		char buffer[256] = { };
		if( snprintf( buffer, sizeof( buffer ), search_path.c_str( ), name.c_str( ) ) >= sizeof( buffer ) )
		{
			// somehow truncated
			continue;
		}

		if( !GetModuleHandleExA( 0, buffer, &module ) )
		{
			module = nullptr;
		}

		if( module != nullptr )
		{
			break;
		}
	}

#elif defined SYSTEM_LINUX || defined SYSTEM_MACOSX

	const std::vector<std::string> search_paths = {

#if defined SYSTEM_LINUX

#if defined ARCHITECTURE_X86_64

		"bin/linux64/lib%s_client.so",
		"bin/linux64/%s_client.so",
		"bin/linux64/lib%s.so",
		"bin/linux64/%s.so",

#elif defined ARCHITECTURE_X86

		"bin/linux32/lib%s.so",
		"bin/linux32/%s.so",
		"garrysmod/bin/%s.so",
		"bin/lib%s_srv.so",
		"bin/%s_srv.so",
		"bin/lib%s.so",
		"bin/%s.so"

#endif

#elif defined SYSTEM_MACOSX

#if defined ARCHITECTURE_X86_64

		"lib%s.dylib",
		"%s.dylib"

#elif defined ARCHITECTURE_X86

		"garrysmod/bin/lib%s.dylib",
		"garrysmod/bin/%s.dylib",
		"bin/lib%s.dylib",
		"bin/%s.dylib"

#endif

#endif

	};

	void *module = nullptr;
	for( const std::string &search_path : search_paths )
	{
		char buffer[256] = { };
		if( snprintf( buffer, sizeof( buffer ), search_path.c_str( ), name.c_str( ) ) >= static_cast<int32_t>( sizeof( buffer ) ) )
		{
			// somehow truncated
			continue;
		}

		module = dlopen( buffer, RTLD_LAZY | RTLD_NOLOAD );
		if( module != nullptr )
		{
			break;
		}
}

#endif

	return module;
}

bool ModuleLoader::ReleaseModule( void *module )
{

#if defined SYSTEM_WINDOWS

	return ::FreeLibrary( reinterpret_cast<HMODULE>( module ) );

#elif defined SYSTEM_LINUX || defined SYSTEM_MACOSX

	return dlclose( module ) == 0;

#endif

}

void *ModuleLoader::GetSymbol( void *module, const std::string &name )
{

#if defined SYSTEM_WINDOWS

	return reinterpret_cast<void *>( GetProcAddress( reinterpret_cast<HMODULE>( module ), name.c_str( ) ) );

#elif defined SYSTEM_LINUX || defined SYSTEM_MACOSX

	return dlsym( module, name.c_str( ) );

#endif

}

}
