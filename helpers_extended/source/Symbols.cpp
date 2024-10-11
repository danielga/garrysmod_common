#include "Symbols.hpp"
#include "Symbol.hpp"
#include "Platform.hpp"

namespace Symbols
{

#if defined SYSTEM_WINDOWS

#if defined ARCHITECTURE_X86_64

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
        Symbol::FromSignature( "\x48\x89\x5c\x24\x10\x48\x89\x74\x24\x18\x48\x89\x7c\x24\x20\x55\x48\x8d\x6c\x24\xa9\x48\x81\xec\xb0\x00\x00\x00\x48\x8b\x05\x2A" )
    };

#elif defined ARCHITECTURE_X86

	const std::vector<Symbol> CBasePlayer_HandleClientLuaError = {
		Symbol::FromName( "?HandleClientLuaError@@YAXPAVCBasePlayer@@PBD@Z" ),
		Symbol::FromSignature( "\x55\x8B\xEC\x83\xEC\x08\x8B\x0D\x2A\x2A\x2A\x2A\x57\x8B\x7D\x08" )
	};

	const std::vector<Symbol> FileSystemFactory = {
		Symbol::FromName( "?FileSystemFactory@@YAPAXPBDPAH@Z" ),
		Symbol::FromSignature( "\x55\x8B\xEC\x68\x2A\x2A\x2A\x2A\xFF\x75\x08\xE8" )
	};

	const Symbol g_pFullFileSystem = Symbol::FromName( "?g_pFullFileSystem@@3PAVIFileSystem@@A" );

	const std::vector<Symbol> IServer = {
		Symbol::FromName( "?sv@@3VCGameServer@@A" ),
		Symbol::FromSignature( "\x2A\x2A\x2A\x2A\xE8\x2A\x2A\x2A\x2A\xD8\x6D\x24\x83\x4D\xEC\x10" )
	};

	const std::vector<Symbol> CNetChan_ProcessMessages = {
		Symbol::FromName( "?ProcessMessages@CNetChan@@AAE_NAAVbf_read@@@Z" ),
		Symbol::FromSignature( "\x55\x8B\xEC\x83\xEC\x2C\xF7\x05" )
	};

	const std::vector<Symbol> CBaseClient_ConnectionStart = {
		Symbol::FromName( "?ConnectionStart@CBaseClient@@UAEXPAVINetChannel@@@Z" ),
		Symbol::FromSignature( "\x55\x8B\xEC\x53\x56\x57\x6A\x1C\x8B\xF1\x2A\x2A\x2A\x2A\x2A\x8B\xD0\x83\xC4\x04\x85\xD2\x2A\x2A\xC7" )
	};

	const std::vector<Symbol> CBaseClientState_ConnectionStart = {
		Symbol::FromName( "?ConnectionStart@CBaseClientState@@UAEXPAVINetChannel@@@Z" ),
		Symbol::FromSignature( "\x55\x8B\xEC\x53\x56\x57\x6A\x1C\x8B\xF9\x2A\x2A\x2A\x2A\x2A\x8B\xC8\x83\xC4\x04\x85\xC9\x2A\x2A\xC7\x41\x08\x00" )
	};

	const std::vector<Symbol> CLC_CmdKeyValues_Constructor = {
		Symbol::FromSignature( "\x55\x8B\xEC\x8B\x45\x2A\x89\x41\x2A\x8B\xC1\xC6\x41\x2A\x01\xC7\x41\x2A\x00\x00\x00\x00\xC7\x01\x2A\x2A\x2A\x2A\x5D\xC2\x04\x00" )
	};

	const std::vector<Symbol> SVC_CreateStringTable_Constructor = {
		Symbol::FromSignature( "\x56\x8B\xF1\x8D\x4E\x2A\xC6\x46\x2A\x01" )
	};

	const std::vector<Symbol> SVC_CmdKeyValues_Constructor = {
		Symbol::FromSignature( "\x55\x8B\xEC\x8B\x45\x2A\x89\x41\x2A\x8B\xC1\xC6\x41\x2A\x01\xC7\x41\x2A\x00\x00\x00\x00\xC7\x01\x2A\x2A\x2A\x2A\x5D\xC2\x04\x00" )
	};

	const std::vector<Symbol> CBaseServer_RecalculateTags = {
		Symbol::FromName( "?RecalculateTags@CBaseServer@@QAEXXZ" ),
		Symbol::FromSignature( "\x55\x8B\xEC\x83\xEC\x78\x56\x57" )
	};

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4996 )
#endif

	const std::vector<Symbol> SteamGameServerAPIContext = {
		Symbol::FromName( "?s_SteamGameServerAPIContext@@3VCSteamGameServerAPIContext@@A" ),
		Symbol::FromSignature( "\x2A\x2A\x2A\x2A\xE8\x2A\x2A\x2A\x2A\x6A\x00\x68\x2A\x2A\x2A\x2A\xFF\x55\x08\x83\xC4\x08\xA" )
	};

