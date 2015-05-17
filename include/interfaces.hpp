#pragma once

#include <interface.h>
#include <string>

namespace SourceSDK
{

class FactoryLoader
{
public:
	FactoryLoader( const std::string &name, bool serverside = true, bool noload = true ) :
		module( nullptr ),
		factory( nullptr )
	{
		std::string filename = name;

#if defined _WIN32

		(void)serverside;
		filename += ".dll";

#elif defined __linux

		filename += serverside ? "_srv.so" : ".so";

#elif defined __APPLE__

		(void)serverside;
		filename += ".dylib";

#endif

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