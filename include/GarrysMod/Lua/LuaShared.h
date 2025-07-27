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

		struct LuaFile
		{
			~LuaFile();
			int time;
#ifdef WIN32
			std::string name;
			std::string source;
			std::string contents;
#else
			const char *name;
			const char *source;
			const char *contents;
#endif
			Bootil::AutoBuffer compressed;
#ifndef WIN32
			int random; // Unknown thing
#endif
			unsigned int timesloadedserver;
			unsigned int timesloadedclient;
		};

		struct LuaFindResult
		{
			std::string fileName;
			bool isFolder;
		};

		class ILuaShared
		{
		public:
			virtual ~ILuaShared( ) = 0;
			// NOTE: magicBool - could maybe be bIsDedicated? true if its a dedicated server?
			virtual void Init( void *( *interfaceFactory )( const char *, int * ), bool magicBool, CSteamAPIContext *context, IGet *pGet ) = 0;
			virtual void Shutdown( ) = 0;
			virtual void DumpStats( ) = 0;
			virtual ILuaInterface *CreateLuaInterface( unsigned char realm, bool unknown ) = 0;
			virtual void CloseLuaInterface( ILuaInterface *luaInterface ) = 0;
			virtual ILuaInterface *GetLuaInterface( unsigned char realm ) = 0;
			virtual LuaFile *LoadFile( const std::string &path, const std::string &pathId, bool fromDatatable, bool fromFile ) = 0;
			virtual LuaFile *GetCache( const std::string &fileName );
			virtual void MountLua( const char *pathID ) = 0;
			virtual void MountLuaAdd( const char *file, const char *pathID ) = 0;
			virtual void UnMountLua( const char *pathID ) = 0;
			// Probably: unknown1 = fileName | unknown2 = pathID
			virtual void SetFileContents( const char *unknown1, const char *unknown2 ) = 0;	
			virtual void SetLuaFindHook( LuaClientDatatableHook *hook ) = 0;
			virtual void FindScripts( const std::string &path, const std::string &pathID, std::vector<LuaFindResult> &outPut ) = 0;
			virtual const char *GetStackTraces( ) = 0;
			// probably unknown = fileName?
			virtual void InvalidateCache( const std::string &unknown ) = 0;
			virtual void EmptyCache( ) = 0;
			virtual bool ScriptExists( const std::string &file, const std::string &path, bool unknown ) = 0;
		};
	}
}
