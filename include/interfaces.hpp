#pragma once

#include <interface.h>
#include <string>

namespace SourceSDK
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

class FactoryLoader
{
public:
	FactoryLoader(
		const std::string &name,
		bool libprefix = true,
		bool srvsuffix = true,
		const std::string &extraprefix = "",
		bool noload = true
	) :
		module( nullptr ),
		factory( nullptr )
	{
		std::string filename = GetBinaryFileName( name, libprefix, srvsuffix, extraprefix );
		module = Sys_LoadModule( filename.c_str( ), noload ? SYS_NOLOAD : SYS_NOFLAGS );
		if( module != nullptr )
			factory = Sys_GetFactory( module );
	}

	~FactoryLoader( )
	{
		if( module != nullptr )
			Sys_UnloadModule( module );
	}

	bool IsValid( ) const
	{
		return factory != nullptr;
	}

	CreateInterfaceFn GetFactory( ) const
	{
		return factory;
	}

	template<class Interface>
	Interface *GetInterface( const char *name ) const
	{
		return IsValid( ) ? static_cast<Interface *>( factory( name, nullptr ) ) : nullptr;
	}

private:
	CSysModule *module;
	CreateInterfaceFn factory;
};

}