#pragma once

#include <cstdint>
#include <string>
#include <sstream>
#include <lua.hpp>

namespace helpers
{

inline std::string GetBinaryFileName(
	const std::string &name,
	bool libprefix = true,
	bool srvsuffix = true,
	const std::string &extraprefix = ""
)
{

#if defined _WIN32

	(void)libprefix;
	(void)srvsuffix;
	return extraprefix + name + ".dll";

#elif defined __linux

	return extraprefix + ( libprefix ? "lib" : "" ) + name + ( srvsuffix ? "_srv.so" : ".so" );

#elif defined __APPLE__

	(void)libprefix;
	(void)srvsuffix;
	return extraprefix + name + ".dylib";

#endif

}

static int32_t LuaErrorTraceback( lua_State *state )
{
	std::string spaces( "\n  " );
	std::ostringstream stream;

	if( lua_type( state, 1 ) == LUA_TSTRING )
		stream << lua_tostring( state, 1 );

	lua_Debug dbg = { 0 };
	for(
		int32_t lvl = 1;
		lua_getstack( state, lvl, &dbg ) == 1;
		++lvl, std::memset( &dbg, 0, sizeof( dbg ) )
	)
	{
		if( lua_getinfo( state, "Sln", &dbg ) == 0 )
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

	lua_pushstring( state, stream.str( ).c_str( ) );
	return 1;
}

}
