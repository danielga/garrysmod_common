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
			virtual ConVar *CreateConVar( const char *, const char *, const char *, int ) = 0;
			virtual ConCommand *CreateConCommand( const char *, const char *, int, void ( * )( const CCommand & ), int ( * )( const char *, char ( * )[128] ) ) = 0;
			virtual void DestroyManaged( ) = 0;
			virtual void Cache( const char *, const char * ) = 0;
			virtual void ClearCache( ) = 0;
		};
	}
}
