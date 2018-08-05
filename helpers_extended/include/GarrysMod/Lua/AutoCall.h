#pragma once

#include "AutoStack.h"

namespace GarrysMod
{
	namespace Lua
	{
		class AutoCall : public AutoStack
		{
		public:
			AutoCall( GarrysMod::Lua::ILuaBase *lua_base, bool custom_error_function = false ) :
				AutoStack( lua_base ), use_custom_error_function( custom_error_function )
			{
				lua->GetField( GarrysMod::Lua::INDEX_GLOBAL, "debug" );
				if( !lua->IsType( -1, GarrysMod::Lua::Type::TABLE ) )
					throw std::runtime_error( "Global debug is not a table!" );

				lua->GetField( -1, "traceback" );
				lua->Remove( -2 );
				if( !lua->IsType( -1, GarrysMod::Lua::Type::FUNCTION ) )
					throw std::runtime_error( "Global debug.traceback is not a function!" );

				++stack_offset;
			}

			void Call( int32_t results )
			{
				lua->Call( GetStackDifference( ) - stack_offset, results );
			}

			bool PCall( int32_t results, int32_t errorFunc = 0 )
			{
				if( errorFunc == 0 && use_custom_error_function )
					errorFunc = stack_start + 1;

				return lua->PCall( GetStackDifference( ) - stack_offset, results, errorFunc ) == 0;
			}

		protected:
			int32_t stack_offset = 0;
			bool use_custom_error_function;
		};
	}
}
