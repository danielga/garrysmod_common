#include "Symbols.hpp"
#include "Symbol.hpp"
#include "Platform.hpp"

namespace Symbols
{

	const std::vector<Symbol> CBasePlayer_HandleClientLuaError = {
		Symbol::FromSignature( "\x55\x89\xE5\x57\x56\x53\x83\xEC\x4C\x65\xA1\x2A\x2A\x2A\x2A\x89\x45\xE4" )
	};

	const std::vector<Symbol> FileSystemFactory = { Symbol::FromName( "_Z17FileSystemFactoryPKcPi" ) };

	const Symbol g_pFullFileSystem = Symbol::FromName( "g_pFullFileSystem" );

	const std::vector<Symbol> IServer = {
		Symbol::FromName( "sv" ),

#if defined ARCHITECTURE_X86

		Symbol::FromSignature( "\x2A\x2A\x2A\x2A\x8B\x08\x89\x04\x24\xFF\x51\x28\xF3\x0F\x10\x45" )

#endif

	};

	const std::vector<Symbol> CNetChan_ProcessMessages = {
		Symbol::FromName( "_ZN8CNetChan15ProcessMessagesER7bf_read" )
	};

	const std::vector<Symbol> CBaseClient_ConnectionStart = {
		Symbol::FromName( "_ZN11CBaseClient15ConnectionStartEP11INetChannel" )
	};

	const std::vector<Symbol> CBaseClientState_ConnectionStart = {
		Symbol::FromName( "_ZN16CBaseClientState15ConnectionStartEP11INetChannel" )
	};

	const std::vector<Symbol> CLC_CmdKeyValues_Constructor = {
		Symbol::FromName( "_ZN16CLC_CmdKeyValuesC1EP9KeyValues" )
	};

	const std::vector<Symbol> SVC_CreateStringTable_Constructor = {
		Symbol::FromName( "_ZN21SVC_CreateStringTableC1Ev" )
	};

	const std::vector<Symbol> SVC_CmdKeyValues_Constructor = {
		Symbol::FromName( "_ZN16SVC_CmdKeyValuesC1EP9KeyValues" )
	};

	const std::vector<Symbol> CBaseServer_RecalculateTags = {
		Symbol::FromName( "_ZN11CBaseServer15RecalculateTagsEv" )
	};

	const std::vector<Symbol> SteamGameServerAPIContext = {
		Symbol::FromName( "_ZL27s_SteamGameServerAPIContext" )
	};

	const std::vector<Symbol> GModDataPack_SendFileToClient = {
		Symbol::FromName( "_ZN12GModDataPack16SendFileToClientEii" )
	};

	const std::vector<Symbol> CNetChan_IsValidFileForTransfer = {
		Symbol::FromName( "_ZN8CNetChan22IsValidFileForTransferEPKc" )
	};

	const std::vector<Symbol> net_sockets = { Symbol::FromName( "_ZL11net_sockets" ) };

	const Symbol GMOD_GetNetSocket = Symbol::FromName( "_Z17GMOD_GetNetSocketi" );

	const std::vector<Symbol> GModDataPack_AddOrUpdateFile = {
		Symbol::FromName( "_ZN12GModDataPack15AddOrUpdateFileEP7LuaFileb" )
	};

#if defined ARCHITECTURE_X86

	const Symbol Steam3Server = Symbol::FromName( "_Z12Steam3Serverv" );

#elif ARCHITECTURE_X86_64

	const Symbol Steam3Server =
		Symbol::FromSignature( "\x55\x48\x89\xE5\x48\x8D\x05\x2A\x2A\x2A\x2A\x5D\xC3\x0F\x1F\x00" );

#endif

	const std::vector<Symbol> GlobalVars = {

#if defined ARCHITECTURE_X86

		Symbol::FromSignature( "\x2A\x2A\x2A\x2A\x8D\xBD\x2A\x2A\x2A\x2A\x89\x3C\x24\xC7\x44\x24\x2A\x01\x00\x00\x00\xE8" )

#elif defined ARCHITECTURE_X86_64

		Symbol::FromSignature( "\x2A\x2A\x2A\x2A\x48\x8D\xBD\x2A\x2A\x2A\x2A\xBE\x01\x00\x00\x00\xE8\x2A\x2A\x2A\x2A\x48\x8D\xBD" )

#endif

	};

	const std::vector<Symbol> AdvancedLuaErrorReporter = { Symbol::FromName( "_Z24AdvancedLuaErrorReporterP9lua_State" ) };

	const std::vector<Symbol> NET_ProcessSocket = { Symbol::FromName( "_Z17NET_ProcessSocketiP28IConnectionlessPacketHandler" ) };

	const std::vector<Symbol> NET_CreateNetChannel;

    const std::vector<Symbol> HandleChange = { Symbol::FromSignature( "\x55\x48\x89\xe5\x53\x48\x81\xec\x88\x00\x00\x00\x48\x89\xfb\x48\x8d\x7d\xc8\x48\x89\xde\xe8\x2A\x2A\x2A\x2A\x8a\x4d\xc8\x89\xc8\x24\x01\x74" ) };

}
