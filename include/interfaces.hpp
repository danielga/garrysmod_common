#pragma once

#include <helpers.hpp>
#include <interface.h>
#include <string>

namespace SourceSDK
{

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
		unload( !noload ),
		module( nullptr ),
		factory( nullptr )
	{
		std::string filename = name;
		if( filename.find( '.' ) == filename.npos )
			filename = helpers::GetBinaryFileName( name, libprefix, srvsuffix, extraprefix );

		module = Sys_LoadModule( filename.c_str( ), noload ? SYS_NOLOAD : SYS_NOFLAGS );
		if( module != nullptr )
			factory = Sys_GetFactory( module );
	}

	~FactoryLoader( )
	{
		if( unload && module != nullptr )
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
	bool unload;
	CSysModule *module;
	CreateInterfaceFn factory;
};

}