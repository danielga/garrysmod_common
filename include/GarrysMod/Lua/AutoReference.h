#pragma once

#include "LuaBase.h"

namespace GarrysMod
{
	namespace Lua
	{
		class AutoReference
		{
		public:
			AutoReference( ) :
				lua( nullptr ),
				ref( -2 )
			{ };

			AutoReference( GarrysMod::Lua::ILuaBase *luabase ) :
				lua( luabase ),
				ref( luabase->ReferenceCreate( ) )
			{ };

			AutoReference( GarrysMod::Lua::ILuaBase *luabase, int ref ) :
				lua( luabase ),
				ref( ref )
			{ };

			~AutoReference( )
			{
				if( IsValid( ) )
					lua->ReferenceFree( ref );
			};

			bool IsValid( ) const
			{
				return lua != nullptr && ref != -2;
			}

			explicit operator bool( ) const
			{
				return IsValid( );
			}

			operator int( ) const
			{
				return ref;
			};

			bool Create( GarrysMod::Lua::ILuaBase *luabase = nullptr )
			{
				Free( );

				if( luabase != nullptr )
					lua = luabase;

				ref = lua->ReferenceCreate( );
				return IsValid( );
			}

			bool Free( )
			{
				if( !IsValid( ) )
					return false;

				lua->ReferenceFree( ref );
				ref = -2;
				return true;
			}

			bool Push( )
			{
				if( !IsValid( ) )
					return false;

				lua->ReferencePush( ref );
				return true;
			}

		private:
			GarrysMod::Lua::ILuaBase *lua;
			int ref;
		};
	}
}