#pragma once

#define GMOD_LUACONVARS_INTERFACE "LUACONVARS001"

class ConVar;
class CCommand;

namespace GarrysMod
{
	namespace Lua
	{
		class ILuaConVars
		{
		public:
			virtual ~ILuaConVars( ) = 0;
			virtual void Init( ) = 0;
			virtual ConVar *CreateConVar( const char* name, const char* defaultValue, const char* helpString, int flags ) = 0;
			virtual ConCommand *CreateConCommand( const char* name, const char* helpString, int flags, void ( * )( const CCommand & ) callback, int ( * )( const char *, char ( * )[128] ) completionFunc ) = 0;
			virtual void DestroyManaged( ) = 0;
			virtual void Cache( const char* key, const char* value ) = 0;
			virtual void ClearCache( ) = 0;
		};
	}
}
