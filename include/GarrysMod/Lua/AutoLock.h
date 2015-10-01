#pragma once

#include "LuaInterface.h"
#include <stdexcept>

namespace GarrysMod
{
	namespace Lua
	{
		class AutoLock
		{
		public:
			AutoLock( ILuaInterface *lua ) :
				lua_interface( lua )
			{
				if( lua_interface == nullptr )
					throw std::runtime_error( "invalid ILuaInterface" );

				lua_interface->ThreadLock( );
			};

			~AutoLock( )
			{
				if( lua_interface != nullptr )
				{
					lua_interface->ThreadUnlock( );
					lua_interface = nullptr;
				}
			};

		private:
			ILuaInterface *lua_interface;
		};
	}
}
