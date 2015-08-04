#pragma once

#include "LuaObject.h"
#include <stdexcept>

namespace GarrysMod
{
	namespace Lua
	{
		class AutoObject
		{
		public:
			AutoObject( ) :
				lua_object( nullptr )
			{ };

			AutoObject( ILuaObject *obj ) :
				lua_object( obj )
			{ };

			~AutoObject( )
			{
				if( IsValid( ) )
				{
					lua_object->UnReference( );
					lua_object = nullptr;
				}
			};

			bool IsValid( ) const
			{
				return lua_object != nullptr;
			}

			explicit operator bool( ) const
			{
				return IsValid( );
			}

			ILuaObject *operator->( ) const
			{
				if( !IsValid( ) )
					throw std::runtime_error( "invalid ILuaObject" );

				return lua_object;
			};

			operator ILuaObject *( ) const
			{
				if( !IsValid( ) )
					throw std::runtime_error( "invalid ILuaObject" );

				return lua_object;
			};

			const AutoObject &operator=( const ILuaObject *obj )
			{
				lua_object = const_cast<ILuaObject *>( obj );
				return *this;
			}

		private:
			ILuaObject *lua_object;
		};
	}
}
