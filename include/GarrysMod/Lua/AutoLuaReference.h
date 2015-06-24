#pragma once

#include "LuaBase.h"

namespace GarrysMod
{
	namespace Lua
	{
		class AutoLuaReference
		{
		public:
			AutoLuaReference( ) :
				lua( nullptr ),
				lua_ref( -2 )
			{ };

			AutoLuaReference( GarrysMod::Lua::ILuaBase *luabase ) :
				lua( luabase ),
				lua_ref( luabase->ReferenceCreate( ) )
			{ };

			AutoLuaReference( GarrysMod::Lua::ILuaBase *luabase, int ref ) :
				lua( luabase ),
				lua_ref( ref )
			{ };

			~AutoLuaReference( )
			{
				if( IsValid( ) )
					lua->ReferenceFree( lua_ref );
			};

			bool IsValid( ) const
			{
				return lua != nullptr && lua_ref != -2;
			}

			explicit operator bool( ) const
			{
				return IsValid( );
			}

			operator int( ) const
			{
				return lua_ref;
			};

			bool Create( GarrysMod::Lua::ILuaBase *luabase = nullptr )
			{
				Free( );

				if( luabase != nullptr )
					lua = luabase;

				lua_ref = lua->ReferenceCreate( );
				return IsValid( );
			}

			bool Free( )
			{
				if( !IsValid( ) )
					return false;

				lua->ReferenceFree( lua_ref );
				lua_ref = -2;
				return true;
			}

			bool Push( )
			{
				if( !IsValid( ) )
					return false;

				lua->ReferencePush( lua_ref );
				return true;
			}

		private:
			GarrysMod::Lua::ILuaBase *lua;
			int lua_ref;
		};
	}
}