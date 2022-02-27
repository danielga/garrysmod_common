#pragma once

class IFileSystem;
class IVEngineServer;
class IVEngineClient;
class IServer;
class CSteamGameServerAPIContext;
class ICvar;
class IServerGameDLL;
class INetworkStringTableContainer;
class CGlobalVars;

namespace InterfacePointers
{

namespace Internal
{

namespace Client
{

IFileSystem *FileSystem( );
INetworkStringTableContainer *NetworkStringTableContainer( );
CGlobalVars *GlobalVars( );

}

namespace Server
{

IFileSystem *FileSystem( );
INetworkStringTableContainer *NetworkStringTableContainer( );
CGlobalVars *GlobalVars( );

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

inline IFileSystem *FileSystemServer( )
{
	return Internal::Server::FileSystem( );
}

inline IFileSystem *FileSystemClient( )
{
	return Internal::Client::FileSystem( );
}

IVEngineServer *VEngineServer( );
IVEngineClient *VEngineClient( );

IServer *Server( );

CSteamGameServerAPIContext *SteamGameServerAPIContext( );

ICvar *Cvar( );

IServerGameDLL *ServerGameDLL( );

inline INetworkStringTableContainer *NetworkStringTableContainer( )
{

#if IS_SERVERSIDE

	return Internal::Server::NetworkStringTableContainer( );

#else

	return Internal::Client::NetworkStringTableContainer( );

#endif

}

inline INetworkStringTableContainer *NetworkStringTableContainerServer( )
{
	return Internal::Server::NetworkStringTableContainer( );
}

inline INetworkStringTableContainer *NetworkStringTableContainerClient( )
{
	return Internal::Client::NetworkStringTableContainer( );
}

inline CGlobalVars *GlobalVars( )
{

#if IS_SERVERSIDE

	return Internal::Server::GlobalVars( );

#else

	return Internal::Client::GlobalVars( );

#endif

}

}
