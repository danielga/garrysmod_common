#include "Symbols.hpp"
#include "Symbol.hpp"
#include "Platform.hpp"

namespace Symbols
{

	const std::vector<Symbol> CBasePlayer_HandleClientLuaError = {
		Symbol::FromName("?HandleClientLuaError@@YAXPAVCBasePlayer@@PBD@Z"),
		Symbol::FromSignature("\x55\x8B\xEC\x83\xEC\x08\x8B\x0D\x2A\x2A\x2A\x2A\x57\x8B\x7D\x08")
	};

	const std::vector<Symbol> FileSystemFactory = {
		Symbol::FromName("?FileSystemFactory@@YAPAXPBDPAH@Z"),
		Symbol::FromSignature("\x55\x8B\xEC\x68\x2A\x2A\x2A\x2A\xFF\x75\x08\xE8")
	};

	const Symbol g_pFullFileSystem = Symbol::FromName("?g_pFullFileSystem@@3PAVIFileSystem@@A");

	const std::vector<Symbol> IServer = {
		Symbol::FromName("?sv@@3VCGameServer@@A"),
		Symbol::FromSignature("\x2A\x2A\x2A\x2A\xE8\x2A\x2A\x2A\x2A\x84\xC0\x74\x2A\x83\x3D\x2A\x2A\x2A\x2A\x03")
	};

	const std::vector<Symbol> CNetChan_ProcessMessages = {
		Symbol::FromName("?ProcessMessages@CNetChan@@AAE_NAAVbf_read@@@Z"),
		Symbol::FromSignature("\x55\x8B\xEC\x83\xEC\x2C\xF7\x05")
	};

	const std::vector<Symbol> CBaseClient_ConnectionStart = {
		Symbol::FromName("?ConnectionStart@CBaseClient@@UAEXPAVINetChannel@@@Z"),
		Symbol::FromSignature("\x55\x8B\xEC\x2A\x53\x56\x57\x8B\xD9\x6A\x1C\x89\x5D\xFC\x2A\x2A\x2A\x2A\x2A\x8B\xD0\x83\xC4\x04\x85\xD2\x2A\x2A\xC7")
	};

	const std::vector<Symbol> CBaseClientState_ConnectionStart = {
		Symbol::FromName("?ConnectionStart@CBaseClientState@@UAEXPAVINetChannel@@@Z"),
		Symbol::FromSignature("\x55\x8B\xEC\x2A\x53\x56\x57\x6A\x1C\x8B\xF9\x2A\x2A\x2A\x2A\x2A\x8B\xC8\x83\xC4\x04\x85\xC9\x2A\x2A\xC7\x41\x08\x00")
	};

	const std::vector<Symbol> CLC_CmdKeyValues_Constructor = {
		Symbol::FromSignature("\x55\x8B\xEC\x8B\x45\x2A\x89\x41\x2A\x8B\xC1\xC6\x41\x2A\x01\xC7\x41\x2A\x00\x00\x00\x00\xC7\x01\x2A\x2A\x2A\x2A\x5D\xC2\x04\x00")
	};

	const std::vector<Symbol> SVC_CreateStringTable_Constructor = {
		Symbol::FromSignature("\x56\x8B\xF1\x8D\x4E\x2A\xC6\x46\x2A\x01")
	};

	const std::vector<Symbol> SVC_CmdKeyValues_Constructor = {
		Symbol::FromSignature("\x55\x8B\xEC\x8B\x45\x2A\x89\x41\x2A\x8B\xC1\xC6\x41\x2A\x01\xC7\x41\x2A\x00\x00\x00\x00\xC7\x01\x2A\x2A\x2A\x2A\x5D\xC2\x04\x00")
	};

	const std::vector<Symbol> CBaseServer_RecalculateTags = {
		Symbol::FromName("?RecalculateTags@CBaseServer@@QAEXXZ"),
		Symbol::FromSignature("\x55\x8B\xEC\x83\xEC\x18\x8B\x0D")
	};

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4996 )
#endif

	const std::vector<Symbol> SteamGameServerAPIContext = {
		Symbol::FromName("?s_SteamGameServerAPIContext@@3VCSteamGameServerAPIContext@@A"),
		Symbol::FromSignature("\x2A\x2A\x2A\x2A\xE8\x2A\x2A\x2A\x2A\x6A\x00\x68\x2A\x2A\x2A\x2A\xFF\x55\x08\x83\xC4\x08\xA")
	};

#ifdef _MSC_VER
#pragma warning( pop )
#endif

	const std::vector<Symbol> GModDataPack_SendFileToClient = {
		Symbol::FromName("?SendFileToClient@GModDataPack@@QAEXHH@Z"),
		Symbol::FromSignature("\x55\x8B\xEC\x83\xEC\x2A\x53\x8B\x5D\x0C\x56\x8B\xF1\x89\x75\xF8")
	};

	const std::vector<Symbol> CNetChan_IsValidFileForTransfer = {
		Symbol::FromName("?IsValidFileForTransfer@CNetChan@@SA_NPBD@Z"),
		Symbol::FromSignature("\x55\x8B\xEC\x53\x8B\x5D\x08\x56\x57\x85\xDB\x0F\x84")
	};

	const std::vector<Symbol> net_sockets = {
		Symbol::FromSignature("\x2A\x2A\x2A\x2A\xFF\x70\x40\xFF\x70\x30")
	};

	const Symbol GMOD_GetNetSocket = Symbol::FromName("?GMOD_GetNetSocket@@YAPAUnetsocket_t@@H@Z");

	const std::vector<Symbol> GModDataPack_AddOrUpdateFile = {
		Symbol::FromName("?AddOrUpdateFile@GModDataPack@@QAEXPAULuaFile@@_N@Z"),
		Symbol::FromSignature("\x55\x8B\xEC\x81\xEC\x80\x00\x00\x00\x53\x8B\xD9\x56\x8B\x75\x08\x57\x8B\x03\x89\x5D\xFC\x8D\x7E\x04")
	};

	const Symbol Steam3Server =
		Symbol::FromSignature("\xB8\x2A\x2A\x2A\x2A\xC3\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\x55\x8B\xEC\x68\x2A\x2A\x2A\x2A\xFF\x15\x2A\x2A\x2A\x2A\x50\xFF\x15");

	const std::vector<Symbol> GlobalVars = {
		Symbol::FromSignature("\x2A\x2A\x2A\x2A\x8D\x45\x2A\x6A\x01\x50\xE8\x2A\x2A\x2A\x2A\x8D\x45")
	};

	const std::vector<Symbol> AdvancedLuaErrorReporter = {
		Symbol::FromName("?AdvancedLuaErrorReporter@@YAHPAUlua_State@@@Z"),
		Symbol::FromSignature("\x55\x8B\xEC\x83\xEC\x5C")
	};

	const std::vector<Symbol> NET_ProcessSocket = { Symbol::FromSignature("\x55\x8B\xEC\x83\xEC\x08\x8B\x0D\x2A\x2A\x2A\x2A\x8B\x81") };

	const std::vector<Symbol> NET_CreateNetChannel = {
		Symbol::FromSignature("\x55\x8B\xEC\x80\x7D\x2A\x00\x53\x8B\x5D")
	};

	const std::vector<Symbol> HandleChange = {
		Symbol::FromSignature("\x55\x8b\xec\x83\xec\x60\x56\x8b\x75\x08\x8d\x45\xd0\x56\x50\xe8\x2A\x2A\x2A\x2A\x83\xc4\x08\x83\x7d\xe0\x00\x0f\x84")
	};

}
