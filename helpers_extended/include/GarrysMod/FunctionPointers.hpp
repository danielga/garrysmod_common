#pragma once

#include <Platform.hpp>

#if defined SYSTEM_WINDOWS

#if defined ARCHITECTURE_X86_64

#define GMCOMMON_CALLING_CONVENTION __fastcall

#else

#define GMCOMMON_CALLING_CONVENTION __thiscall

#endif

#else

#define GMCOMMON_CALLING_CONVENTION

#endif

class CBasePlayer;
class CNetChan;
class bf_read;
class CBaseClient;
class INetChannel;
class CBaseClientState;
class CBaseServer;
class GModDataPack;
struct netsocket_t;
class LuaFile;
class CSteam3Server;

namespace FunctionPointers
{

typedef void ( *CBasePlayer_HandleClientLuaError_t )( CBasePlayer *player, const char *error );
CBasePlayer_HandleClientLuaError_t CBasePlayer_HandleClientLuaError( );

typedef void *( *CreateInterfaceFn )( const char *pName, int *pReturnCode );
CreateInterfaceFn FileSystemFactory( );

typedef bool ( GMCOMMON_CALLING_CONVENTION *CNetChan_ProcessMessages_t )( CNetChan *netchan, bf_read &buf );
CNetChan_ProcessMessages_t CNetChan_ProcessMessages( );

typedef void ( GMCOMMON_CALLING_CONVENTION *CBaseClient_ConnectionStart_t )( CBaseClient *client, INetChannel *netchan );
CBaseClient_ConnectionStart_t CBaseClient_ConnectionStart( );

typedef void ( GMCOMMON_CALLING_CONVENTION *CBaseClientState_ConnectionStart_t )( CBaseClientState *clientstate, INetChannel *netchan );
CBaseClientState_ConnectionStart_t CBaseClientState_ConnectionStart( );

typedef void ( GMCOMMON_CALLING_CONVENTION *CBaseServer_RecalculateTags_t )( CBaseServer *server );
CBaseServer_RecalculateTags_t CBaseServer_RecalculateTags( );

typedef void ( GMCOMMON_CALLING_CONVENTION *GModDataPack_SendFileToClient_t )( GModDataPack *pack, int client, int fileID );
GModDataPack_SendFileToClient_t GModDataPack_SendFileToClient( );

typedef bool ( *CNetChan_IsValidFileForTransfer_t )( const char *filepath );
CNetChan_IsValidFileForTransfer_t CNetChan_IsValidFileForTransfer( );

typedef netsocket_t *( *GMOD_GetNetSocket_t )( int idx );
GMOD_GetNetSocket_t GMOD_GetNetSocket( );

typedef void ( GMCOMMON_CALLING_CONVENTION *GModDataPack_AddOrUpdateFile_t )( GModDataPack *pack, LuaFile *file, bool force );
GModDataPack_AddOrUpdateFile_t GModDataPack_AddOrUpdateFile( );

typedef CSteam3Server *( *Steam3Server_t )( );
Steam3Server_t Steam3Server( );

}
