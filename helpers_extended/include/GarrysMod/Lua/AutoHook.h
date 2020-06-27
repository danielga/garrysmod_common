#pragma once

#include "AutoCall.h"

namespace GarrysMod
{
	namespace Lua
	{
		class AutoHook : public AutoCall
		{
		public:
			AutoHook( GarrysMod::Lua::ILuaBase *lua_base, const std::string &name ) :
				AutoCall( lua_base, true ), hook_name( name )
			{
				lua->GetField( GarrysMod::Lua::INDEX_GLOBAL, "hook" );
				if( !lua->IsType( -1, GarrysMod::Lua::Type::TABLE ) )
					throw std::runtime_error( "Global hook is not a table!" );

				lua->GetField( -1, "Run" );
				lua->Remove( -2 );
				if( !lua->IsType( -1, GarrysMod::Lua::Type::FUNCTION ) )
					throw std::runtime_error( "Global hook.Run is not a function!" );

				++stack_offset;

				lua->PushString( GetName( ) );
			}

			const char *GetName( ) const
			{
				return hook_name.c_str( );
			}

			bool Call( )
			{
				bool call_original = true;
				lua->Call( GetStackDifference( ) - stack_offset, 1 );
				if( lua->IsType( -1, GarrysMod::Lua::Type::BOOL ) )
					call_original = !lua->GetBool( -1 );

				Reset( );
				return call_original;
			}

			bool PCall( int32_t errorFunc = 0 )
			{
				if( errorFunc == 0 && use_custom_error_function )
					errorFunc = stack_start + 1;

				bool call_original = true;
				if( lua->PCall( GetStackDifference( ) - stack_offset, 1, errorFunc ) != 0 )
					lua->ErrorNoHalt( "\n[%s] %s\n\n", GetName( ), lua->GetString( -1 ) );
				else if( lua->IsType( -1, GarrysMod::Lua::Type::BOOL ) )
					call_original = !lua->GetBool( -1 );

				Reset( );
				return call_original;
			}
			
		private:
			std::string hook_name;
		};
	}
}
