#pragma once

#include <Platform.hpp>

#if defined SYSTEM_WINDOWS

#if defined ARCHITECTURE_X86_64

#define GMCOMMON_CALLING_CONVENTION __fastcall
#define GMCOMMON_CALLING_CONVENTION_STD __fastcall

#else

#define GMCOMMON_CALLING_CONVENTION __thiscall
#define GMCOMMON_CALLING_CONVENTION_STD __stdcall

#endif

#else

#define GMCOMMON_CALLING_CONVENTION
#define GMCOMMON_CALLING_CONVENTION_STD

#endif

class CBasePlayer;
class CNetChan;
class bf_read;
class CBaseClient;
class INetChannel;
class INetChannelHandler;
class CBaseClientState;
class CBaseServer;
class GModDataPack;
struct netsocket_t;
class LuaFile;
class CSteam3Server;
struct lua_State;
class IConnectionlessPacketHandler;
struct ns_address;

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

typedef void ( GMCOMMON_CALLING_CONVENTION *CLC_CmdKeyValues_Constructor_t )( CBaseClientState *clientstate, void *keyvalues );
CLC_CmdKeyValues_Constructor_t CLC_CmdKeyValues_Constructor( );

typedef void ( GMCOMMON_CALLING_CONVENTION *SVC_CreateStringTable_Constructor_t )( CBaseClientState *clientstate );
SVC_CreateStringTable_Constructor_t SVC_CreateStringTable_Constructor( );

typedef void ( GMCOMMON_CALLING_CONVENTION *SVC_CmdKeyValues_Constructor_t )( CBaseClientState *clientstate, void *keyvalues );
SVC_CmdKeyValues_Constructor_t SVC_CmdKeyValues_Constructor( );

typedef void ( GMCOMMON_CALLING_CONVENTION *CBaseServer_RecalculateTags_t )( CBaseServer *server );
CBaseServer_RecalculateTags_t CBaseServer_RecalculateTags( );

typedef void ( GMCOMMON_CALLING_CONVENTION *GModDataPack_SendFileToClient_t )( GModDataPack *pack, int client, int fileID );
GModDataPack_SendFileToClient_t GModDataPack_SendFileToClient( );

typedef bool ( *CNetChan_IsValidFileForTransfer_t )( const char *filepath );
CNetChan_IsValidFileForTransfer_t CNetChan_IsValidFileForTransfer( );

typedef const netsocket_t *( *GMOD_GetNetSocket_t )( int idx );
GMOD_GetNetSocket_t GMOD_GetNetSocket( );

typedef void ( GMCOMMON_CALLING_CONVENTION *GModDataPack_AddOrUpdateFile_t )( GModDataPack *pack, LuaFile *file, bool force );
GModDataPack_AddOrUpdateFile_t GModDataPack_AddOrUpdateFile( );

typedef CSteam3Server *( *Steam3Server_t )( );
Steam3Server_t Steam3Server( );

typedef int ( *AdvancedLuaErrorReporter_t )( lua_State *L );
AdvancedLuaErrorReporter_t AdvancedLuaErrorReporter( );

typedef void ( *NET_ProcessSocket_t )( int sock, IConnectionlessPacketHandler *handler );
NET_ProcessSocket_t NET_ProcessSocket( );

typedef INetChannel *( *NET_CreateNetChannel_t )(
	int socket,
	const ns_address *adr,
	const char * name,
	INetChannelHandler *handler,
	const unsigned char *pbEncryptionKey,
	bool bForceNewChannel
);
NET_CreateNetChannel_t NET_CreateNetChannel( );

typedef void ( GMCOMMON_CALLING_CONVENTION_STD *HandleChange_t )( const std::string& path );
HandleChange_t HandleChange( );

}