#ifdef _MSC_VER
#pragma warning( pop )
#endif

	const std::vector<Symbol> GModDataPack_SendFileToClient = {
		Symbol::FromName( "?SendFileToClient@GModDataPack@@QAEXHH@Z" ),
		Symbol::FromSignature( "\x55\x8B\xEC\x83\xEC\x50\x53\x8B\x5D\x0C\x56\x8B\xF1\x89\x75\xF8" )
	};

	const std::vector<Symbol> CNetChan_IsValidFileForTransfer = {
		Symbol::FromName( "?IsValidFileForTransfer@CNetChan@@SA_NPBD@Z" ),
		Symbol::FromSignature( "\x55\x8B\xEC\x53\x8B\x5D\x08\x56\x57\x85\xDB\x0F\x84" )
	};

	const std::vector<Symbol> net_sockets = {
		Symbol::FromSignature( "\x2A\x2A\x2A\x2A\x56\x57\x8B\x7D\x08\x8B\xF7\x03\xF6\x8B\x44\xF3\x0C\x85\xC0\x74\x0A\x57\x50" )
	};

	const Symbol GMOD_GetNetSocket = Symbol::FromName( "?GMOD_GetNetSocket@@YAPAUnetsocket_t@@H@Z" );

	const std::vector<Symbol> GModDataPack_AddOrUpdateFile = {
		Symbol::FromName( "?AddOrUpdateFile@GModDataPack@@QAEXPAULuaFile@@_N@Z" ),
		Symbol::FromSignature( "\x55\x8B\xEC\x81\xEC\x80\x00\x00\x00\x53\x8B\xD9\x56\x8B\x75\x08\x57\x8B\x03\x89\x5D\xFC\x8D\x7E\x04" )
	};

	const Symbol Steam3Server =
		Symbol::FromSignature( "\xB8\x2A\x2A\x2A\x2A\xC3\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\x55\x8B\xEC\x68\x2A\x2A\x2A\x2A\xFF\x15\x2A\x2A\x2A\x2A\x50\xFF\x15" );

	const std::vector<Symbol> GlobalVars = {
		Symbol::FromSignature( "\x2A\x2A\x2A\x2A\x8D\x45\x2A\x6A\x01\x50\xE8\x2A\x2A\x2A\x2A\x8D\x45" )
	};

	const std::vector<Symbol> AdvancedLuaErrorReporter = {
		Symbol::FromName( "?AdvancedLuaErrorReporter@@YAHPAUlua_State@@@Z" ),
		Symbol::FromSignature( "\x55\x8B\xEC\x83\xEC\x5C" )
	};

	const std::vector<Symbol> NET_ProcessSocket = { Symbol::FromSignature( "\x55\x8B\xEC\x83\xEC\x08\x8B\x0D\x2A\x2A\x2A\x2A\x8B\x81" ) };

	const std::vector<Symbol> NET_CreateNetChannel = {
		Symbol::FromSignature( "\x55\x8B\xEC\x80\x7D\x2A\x00\x53\x8B\x5D" )
	};

    const std::vector<Symbol> HandleChange = {
        Symbol::FromSignature( "\x55\x8b\xec\x83\xec\x60\x56\x8b\x75\x08\x8d\x45\xd0\x57\x56\x50\xe8\x2a\x2a\x2a\x00\x83\xc4\x08\x83\x7d\xe0\x00\x0f\x84\x60\x02" )
    };

