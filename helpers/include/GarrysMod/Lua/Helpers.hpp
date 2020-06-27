#pragma once

#include <cstdint>
#include <cstring>
#include <string>
#include <sstream>

#include <GarrysMod/Lua/LuaInterface.h>
#include <GarrysMod/Lua/Interface.h>
#include <lua.hpp>

namespace LuaHelpers
{

static int32_t LuaErrorTraceback( lua_State *state )
{
	GarrysMod::Lua::ILuaInterface *lua = static_cast<GarrysMod::Lua::ILuaInterface *>( state->luabase );
	lua->SetState( state );

	std::string spaces( "\n  " );
	std::ostringstream stream;

	if( lua->IsType( 1, GarrysMod::Lua::Type::String ) )
		stream << lua->GetString( 1 );

	lua_Debug dbg;
	for(
		int32_t lvl = 1;
		lua->GetStack( lvl, &dbg ) == 1;
		++lvl, std::memset( &dbg, 0, sizeof( dbg ) )
	)
	{
		if( lua->GetInfo( "Sln", &dbg ) == 0 )
			break;

		stream
			<< spaces
			<< lvl
			<< ". "
			<< ( dbg.name == nullptr ? "unknown" : dbg.name )
			<< " - "
			<< dbg.short_src
			<< ':'
			<< dbg.currentline;
		spaces += ' ';
	}

	lua->PushString( stream.str( ).c_str( ) );
	return 1;
}

static int32_t PushHookRun( GarrysMod::Lua::ILuaBase *lua, const char *hook_name )
{
	lua->PushCFunction( LuaErrorTraceback );

	lua->GetField( GarrysMod::Lua::INDEX_GLOBAL, "hook" );
	if( !lua->IsType( -1, GarrysMod::Lua::Type::Table ) )
	{
		lua->Pop( 2 );
		return 0;
	}

	lua->GetField( -1, "Run" );
	if( !lua->IsType( -1, GarrysMod::Lua::Type::Function ) )
	{
		lua->Pop( 3 );
		return 0;
	}

	lua->Remove( -2 );

	lua->PushString( hook_name );

	return 3;
}

static bool CallHookRun(
	GarrysMod::Lua::ILuaBase *lua,
	int32_t args = 0,
	int32_t rets = 0,
	bool print_error = true,
	bool pop_error = true
)
{
	if( lua->PCall( 1 + args, rets, -args - 3 ) != 0 )	/* -args for the last arg, -1 for hook name,
														-1 for hook.Run and -1 for debug.traceback */
	{
		if( print_error )
			static_cast<GarrysMod::Lua::ILuaInterface *>( lua )->ErrorNoHalt(
				"\n%s\n\n",
				lua->GetString( -1 )
			);

		if( pop_error )
			lua->Pop( 2 ); /* pop error string and debug.traceback */
		else
			lua->Remove( -2 ); /* remove debug.traceback */

		return false;
	}

	lua->Remove( -rets - 1 ); /* remove debug.traceback and leave the returns in the stack */
	return true;
}

}
