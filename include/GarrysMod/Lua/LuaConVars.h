#pragma once

#define GMOD_LUACONVARS_INTERFACE "LUACONVARS001"

class ConVar;
class CCommand;
class ConCommand;

namespace GarrysMod
{
	namespace Lua
	{
		class ILuaConVars
		{
		public:
			virtual ~ILuaConVars( ) = 0;
			virtual void Init( ) = 0;
			virtual ConVar *CreateConVar( const char *name, const char *defaultValue, const char *helpString, int flags ) = 0;
			virtual ConCommand *CreateConCommand( const char *name, const char *helpString, int flags, void ( *callback )( const CCommand & ), int ( *completionFunc )( const char *, char ( * )[128] ) ) = 0;
			virtual void DestroyManaged( ) = 0;
			virtual void Cache( const char *key, const char *value ) = 0;
			virtual void ClearCache( ) = 0;
			virtual void SaveManaged( ) = 0;
		};
	}
}
