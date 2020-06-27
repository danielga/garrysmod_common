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

	CBasePlayer_HandleClientLuaError_t CBasePlayer_HandleClientLuaError( )
	{
		static CBasePlayer_HandleClientLuaError_t func_pointer = nullptr;
		if( func_pointer != nullptr )
			return func_pointer;

		for( const auto &symbol : Symbols::CBasePlayer_HandleClientLuaError )
		{
			func_pointer = reinterpret_cast<CBasePlayer_HandleClientLuaError_t>( symbol_finder.Resolve(
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

	CNetChan_ProcessMessages_t CNetChan_ProcessMessages( )
	{
		static CNetChan_ProcessMessages_t func_pointer = nullptr;
		if( func_pointer != nullptr )
			return func_pointer;

		for( const auto &symbol : Symbols::CNetChan_ProcessMessages )
		{
			func_pointer = reinterpret_cast<CNetChan_ProcessMessages_t>( symbol_finder.Resolve(
				engine_loader.GetModule( ),
				symbol.name.c_str( ),
				symbol.length
			) );
			if( func_pointer != nullptr )
				break;
		}

		return func_pointer;
	}

	CBaseClient_ConnectionStart_t CBaseClient_ConnectionStart( )
	{
		static CBaseClient_ConnectionStart_t func_pointer = nullptr;
		if( func_pointer != nullptr )
			return func_pointer;

		for( const auto &symbol : Symbols::CBaseClient_ConnectionStart )
		{
			func_pointer = reinterpret_cast<CBaseClient_ConnectionStart_t>( symbol_finder.Resolve(
				engine_loader.GetModule( ),
				symbol.name.c_str( ),
				symbol.length
			) );
			if( func_pointer != nullptr )
				break;
		}

		return func_pointer;
	}

	CBaseClientState_ConnectionStart_t CBaseClientState_ConnectionStart( )
	{
		static CBaseClientState_ConnectionStart_t func_pointer = nullptr;
		if( func_pointer != nullptr )
			return func_pointer;

		const auto starting_point = reinterpret_cast<const uint8_t *>( CBaseClient_ConnectionStart( ) );
		for( const auto &symbol : Symbols::CBaseClientState_ConnectionStart )
		{
			func_pointer = reinterpret_cast<CBaseClientState_ConnectionStart_t>( symbol_finder.Resolve(
				engine_loader.GetModule( ),
				symbol.name.c_str( ),
				symbol.length,
				// starting point for sigscan
				// we use an offset because, on Linux, CBaseClient::ConnectionStart and
				// CBaseClientState::ConnectionStart have the same signature
				// this code expects CBaseClient::ConnectionStart to appear before
				// CBaseClientState::ConnectionStart
				starting_point + 16
			) );
			if( func_pointer != nullptr )
				break;
		}

		return func_pointer;
	}

	CBaseServer_RecalculateTags_t CBaseServer_RecalculateTags( )
	{
		static CBaseServer_RecalculateTags_t func_pointer = nullptr;
		if( func_pointer != nullptr )
			return func_pointer;

		for( const auto &symbol : Symbols::CBaseServer_RecalculateTags )
		{
			func_pointer = reinterpret_cast<CBaseServer_RecalculateTags_t>( symbol_finder.Resolve(
				engine_loader.GetModule( ),
				symbol.name.c_str( ),
				symbol.length
			) );
			if( func_pointer != nullptr )
				break;
		}

		return func_pointer;
	}

	GModDataPack_SendFileToClient_t GModDataPack_SendFileToClient( )
	{
		static GModDataPack_SendFileToClient_t func_pointer = nullptr;
		if( func_pointer != nullptr )
			return func_pointer;

		for( const auto &symbol : Symbols::GModDataPack_SendFileToClient )
		{
			func_pointer = reinterpret_cast<GModDataPack_SendFileToClient_t>( symbol_finder.Resolve(
				server_loader.GetModule( ),
				symbol.name.c_str( ),
				symbol.length
			) );
			if( func_pointer != nullptr )
				break;
		}

		return func_pointer;
	}

	CNetChan_IsValidFileForTransfer_t CNetChan_IsValidFileForTransfer( )
	{
		static CNetChan_IsValidFileForTransfer_t func_pointer = nullptr;
		if( func_pointer != nullptr )
			return func_pointer;

		for( const auto &symbol : Symbols::CNetChan_IsValidFileForTransfer )
		{
			func_pointer = reinterpret_cast<CNetChan_IsValidFileForTransfer_t>( symbol_finder.Resolve(
				engine_loader.GetModule( ),
				symbol.name.c_str( ),
				symbol.length
			) );
			if( func_pointer != nullptr )
				break;
		}

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

		func_pointer = reinterpret_cast<GMOD_GetNetSocket_t>( symbol_finder.Resolve(
			engine_loader.GetModule( ),
			Symbols::GMOD_GetNetSocket.name.c_str( ),
			Symbols::GMOD_GetNetSocket.length
		) );
		if( func_pointer != nullptr )
			return func_pointer;

		void *net_sockets_ptr = symbol_finder.Resolve(
			engine_loader.GetModule( ),
			Symbols::net_sockets.name.c_str( ),
			Symbols::net_sockets.length
		);
		if( net_sockets_ptr != nullptr )
		{
			net_sockets =

#if defined SYSTEM_POSIX

				reinterpret_cast<netsockets_t *>

#else

				*reinterpret_cast<netsockets_t **>

#endif

				( net_sockets_ptr );
		}

		if( net_sockets != nullptr )
			func_pointer = GetNetSocket;

		return func_pointer;
	}
}