#endif

#elif defined SYSTEM_LINUX

	const std::vector<Symbol> CBasePlayer_HandleClientLuaError = {
		Symbol::FromName( "_Z20HandleClientLuaErrorP11CBasePlayerPKc" ),

#if defined ARCHITECTURE_X86

		Symbol::FromSignature( "\x55\x89\xE5\x57\x56\x53\x83\xEC\x4C\x65\xA1\x2A\x2A\x2A\x2A\x89" )

#endif

	};

	const std::vector<Symbol> FileSystemFactory = {
		Symbol::FromName( "_Z17FileSystemFactoryPKcPi" ),

#if defined ARCHITECTURE_X86

		Symbol::FromSignature( "\x55\x89\xE5\x83\xEC\x18\x89\x75\xFC\x8B\x75\x08\x89\x5D\xF8\x8B" )

#endif

	};

	const Symbol g_pFullFileSystem = Symbol::FromName( "g_pFullFileSystem" );

	const std::vector<Symbol> IServer = {
		Symbol::FromName( "sv" ),

#if defined ARCHITECTURE_X86

		Symbol::FromSignature( "\x2A\x2A\x2A\x2A\x89\x5D\xA8\x89\x44\x24\x08\xE8" )

#endif

	};

	const std::vector<Symbol> CNetChan_ProcessMessages = {
		Symbol::FromName( "_ZN8CNetChan15ProcessMessagesER7bf_read" ),

#if defined ARCHITECTURE_X86

		Symbol::FromSignature( "\x55\x89\xE5\x57\x56\x53\x83\xEC\x6C\x8B\x3D\x2A\x2A\x2A\x2A\x8B" )

#endif

	};

	const std::vector<Symbol> CBaseClient_ConnectionStart = {
		Symbol::FromName( "_ZN11CBaseClient15ConnectionStartEP11INetChannel" ),

#if defined ARCHITECTURE_X86

		Symbol::FromSignature( "\x55\x89\xE5\x57\x56\x53\x83\xEC\x1C\x8B" )

#endif

	};

	const std::vector<Symbol> CBaseClientState_ConnectionStart = {
		Symbol::FromName( "_ZN16CBaseClientState15ConnectionStartEP11INetChannel" ),

#if defined ARCHITECTURE_X86

		Symbol::FromSignature( "\x55\x89\xE5\x57\x56\x53\x83\xEC\x1C\x8B" )

#endif

	};

	const std::vector<Symbol> CLC_CmdKeyValues_Constructor = {
		Symbol::FromName( "_ZN16CLC_CmdKeyValuesC1EP9KeyValues" ),

#if defined ARCHITECTURE_X86

		Symbol::FromSignature( "\x55\x89\xE5\x8B\x45\x2A\x8B\x55\x2A\xC6\x40\x2A\x01\xC7\x40\x2A\x00\x00\x00\x00\x89\x50\x2A\xC7\x00\x2A\x2A\x2A\x2A\x5D\xC3" )

#elif defined ARCHITECTURE_X86_64

		Symbol::FromSignature( "\x5D\xC3\x55\x48\x8D\x05\x2A\x2A\x2A\x2A\xC6\x47\x2A\x01\x48\x89\xE5\x48\xC7\x47\x2A\x00\x00\x00\x00\x48\x89\x77\x2A\x48\x89\x07\x5D\xC3", 2 )

#endif

	};

	const std::vector<Symbol> SVC_CreateStringTable_Constructor = {
		Symbol::FromName( "_ZN21SVC_CreateStringTableC1Ev" ),

#if defined ARCHITECTURE_X86

		Symbol::FromSignature( "\x55\x89\xE5\x53\x83\xEC\x14\x8B\x5D\x2A\x8D\x43\x2A\xC6\x43\x2A\x01" )

#elif defined ARCHITECTURE_X86_64

		Symbol::FromSignature( "\x55\x48\x8D\x05\x2A\x2A\x2A\x2A\x48\x89\xE5\x53\x48\x89\xFB\x48\x8D\xBF\x2A\x2A\x2A\x2A\x48\x83\xEC\x08\xC6\x47\x2A\x01" )

#endif

	};

	const std::vector<Symbol> SVC_CmdKeyValues_Constructor = {
		Symbol::FromName( "_ZN16SVC_CmdKeyValuesC1EP9KeyValues" ),

#if defined ARCHITECTURE_X86

		Symbol::FromSignature( "\x55\x89\xE5\x8B\x45\x2A\x8B\x55\x2A\xC6\x40\x2A\x01\xC7\x40\x2A\x00\x00\x00\x00\x89\x50\x2A\xC7\x00\x2A\x2A\x2A\x2A\x5D\xC3" )

#elif defined ARCHITECTURE_X86_64

		Symbol::FromSignature( "\x55\x48\x8D\x05\x2A\x2A\x2A\x2A\xC6\x47\x2A\x01\x48\x89\xE5\x48\xC7\x47\x2A\x00\x00\x00\x00\x48\x89\x77\x2A\x48\x89\x07\x5D\xC3" )

#endif

	};

	const std::vector<Symbol> CBaseServer_RecalculateTags = {
		Symbol::FromName( "_ZN11CBaseServer15RecalculateTagsEv" ),

#if defined ARCHITECTURE_X86

		Symbol::FromSignature( "\x55\x89\xE5\x57\x56\x8D\x85\x2A\x2A\x2A\x2A\x53\x8D\x5D" )

#endif

	};

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

	const std::vector<Symbol> SteamGameServerAPIContext = {
		Symbol::FromName( "_ZL27s_SteamGameServerAPIContext" ),

#if defined ARCHITECTURE_X86

		Symbol::FromSignature( "\x2A\x2A\x2A\x2A\x0F\x84\x2A\x2A\x2A\x2A\xC7\x04\x24\x2A\x2A\x2A\x2A\xE8\x2A\x2A\x2A\x2A\x8B\x00\xC7\x04\x24" )

#endif

	};

