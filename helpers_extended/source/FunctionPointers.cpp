#include "FunctionPointers.hpp"
#include "Symbols.hpp"

#include <GarrysMod/FactoryLoader.hpp>

#include <scanning/symbolfinder.hpp>

struct netsocket_t
{
	int32_t nPort;
	bool bListening;
	int32_t hUDP;
	int32_t hTCP;
};

namespace FunctionPointers
{
	static SourceSDK::FactoryLoader client_loader( "client" );
	static SourceSDK::FactoryLoader server_loader( "server" );
	static SourceSDK::FactoryLoader engine_loader( "engine" );
	static SourceSDK::FactoryLoader dedicated_loader( "dedicated" );
	static SourceSDK::FactoryLoader filesystem_loader( "filesystem_stdio" );
	static SourceSDK::FactoryLoader lua_shared_loader( "lua_shared" );

	static SymbolFinder symbol_finder;

	template<class T>
	static inline T ResolveSymbol(
		SourceSDK::FactoryLoader &loader, const Symbol &symbol,
		const void *starting_point = nullptr
	)
	{
		if( symbol.type == Symbol::Type::None )
			return nullptr;

		return reinterpret_cast<T>( symbol_finder.Resolve(
			loader.GetModule( ), symbol.name.c_str( ), symbol.length, starting_point
		) );
	}

	template<class T>
	static inline T ResolveSymbols(
		SourceSDK::FactoryLoader &loader, const std::vector<Symbol> &symbols,
		const void *starting_point = nullptr
	)
	{
		T func_pointer = nullptr;
		for( const auto &symbol : symbols )
		{
			func_pointer = ResolveSymbol<T>( loader, symbol, starting_point );
			if( func_pointer != nullptr )
				break;
		}

		return func_pointer;
	}

	CBasePlayer_HandleClientLuaError_t CBasePlayer_HandleClientLuaError( )
	{
		static CBasePlayer_HandleClientLuaError_t func_pointer = nullptr;
		if( func_pointer == nullptr )
			func_pointer = ResolveSymbols<CBasePlayer_HandleClientLuaError_t>(
				server_loader, Symbols::CBasePlayer_HandleClientLuaError
			);

		return func_pointer;
	}

	CreateInterfaceFn FileSystemFactory( )
	{
		static CreateInterfaceFn func_pointer = nullptr;
		if( func_pointer == nullptr )
			func_pointer = ResolveSymbols<CreateInterfaceFn>(
				dedicated_loader, Symbols::FileSystemFactory
			);

		return func_pointer;
	}

	CNetChan_ProcessMessages_t CNetChan_ProcessMessages( )
	{
		static CNetChan_ProcessMessages_t func_pointer = nullptr;
		if( func_pointer == nullptr )
			func_pointer = ResolveSymbols<CNetChan_ProcessMessages_t>(
				engine_loader, Symbols::CNetChan_ProcessMessages
			);

		return func_pointer;
	}

	CBaseClient_ConnectionStart_t CBaseClient_ConnectionStart( )
	{
		static CBaseClient_ConnectionStart_t func_pointer = nullptr;
		if( func_pointer == nullptr )
			func_pointer = ResolveSymbols<CBaseClient_ConnectionStart_t>(
				engine_loader, Symbols::CBaseClient_ConnectionStart
			);

		return func_pointer;
	}

	CBaseClientState_ConnectionStart_t CBaseClientState_ConnectionStart( )
	{
		static CBaseClientState_ConnectionStart_t func_pointer = nullptr;
		if( func_pointer == nullptr )
			// we use a starting point for sigscan because, on Linux, CBaseClient::ConnectionStart
			// and CBaseClientState::ConnectionStart have the same signature
			// this code expects CBaseClient::ConnectionStart to appear before
			// CBaseClientState::ConnectionStart
			func_pointer = ResolveSymbols<CBaseClientState_ConnectionStart_t>(
				engine_loader, Symbols::CBaseClientState_ConnectionStart,
				reinterpret_cast<const uint8_t *>( CBaseClient_ConnectionStart( ) ) + 16
			);

		return func_pointer;
	}

	CBaseServer_RecalculateTags_t CBaseServer_RecalculateTags( )
	{
		static CBaseServer_RecalculateTags_t func_pointer = nullptr;
		if( func_pointer == nullptr )
			func_pointer = ResolveSymbols<CBaseServer_RecalculateTags_t>(
				engine_loader, Symbols::CBaseServer_RecalculateTags
			);

		return func_pointer;
	}

	GModDataPack_SendFileToClient_t GModDataPack_SendFileToClient( )
	{
		static GModDataPack_SendFileToClient_t func_pointer = nullptr;
		if( func_pointer == nullptr )
			func_pointer = ResolveSymbols<GModDataPack_SendFileToClient_t>(
				server_loader, Symbols::GModDataPack_SendFileToClient
			);

		return func_pointer;
	}

	CNetChan_IsValidFileForTransfer_t CNetChan_IsValidFileForTransfer( )
	{
		static CNetChan_IsValidFileForTransfer_t func_pointer = nullptr;
		if( func_pointer == nullptr )
			func_pointer = ResolveSymbols<CNetChan_IsValidFileForTransfer_t>(
				engine_loader, Symbols::CNetChan_IsValidFileForTransfer
			);

		return func_pointer;
	}

	struct netsockets_t // basic representation of CUtlVector<netsocket_t>
	{
		netsocket_t *buffer;
		int buffer_count;
		int buffer_size;
		int elements_count;
		netsocket_t *elements;
	};
	static netsockets_t *net_sockets = nullptr;
	static netsocket_t *GetNetSocket( int idx )
	{
		return idx >= 0 && idx < net_sockets->elements_count ? &net_sockets->elements[idx] : nullptr;
	}

	GMOD_GetNetSocket_t GMOD_GetNetSocket( )
	{
		static GMOD_GetNetSocket_t func_pointer = nullptr;
		if( func_pointer != nullptr )
			return func_pointer;

		func_pointer = ResolveSymbol<GMOD_GetNetSocket_t>( engine_loader, Symbols::GMOD_GetNetSocket );
		if( func_pointer != nullptr )
			return func_pointer;

		void *net_sockets_ptr = ResolveSymbol<void *>( engine_loader, Symbols::net_sockets );
		if( net_sockets_ptr != nullptr )
		{
			net_sockets =

#if defined SYSTEM_POSIX

				reinterpret_cast<netsockets_t *>

#else

				*reinterpret_cast<netsockets_t **>

#endif

				( net_sockets_ptr );
			if( net_sockets != nullptr )
				func_pointer = GetNetSocket;
		}

		return func_pointer;
	}

	GModDataPack_AddOrUpdateFile_t GModDataPack_AddOrUpdateFile( )
	{
		static GModDataPack_AddOrUpdateFile_t func_pointer = nullptr;
		if( func_pointer == nullptr )
			func_pointer = ResolveSymbols<GModDataPack_AddOrUpdateFile_t>(
				server_loader, Symbols::GModDataPack_AddOrUpdateFile
			);

		return func_pointer;
	}
}
