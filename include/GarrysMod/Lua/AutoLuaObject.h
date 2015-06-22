#pragma once

#include "LuaObject.h"

namespace GarrysMod
{
	namespace Lua
	{
		class AutoLuaObject
		{
		public:
			AutoLuaObject( ) :
				lua_object( nullptr )
			{ };

			AutoLuaObject( ILuaObject *obj ) :
				lua_object( obj )
			{ };

			~AutoLuaObject( )
			{
				if( !lua_object )
					return;

				lua_object->UnReference( );
			};

			ILuaObject *operator->( )
			{
				return lua_object;
			};

			ILuaObject *operator->( ) const
			{
				return lua_object;
			};

			operator ILuaObject *( )
			{
				return lua_object;
			};

			operator ILuaObject *( ) const
			{
				return lua_object;
			};

			const AutoLuaObject &operator=( const ILuaObject *obj )
			{
				lua_object = const_cast<ILuaObject *>( obj );
				return *this;
			}

		private:
			ILuaObject *lua_object;
		};
	}
}