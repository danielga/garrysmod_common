#pragma once

#include <string>

namespace SourceSDK
{

class ModuleLoader
{
public:
	ModuleLoader( const std::string &name ) : module( LoadModule( name ) ) { }

	~ModuleLoader( )
	{
		if( module != nullptr )
		{
			ReleaseModule( module );
			module = nullptr;
		}
	}

	bool IsValid( ) const
	{
		return module != nullptr;
	}

	void *GetModule( ) const
	{
		return module;
	}

	void *GetSymbol( const std::string &symbol ) const
	{
		if( !IsValid( ) )
		{
			return nullptr;
		}
		
		return GetSymbol( module, symbol );
	}

private:
	static void *LoadModule( const std::string &name );
	static bool ReleaseModule( void *module );
	static void *GetSymbol( void *module, const std::string &name );

	void *module;
};

}
