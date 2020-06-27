#pragma once

#define GMOD_LUASHARED_INTERFACE "LUASHARED003"

#include <string>
#include <vector>

class LuaClientDatatableHook;
class IGet;
class CSteamAPIContext;

namespace Bootil
{
	struct AutoBuffer
	{
		void *m_pData;
		unsigned int m_iSize;
		unsigned int m_iPos;
		unsigned int m_iWritten;
	};
}

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
				nullptr
			};
		}

		struct File
		{
			int time;
			std::string name;
			std::string source;
			std::string contents;
			Bootil::AutoBuffer compressed;
			unsigned int timesloadedserver;
			unsigned int timesloadedclient;
		};

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
			virtual File *LoadFile( const std::string &path, const std::string &pathId, bool fromDatatable, bool fromFile ) = 0;
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