#pragma GCC diagnostic pop

	const std::vector<Symbol> GModDataPack_SendFileToClient = {
		Symbol::FromName( "_ZN12GModDataPack16SendFileToClientEii" ),

#if defined ARCHITECTURE_X86

		Symbol::FromSignature( "\x55\x89\xE5\x57\x56\x53\x83\xEC\x6C\x8B\x75\x10\x85\xF6\x0F\x88" )

#endif

	};

	const std::vector<Symbol> CNetChan_IsValidFileForTransfer = {
		Symbol::FromName( "_ZN8CNetChan22IsValidFileForTransferEPKc" ),

#if defined ARCHITECTURE_X86

		Symbol::FromSignature( "\x55\x89\xC1\x89\xE5\x57\x31\xFF\x56\x89\xC6\x53\x83\xEC\x1C\xEB" )

#endif

	};

	const std::vector<Symbol> net_sockets = {
		Symbol::FromName( "_ZL11net_sockets" ),

#if defined ARCHITECTURE_X86

		Symbol::FromSignature( "\x2A\x2A\x2A\x2A\x29\xF7\x89\x7C\x24\x04\xE8\x2A\x2A\x2A\x2A\x89\x7C\x24\x08\x89\x74\x24\x04" )

#endif

	};

	const Symbol GMOD_GetNetSocket = Symbol::FromName( "_Z17GMOD_GetNetSocketi" );

	const std::vector<Symbol> GModDataPack_AddOrUpdateFile = {
		Symbol::FromName( "_ZN12GModDataPack15AddOrUpdateFileEP7LuaFileb" ),
		
#if defined ARCHITECTURE_X86

		Symbol::FromSignature( "\x55\x89\xE5\x57\x56\x53\x83\xEC\x4C\x8B\x45\x10\x8B\x5D\x08\x8B" )

#endif

	};

