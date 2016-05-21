#pragma once

#include "Types.h"
#include "LuaBase.h"
#include "UserData.h"

struct lua_State
{
	unsigned char _ignore_this_common_lua_header_[69];
	GarrysMod::Lua::ILuaBase *luabase;
};

#if defined _WIN32

	#define GMOD_DLL_EXPORT extern "C" __declspec(dllexport)

#else

	#define GMOD_DLL_EXPORT extern "C" __attribute__((visibility("default")))

#endif

#define GMOD_MODULE_OPEN( ) GMOD_DLL_EXPORT int gmod13_open( lua_State *state )
#define GMOD_MODULE_CLOSE( ) GMOD_DLL_EXPORT int gmod13_close( lua_State *state )

#define LUA state->luabase

#define LUA_FUNCTION( name ) int name( lua_State *state )
#define LUA_FUNCTION_STATIC( name ) static LUA_FUNCTION( name )
