#pragma once

#include "LuaObject.h"

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
				if( !lua_object )
					return;

				lua_object->UnReference( );
			};

			ILuaObject *operator->( ) const
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