#pragma once

#include "LuaBase.h"
#include <stdexcept>

namespace GarrysMod
{
	namespace Lua
	{
		class AutoStack
		{
		public:
			AutoStack( GarrysMod::Lua::ILuaBase *lua ) :
				lua_base( lua )
			{
				if( lua == nullptr )
					throw std::runtime_error( "ILuaBase can't be NULL!" );

				stack_start = lua->Top( );
			}

			~AutoStack( )
			{
				Reset( );
			}

			int32_t GetStackDifference( ) const
			{
				return lua_base->Top( ) - stack_start;
			}

			bool Reset( )
			{
				int32_t diff = GetStackDifference( );
				if( diff < 0 )
					return false;

				if( diff > 0 )
					lua_base->Pop( diff );

				return true;
			}

		private:
			ILuaBase *lua_base;
			int32_t stack_start;
		};
	}
}