#if defined ARCHITECTURE_X86

	const Symbol Steam3Server =
		Symbol::FromSignature( "\x55\xB8\x2A\x2A\x2A\x2A\x89\xE5\x5D\xC3\x8D\xB6\x00\x00\x00\x00\x55\x31\xC0\xB9\x2A\x00\x00\x00\x89\xE5\x57\x56\x53\x83\xEC\x2C" );

#elif defined ARCHITECTURE_X86_64

	const Symbol Steam3Server =
		Symbol::FromSignature( "\x55\x48\x8D\x05\x2A\x2A\x2A\x2A\x48\x89\xE5\x5D\xC3\x90\x66\x90" );

#endif

	const std::vector<Symbol> GlobalVars = {

#if defined ARCHITECTURE_X86

		Symbol::FromSignature( "\x2A\x2A\x2A\x2A\x8D\x9D\x2A\x2A\x2A\x2A\xC7\x44\x24\x2A\x01\x00\x00\x00\x89\x1C\x24" )

#elif defined ARCHITECTURE_X86_64

		Symbol::FromSignature( "\x2A\x2A\x2A\x2A\x4C\x8D\xAD\x2A\x2A\x2A\x2A\xBE\x01\x00\x00\x00\x4C\x89\xEF\xE8" )

#endif

	};

	const std::vector<Symbol> AdvancedLuaErrorReporter = {

#if defined ARCHITECTURE_X86

		Symbol::FromSignature( "\x55\x89\xE5\x57\x56\x53\x83\xEC\x7C\x8B\x15\x2A\x2A\x2A\x2A\x8B\x7D\x2A" ),

#endif

		Symbol::FromName( "_Z24AdvancedLuaErrorReporterP9lua_State" )

	};

	const std::vector<Symbol> NET_ProcessSocket = { Symbol::FromName( "_Z17NET_ProcessSocketiP28IConnectionlessPacketHandler" ) };

	const std::vector<Symbol> NET_CreateNetChannel = {
		Symbol::FromName( "_Z20NET_CreateNetChanneliP8netadr_sPKcP18INetChannelHandlerbi" ),

#if defined ARCHITECTURE_X86_OLD

		Symbol::FromSignature( "\x55\x89\xE5\x57\x56\x53\x83\xEC\x3C\xC7\x45\x2A\x00\x00\x00\x00\x80\x7D\x2A\x01" )

#elif defined ARCHITECTURE_X86

		Symbol::FromSignature( "\x55\x89\xE5\x57\x56\x53\x83\xEC\x2C\x80\x7D\x2A\x01" )

#elif defined ARCHITECTURE_X86_64

		Symbol::FromSignature( "\x55\x48\x89\xE5\x41\x57\x45\x89\xCF\x41\x56\x49\x89\xCE" )

#endif

	};

    const std::vector<Symbol> HandleChange = {

#if defined ARCHITECTURE_X86

        Symbol::FromSignature( "\x55\x89\xe5\x57\x56\x53\x8d\x5d\x98\x83\xec\x7c\x8b\x75\x08\x89\x1c\x24\x89\x74\x24\x04\xe8\x44\x4d\x94\x00\x8b\x45\x98\x83\xec\x04\x8b\x50\xf4\x85\xd2\x0f\x84\x86\x00\x00\x00\x8d\x45\xb8\x89" )

#elif ARCHITECTURE_X86_64

        Symbol::FromSignature( "\x55\x48\x89\xfe\x48\x89\xe5\x41\x57\x41\x56\x41\x55\x41\x54\x4c\x8d\x65\x80\x53\x48\x89\xfb\x4c\x89\xe7\x48\x83\xec\x68\xe8\x2d\x23\x1c\x00\x48\x8b\x45\x80\x48\x83\x78\xe8\x00\x75\x2a\x48\x8b" )

#endif

    };

#elif defined SYSTEM_MACOSX

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

    const std::vector<Symbol> HandleChange = { Symbol::FromSignature( "\x55\x48\x89\xe5\x53\x48\x81\xec\x88\x00\x00\x00\x48\x89\xfb\x48\x8d\x7d\xc8\x48\x89\xde\xe8\x2A\x21\x11\x00\x8a\x4d\xc8\x89\xc8" ) };

#endif

}
