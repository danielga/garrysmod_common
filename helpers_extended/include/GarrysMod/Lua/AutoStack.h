#pragma once

#include <stdexcept>

#include <GarrysMod/Lua/LuaInterface.h>

namespace GarrysMod
{
	namespace Lua
	{
		class AutoStack
		{
		public:
			AutoStack( ILuaBase *lua_base ) :
				lua( static_cast<ILuaInterface *>( lua_base ) )
			{
				if( lua == nullptr )
					throw std::runtime_error( "ILuaBase can't be NULL!" );

				stack_start = lua->Top( );
			}

			virtual ~AutoStack( )
			{
				Reset( );
			}

			int32_t GetStackDifference( ) const
			{
				return lua->Top( ) - stack_start;
			}

			bool Reset( )
			{
				int32_t diff = GetStackDifference( );
				if( diff < 0 )
					return false;

				if( diff > 0 )
					lua->Pop( diff );

				return true;
			}

		protected:
			ILuaInterface *lua;
			int32_t stack_start;
		};
	}
}
