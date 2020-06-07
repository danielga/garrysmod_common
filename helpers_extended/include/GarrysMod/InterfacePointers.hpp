#pragma once

class IFileSystem;
class IVEngineServer;
class IVEngineClient;

namespace InterfacePointers
{

namespace Client
{

IFileSystem *FileSystem( );

}

namespace Server
{

IFileSystem *FileSystem( );

}

inline IFileSystem *FileSystem( )
{

#if IS_SERVERSIDE

	return Server::FileSystem( );

#else

	return Client::FileSystem( );

#endif

}

IVEngineServer *VEngineServer( );
IVEngineClient *VEngineClient( );

}
