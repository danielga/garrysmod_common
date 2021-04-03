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
	static SourceSDK::FactoryLoader vstdlib_loader( "vstdlib" );

	static SymbolFinder symbol_finder;

	static const char filesystem_name[] = "VFileSystem022";
	static const char vengineserver_name[] = "VEngineServer021";
	static const char vengineclient_name[] = "VEngineClient015";
	static const char cvar_name[] = "VEngineCvar004";
	static const char servergamedll_name[] = "ServerGameDLL009";
	static const char networkstringtableserver_name[] = "VEngineServerStringTable001";
	static const char networkstringtableclient_name[] = "VEngineClientStringTable001";

	template<class T>
	static inline T *ResolveSymbol(
		SourceSDK::FactoryLoader &loader, const Symbol &symbol
	)
	{
		if( symbol.type == Symbol::Type::None )
			return nullptr;

#if defined SYSTEM_WINDOWS

		auto iface = reinterpret_cast<T **>( symbol_finder.Resolve(
			loader.GetModule( ), symbol.name.c_str( ), symbol.length
		) );
		return iface != nullptr ? *iface : nullptr;

#elif defined SYSTEM_POSIX

		return reinterpret_cast<T *>( symbol_finder.Resolve(
			loader.GetModule( ), symbol.name.c_str( ), symbol.length
		) );

#endif

	}

	template<class T>
	static inline T *ResolveSymbols(
		SourceSDK::FactoryLoader &loader, const std::vector<Symbol> &symbols
	)
	{
		T *iface_pointer = nullptr;
		for( const auto &symbol : symbols )
		{
			iface_pointer = ResolveSymbol<T>( loader, symbol );
			if( iface_pointer != nullptr )
				break;
		}

		return iface_pointer;
	}

	namespace Internal
	{
		namespace Client
		{
			IFileSystem *FileSystem( )
			{
				static IFileSystem *iface_pointer = nullptr;
				if( iface_pointer == nullptr )
					iface_pointer = ResolveSymbol<IFileSystem>(
						client_loader, Symbols::g_pFullFileSystem
					);

				if( iface_pointer == nullptr )
					iface_pointer = filesystem_loader.GetInterface<IFileSystem>( filesystem_name );

				return iface_pointer;
			}

			INetworkStringTableContainer *NetworkStringTableContainer( )
			{
				static INetworkStringTableContainer *iface_pointer = nullptr;
				if( iface_pointer == nullptr )
					iface_pointer = engine_loader.GetInterface<INetworkStringTableContainer>(
						networkstringtableclient_name
					);

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
					iface_pointer = ResolveSymbol<IFileSystem>(
						dedicated_loader, Symbols::g_pFullFileSystem
					);
					if( iface_pointer == nullptr )
						iface_pointer = ResolveSymbol<IFileSystem>(
							server_loader, Symbols::g_pFullFileSystem
						);
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

			INetworkStringTableContainer *NetworkStringTableContainer( )
			{
				static INetworkStringTableContainer *iface_pointer = nullptr;
				if( iface_pointer == nullptr )
					iface_pointer = engine_loader.GetInterface<INetworkStringTableContainer>(
						networkstringtableserver_name
					);

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

	ICvar *Cvar( )
	{
		static ICvar *iface_pointer = nullptr;
		if( iface_pointer == nullptr )
			iface_pointer = vstdlib_loader.GetInterface<ICvar>( cvar_name );

		return iface_pointer;
	}

	IServerGameDLL *ServerGameDLL( )
	{
		static IServerGameDLL *iface_pointer = nullptr;
		if( iface_pointer == nullptr )
			iface_pointer = server_loader.GetInterface<IServerGameDLL>( servergamedll_name );

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
		{
			auto Steam3Server = FunctionPointers::Steam3Server( );
			if( Steam3Server != nullptr )
				iface_pointer = reinterpret_cast<CSteamGameServerAPIContext *>( Steam3Server( ) );
		}

		return iface_pointer;
	}
}
