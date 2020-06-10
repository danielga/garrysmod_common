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
	static SourceSDK::FactoryLoader filesystem_loader( "filesystem_stdio" );
	static SourceSDK::FactoryLoader lua_shared_loader( "lua_shared" );

	static SymbolFinder symbol_finder;

	static const char filesystem_name[] = "VFileSystem022";
	static const char vengineserver_name[] = "VEngineServer021";
	static const char vengineclient_name[] = "VEngineClient015";

	namespace Internal
	{
		namespace Client
		{
			IFileSystem *FileSystem( )
			{
				static IFileSystem *iface_pointer = nullptr;
				if( iface_pointer == nullptr )
					iface_pointer = reinterpret_cast<IFileSystem *>(
						client_loader.GetSymbol( Symbols::g_pFullFileSystem.name )
					);

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

				auto factory = FunctionPointers::FileSystemFactory( );
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

	template<class T>
	static inline T *ResolveSymbols(
		SourceSDK::FactoryLoader &loader, const std::vector<Symbol> &symbols
	)
	{
		T *iface_pointer = nullptr;
		for( const auto &symbol : symbols )
		{
			auto iface = reinterpret_cast<T **>( symbol_finder.Resolve(
				loader.GetModule( ), symbol.name.c_str( ), symbol.length
			) );
			if( iface != nullptr )
			{
				iface_pointer = *iface;
				break;
			}
		}

		return iface_pointer;
	}

	IServer *Server( )
	{
		static IServer *iface_pointer = nullptr;
		if( iface_pointer == nullptr )
			iface_pointer = ResolveSymbols<IServer>( engine_loader, Symbols::IServer );

		return iface_pointer;
	}

	CSteamGameServerAPIContext *SteamGameServerAPIContext( )
	{
		static CSteamGameServerAPIContext *iface_pointer = nullptr;
		if( iface_pointer == nullptr )
			iface_pointer = ResolveSymbols<CSteamGameServerAPIContext>(
				engine_loader, Symbols::SteamGameServerAPIContext
			);

		return iface_pointer;
	}
}
