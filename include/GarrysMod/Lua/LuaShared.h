#pragma once

#define GMOD_LUASHARED_INTERFACE "LUASHARED003"

#include <string>
#include <vector>

class LuaClientDatatableHook;
class IGet;
class CSteamAPIContext;

namespace GarrysMod
{
	namespace Lua
	{
		class ILuaInterface;

		namespace State
		{
			enum
			{
				CLIENT = 0,
				SERVER,
				MENU
			};

			static const char *Name[] = {
				"client",
				"server",
				"menu",

				0
			};
		}

		class ILuaShared
		{
		public:
			virtual ~ILuaShared( ) = 0;
			virtual void Init( void *( * )( const char *, int * ), bool, CSteamAPIContext *, IGet * ) = 0;
			virtual void Shutdown( ) = 0;
			virtual void DumpStats( ) = 0;
			virtual ILuaInterface *CreateLuaInterface( unsigned char, bool ) = 0;
			virtual void CloseLuaInterface( ILuaInterface * ) = 0;
			virtual ILuaInterface *GetLuaInterface( unsigned char ) = 0;
			virtual void LoadFile( const std::string &, const std::string &, bool, bool ) = 0;
			virtual void GetCache( const std::string & );
			virtual void MountLua( const char * ) = 0;
			virtual void MountLuaAdd( const char *, const char * ) = 0;
			virtual void UnMountLua( const char * ) = 0;
			virtual void SetFileContents( const char *, const char * ) = 0;	
			virtual void SetLuaFindHook( LuaClientDatatableHook * ) = 0;
			virtual void FindScripts( const std::string &, const std::string &, std::vector<std::string> & ) = 0;
		};
	}
}
