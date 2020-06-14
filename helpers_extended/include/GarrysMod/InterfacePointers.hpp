#pragma once

class IFileSystem;
class IVEngineServer;
class IVEngineClient;
class IServer;
class CSteamGameServerAPIContext;

namespace InterfacePointers
{

namespace Internal
{

namespace Client
{

IFileSystem *FileSystem( );

}

namespace Server
{

IFileSystem *FileSystem( );

}

}

inline IFileSystem *FileSystem( )
{

#if IS_SERVERSIDE

	return Internal::Server::FileSystem( );

#else

	return Internal::Client::FileSystem( );

#endif

}

IVEngineServer *VEngineServer( );
IVEngineClient *VEngineClient( );

IServer *Server( );

CSteamGameServerAPIContext *SteamGameServerAPIContext( );

}
