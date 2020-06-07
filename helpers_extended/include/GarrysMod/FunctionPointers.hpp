#pragma once

class CBasePlayer;

namespace FunctionPointers
{

#if IS_SERVERSIDE

typedef void ( *HandleClientLuaError_t )( CBasePlayer *player, const char *error );
HandleClientLuaError_t HandleClientLuaError( );

#endif

typedef void *( *CreateInterfaceFn )( const char *pName, int *pReturnCode );
CreateInterfaceFn FileSystemFactory( );

}
