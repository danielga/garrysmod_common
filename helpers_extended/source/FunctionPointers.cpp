#include "FunctionPointers.hpp"
#include "Symbols.hpp"

#include <GarrysMod/FactoryLoader.hpp>

#include <scanning/symbolfinder.hpp>

namespace FunctionPointers
{
	static SourceSDK::FactoryLoader client_loader( "client" );
	static SourceSDK::FactoryLoader server_loader( "server" );
	static SourceSDK::FactoryLoader engine_loader( "engine" );
	static SourceSDK::FactoryLoader dedicated_loader( "dedicated" );
	static SourceSDK::FactoryLoader filesystem_loader( "filesystem_stdio" );
	static SourceSDK::FactoryLoader lua_shared_loader( "lua_shared" );

	static SymbolFinder symbol_finder;

	HandleClientLuaError_t HandleClientLuaError( )
	{
		static HandleClientLuaError_t func_pointer = nullptr;
		if( func_pointer != nullptr )
			return func_pointer;

		for( const auto &symbol : Symbols::HandleClientLuaError )
		{
			func_pointer = reinterpret_cast<HandleClientLuaError_t>( symbol_finder.Resolve(
				server_loader.GetModule( ), symbol.name.c_str( ), symbol.length
			) );
			if( func_pointer != nullptr )
				break;
		}

		return func_pointer;
	}

	CreateInterfaceFn FileSystemFactory( )
	{
		static CreateInterfaceFn func_pointer = nullptr;
		if( func_pointer != nullptr )
			return func_pointer;

		for( const auto &symbol : Symbols::FileSystemFactory )
		{
			func_pointer = reinterpret_cast<CreateInterfaceFn>( symbol_finder.Resolve(
				dedicated_loader.GetModule( ),
				symbol.name.c_str( ),
				symbol.length
			) );
			if( func_pointer != nullptr )
				break;
		}

		return func_pointer;
	}
}
