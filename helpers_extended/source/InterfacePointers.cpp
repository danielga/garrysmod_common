#define IS_SERVERSIDE

#include "InterfacePointers.hpp"
#include "FunctionPointers.hpp"
#include "Symbols.hpp"

#include <GarrysMod/FactoryLoader.hpp>

#include <scanning/symbolfinder.hpp>

namespace InterfacePointers
{
	static SourceSDK::FactoryLoader client_loader( "client" );
	static SourceSDK::FactoryLoader server_loader( "server" );
	static SourceSDK::FactoryLoader engine_loader( "engine" );
	static SourceSDK::FactoryLoader dedicated_loader( "dedicated" );
	static SourceSDK::FactoryLoader filesystem_loader( "filesystem" );
	static SourceSDK::FactoryLoader lua_shared_loader( "lua_shared" );

	static SymbolFinder symbol_finder;

	static const char filesystem_name[] = "VFileSystem022";
	static const char vengineserver_name[] = "VEngineServer021";
	static const char vengineclient_name[] = "VEngineClient015";

	namespace Client
	{
		IFileSystem *FileSystem( )
		{
			static IFileSystem *iface_pointer = nullptr;
			if( iface_pointer == nullptr )
			{
				IFileSystem **filesystem_ptr =
					reinterpret_cast<IFileSystem **>( symbol_finder.Resolve(
						client_loader.GetModule( ),
						Symbols::g_pFullFileSystem.name.c_str( ),
						Symbols::g_pFullFileSystem.length
					) );

				if( filesystem_ptr != nullptr )
					iface_pointer = *filesystem_ptr;
			}

			if( iface_pointer == nullptr )
				iface_pointer = filesystem_loader.GetInterface<IFileSystem>( filesystem_name );

			return iface_pointer;
		}
	}

	namespace Server
	{
		IFileSystem *FileSystem( )
		{
			static IFileSystem *iface_pointer = nullptr;
			if( iface_pointer != nullptr )
				return iface_pointer;

			FunctionPointers::CreateInterfaceFn factory = FunctionPointers::FileSystemFactory( );
			if( factory == nullptr )
			{
				IFileSystem **filesystem_ptr =
					reinterpret_cast<IFileSystem **>( symbol_finder.Resolve(
						dedicated_loader.GetModule( ),
						Symbols::g_pFullFileSystem.name.c_str( ),
						Symbols::g_pFullFileSystem.length
					) );
				if( filesystem_ptr == nullptr )
					filesystem_ptr =
					reinterpret_cast<IFileSystem **>( symbol_finder.Resolve(
						server_loader.GetModule( ),
						Symbols::g_pFullFileSystem.name.c_str( ),
						Symbols::g_pFullFileSystem.length
					) );

				if( filesystem_ptr != nullptr )
					iface_pointer = *filesystem_ptr;
			}
			else
			{
				iface_pointer =
					static_cast<IFileSystem *>( factory( filesystem_name, nullptr ) );
			}

			if( iface_pointer == nullptr )
				iface_pointer = filesystem_loader.GetInterface<IFileSystem>( filesystem_name );

			return iface_pointer;
		}
	}

	IVEngineServer *VEngineServer( )
	{
		static IVEngineServer *iface_pointer = nullptr;
		if( iface_pointer == nullptr )
			iface_pointer = engine_loader.GetInterface<IVEngineServer>( vengineserver_name );

		return iface_pointer;
	}

	IVEngineClient *VEngineClient( )
	{
		static IVEngineClient *iface_pointer = nullptr;
		if( iface_pointer == nullptr )
			iface_pointer = engine_loader.GetInterface<IVEngineClient>( vengineclient_name );

		return iface_pointer;
	}
}
