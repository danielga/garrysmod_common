#pragma once

#include <vector>

#include "Symbol.hpp"
#include "Platform.hpp"

namespace Symbols
{

extern const std::vector<Symbol> CBasePlayer_HandleClientLuaError;
extern const std::vector<Symbol> FileSystemFactory;
extern const Symbol g_pFullFileSystem;
extern const std::vector<Symbol> IServer;
extern const std::vector<Symbol> CNetChan_ProcessMessages;
extern const std::vector<Symbol> CBaseClient_ConnectionStart;
extern const std::vector<Symbol> CBaseClientState_ConnectionStart;
extern const std::vector<Symbol> CLC_CmdKeyValues_Constructor;
extern const std::vector<Symbol> SVC_CreateStringTable_Constructor;
extern const std::vector<Symbol> SVC_CmdKeyValues_Constructor;
extern const std::vector<Symbol> CBaseServer_RecalculateTags;
extern DEPRECATED_WITH_SUBSTITUTE( Symbols::Steam3Server ) const std::vector<Symbol> SteamGameServerAPIContext;
extern const std::vector<Symbol> GModDataPack_SendFileToClient;
extern const std::vector<Symbol> CNetChan_IsValidFileForTransfer;
extern const std::vector<Symbol> net_sockets;
extern const Symbol GMOD_GetNetSocket;
extern const std::vector<Symbol> GModDataPack_AddOrUpdateFile;
extern const Symbol Steam3Server;
extern const std::vector<Symbol> GlobalVars;
extern const std::vector<Symbol> AdvancedLuaErrorReporter;
extern const std::vector<Symbol> NET_ProcessSocket;
extern const std::vector<Symbol> NET_CreateNetChannel;
extern const std::vector<Symbol> HandleChange;

}
