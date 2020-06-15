#pragma once

#include <Platform.hpp>

#if defined SYSTEM_WINDOWS

#if defined ARCHITECTURE_X86_64

#define CALLING_CONVENTION __fastcall

#else

#define CALLING_CONVENTION __thiscall

#endif

#else

#define CALLING_CONVENTION

#endif

class CBasePlayer;
class CNetChan;
class bf_read;
class CBaseClient;
class INetChannel;
class CBaseClientState;
class CBaseServer;
class GModDataPack;

namespace FunctionPointers
{

typedef void ( *CBasePlayer_HandleClientLuaError_t )( CBasePlayer *player, const char *error );
CBasePlayer_HandleClientLuaError_t CBasePlayer_HandleClientLuaError( );

typedef void *( *CreateInterfaceFn )( const char *pName, int *pReturnCode );
CreateInterfaceFn FileSystemFactory( );

typedef bool ( CALLING_CONVENTION *CNetChan_ProcessMessages_t )( CNetChan *netchan, bf_read &buf );
CNetChan_ProcessMessages_t CNetChan_ProcessMessages( );

typedef void ( CALLING_CONVENTION *CBaseClient_ConnectionStart_t )( CBaseClient *client, INetChannel *netchan );
CBaseClient_ConnectionStart_t CBaseClient_ConnectionStart( );

typedef void ( CALLING_CONVENTION *CBaseClientState_ConnectionStart_t )( CBaseClientState *clientstate, INetChannel *netchan );
CBaseClientState_ConnectionStart_t CBaseClientState_ConnectionStart( );

typedef void ( CALLING_CONVENTION *CBaseServer_RecalculateTags_t )( CBaseServer *server );
CBaseServer_RecalculateTags_t CBaseServer_RecalculateTags( );

typedef void ( CALLING_CONVENTION *GModDataPack_SendFileToClient_t )( GModDataPack *pack, int client, int fileID );
GModDataPack_SendFileToClient_t GModDataPack_SendFileToClient( );

}
