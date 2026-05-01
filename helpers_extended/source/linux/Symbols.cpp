#include "Symbols.hpp"
#include "Symbol.hpp"
#include "Platform.hpp"

namespace Symbols
{

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

		Symbol::FromSignature( "\x2A\x2A\x2A\x2A\x8D\x49\x2A\x33\xFF" )

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

        Symbol::FromSignature( "\x55\x89\xe5\x57\x56\x53\x8d\x5d\x98\x83\xec\x7c\x8b\x75\x08\x89\x1c\x24\x89\x74\x24\x04\xe8\x2A\x2A\x2A\x2A\x8b\x45\x98\x83\xec\x04\x8b\x50\xf4\x85\xd2\x0f\x84" )

#elif ARCHITECTURE_X86_64

        Symbol::FromSignature( "\x55\x48\x89\xfe\x48\x89\xe5\x41\x57\x41\x56\x41\x55\x41\x54\x4c\x8d\x65\x80\x53\x48\x89\xfb\x4c\x89\xe7\x48\x83\xec\x68\xe8\x2A\x2A\x2A\x2A\x48\x8b\x45\x80\x48\x83\x78\xe8\x00\x75" )

#endif

    };

}
