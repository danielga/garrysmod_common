#pragma once

#include "Platform.hpp"
#include "ModuleLoader.hpp"

namespace SourceSDK
{

class FactoryLoader
{
public:
	typedef void *( *CreateInterfaceFn )( const char *, int * );

	FactoryLoader( const std::string &name ) :
		module( name ),
		factory( nullptr )
	{
		if( module.IsValid( ) )
			factory = reinterpret_cast<CreateInterfaceFn>( module.GetSymbol( "CreateInterface" ) );
	}

	DEPRECATED_WITH_SUBSTITUTE( "FactoryLoader( const std::string &name )" )
	FactoryLoader( const std::string &name, bool, bool = true, const std::string & = "", bool = true ) :
		FactoryLoader( name )
	{ }

	bool IsValid( ) const
	{
		return module.IsValid( ) && factory != nullptr;
	}

	const ModuleLoader &GetModuleLoader( ) const
	{
		return module;
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
	ModuleLoader module;
	CreateInterfaceFn factory;
};

}
