#include "Symbols.hpp"
#include "Symbol.hpp"
#include "Platform.hpp"

namespace Symbols
{

	const std::vector<Symbol> CBasePlayer_HandleClientLuaError = {
		Symbol::FromName( "?HandleClientLuaError@@YAXPEAVCBasePlayer@@PEBD@Z" )
	};

	const std::vector<Symbol> FileSystemFactory = {
		Symbol::FromName( "?FileSystemFactory@@YAPEAXPEBDPEAH@Z" )
	};

	const Symbol g_pFullFileSystem = Symbol::FromName( "?g_pFullFileSystem@@3PEAVIFileSystem@@EA" );

	const std::vector<Symbol> IServer = {
		Symbol::FromName( "?sv@@3VCGameServer@@A" )
	};

	const std::vector<Symbol> CNetChan_ProcessMessages = {
		Symbol::FromName( "?ProcessMessages@CNetChan@@AEAA_NAEAVbf_read@@@Z" )
	};

	const std::vector<Symbol> CBaseClient_ConnectionStart = {
		Symbol::FromName( "?ConnectionStart@CBaseClient@@UEAAXPEAVINetChannel@@@Z" )
	};

	const std::vector<Symbol> CBaseClientState_ConnectionStart = {
		Symbol::FromName( "?ConnectionStart@CBaseClientState@@UEAAXPEAVINetChannel@@@Z" )
	};

	const std::vector<Symbol> CLC_CmdKeyValues_Constructor = {
		Symbol::FromSignature( "\x48\x8D\x05\x2A\x2A\x2A\x2A\xC6\x41\x2A\x01\x48\x89\x01\x48\x8B\xC1" )
	};

	const std::vector<Symbol> SVC_CreateStringTable_Constructor = {
		Symbol::FromSignature( "\x40\x53\x48\x83\xEC\x20\x48\x8B\xD9\xC6\x41\x2A\x01\x33\xC9" )
	};

	const std::vector<Symbol> SVC_CmdKeyValues_Constructor = {
		Symbol::FromSignature( "\x48\x8D\x05\x2A\x2A\x2A\x2A\xC6\x41\x2A\x01\x48\x89\x01\x48\x8B\xC1" )
	};

	const std::vector<Symbol> CBaseServer_RecalculateTags = {
		Symbol::FromName( "?RecalculateTags@CBaseServer@@QEAAXXZ" )
	};

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4996 )
#endif

	const std::vector<Symbol> SteamGameServerAPIContext = {
		Symbol::FromName( "?s_SteamGameServerAPIContext@@3VCSteamGameServerAPIContext@@A" )
	};

#ifdef _MSC_VER
#pragma warning( pop )
#endif

	const std::vector<Symbol> GModDataPack_SendFileToClient = {
		Symbol::FromName( "?SendFileToClient@GModDataPack@@QEAAXHH@Z" )
	};

	const std::vector<Symbol> CNetChan_IsValidFileForTransfer = {
		Symbol::FromName( "?IsValidFileForTransfer@CNetChan@@SA_NPEBD@Z" )
	};

	const std::vector<Symbol> net_sockets;

	const Symbol GMOD_GetNetSocket = Symbol::FromName( "?GMOD_GetNetSocket@@YAPEAUnetsocket_t@@H@Z" );

	const std::vector<Symbol> GModDataPack_AddOrUpdateFile = {
		Symbol::FromName( "?AddOrUpdateFile@GModDataPack@@QEAAXPEAULuaFile@@_N@Z" )
	};

	const Symbol Steam3Server =
		Symbol::FromSignature( "\x48\x8D\x05\x2A\x2A\x2A\x2A\xC3\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\x40\x53\x48\x83\xEC\x20\x48\x8B\xD9\xFF\x15" );

	const std::vector<Symbol> GlobalVars = {
		Symbol::FromSignature( "\x2A\x2A\x2A\x2A\x41\x8B\xD4\x48\x8D\x4C\x24\x2A\xE8\x2A\x2A\x2A\x2A\x41\x8B\xD4\x48" )
	};

	const std::vector<Symbol> AdvancedLuaErrorReporter = { Symbol::FromName( "?AdvancedLuaErrorReporter@@YAHPEAUlua_State@@@Z" ) };

	const std::vector<Symbol> NET_ProcessSocket;

	const std::vector<Symbol> NET_CreateNetChannel = {
		Symbol::FromSignature( "\x48\x89\x6C\x24\x2A\x48\x89\x7C\x24\x2A\x41\x54\x41\x56\x41\x57\x48\x83\xEC\x30\x80\x7C\x24\x2A\x00" )
	};

    const std::vector<Symbol> HandleChange = {
        Symbol::FromSignature( "\x48\x89\x5c\x24\x10\x48\x89\x74\x24\x18\x48\x89\x7c\x24\x20\x55\x48\x8d\x6c\x24\xa9\x48\x81\xec\xb0\x00\x00\x00\x48\x8b\x05\x2A\x2A\x2A\x2A\x48\x33\xc4\x48\x89\x45\x47\x48\x8b\xf9\x48\x8b\xd1\x48\x8d\x4d\xc7\xe8\x2A\x2A\x2A\x2A\x48\x83\x7d\xd7\x00\x0f\x84" )
    };

}
