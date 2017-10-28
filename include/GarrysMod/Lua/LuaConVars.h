#pragma once

#define GMOD_LUACONVARS_INTERFACE "LUACONVARS001"

class CCommand;

namespace GarrysMod
{
	namespace Lua
	{
		class ILuaConVars
		{
		public:
			virtual ~ILuaConVars( ) = 0;
			void Init( ) = 0;
			void CreateConVar( const char *, const char *, const char *, int ) = 0;
			void CreateConCommand( const char *, const char *, int, void ( * )( const CCommand & ), int ( * )( const char *, char ( * )[128] ) ) = 0;
			void DestroyManaged( ) = 0;
			void Cache( const char *, const char * ) = 0;
			void ClearCache( ) = 0;
		};
	}
}
