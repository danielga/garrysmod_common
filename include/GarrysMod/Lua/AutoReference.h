#pragma once

#include "LuaBase.h"
#include <stdint.h>

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

			AutoReference( GarrysMod::Lua::ILuaBase *luabase, int32_t index ) :
				lua( luabase ),
				ref( -2 )
			{
				lua->Push( index );
				ref = lua->ReferenceCreate( );
			};

			~AutoReference( )
			{
				Free( );
			};

			bool IsValid( ) const
			{
				return lua != nullptr && ref != -2;
			}

			operator bool( ) const
			{
				return IsValid( );
			}

			operator int32_t( ) const
			{
				return ref;
			};

			void Setup( GarrysMod::Lua::ILuaBase *luabase )
			{
				Free( );
				lua = luabase;
			}

			bool Create( )
			{
				if( !Free( ) )
					return false;

				ref = lua->ReferenceCreate( );
				return IsValid( );
			}

			bool Create( int32_t index )
			{
				if( !Free( ) )
					return false;

				lua->Push( index );
				ref = lua->ReferenceCreate( );
				return IsValid( );
			}

			bool Free( )
			{
				if( lua == nullptr )
					return false;

				if( ref >= 0 )
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
			int32_t ref;
		};
	}
}
