#ifndef GARRYSMOD_LUA_INTERFACE_H
#define GARRYSMOD_LUA_INTERFACE_H

#ifdef GMOD_USE_ILUAINTERFACE
#include "LuaInterface.h"
#define GMOD_LUACLASS GarrysMod::Lua::ILuaInterface*
#else
#include "LuaBase.h"
#define GMOD_LUACLASS GarrysMod::Lua::ILuaBase*
#endif

struct lua_State
{
#if ( defined( _WIN32 ) || defined( __linux__ ) || defined( __APPLE__ ) ) && \
    !defined( __x86_64__ ) && !defined( _M_X64 )
    // Win32, Linux32 and macOS32
    unsigned char _ignore_this_common_lua_header_[48 + 22];
#elif ( defined( _WIN32 ) || defined( __linux__ ) || defined( __APPLE__ ) ) && \
    ( defined( __x86_64__ ) || defined( _M_X64 ) )
    // Win64, Linux64 and macOS64 (not tested)
    unsigned char _ignore_this_common_lua_header_[92 + 22];
#else
    #error Unsupported platform
#endif

    GMOD_LUACLASS luabase;
};

#ifndef GMOD
    #ifdef _WIN32
        #define GMOD_DLL_EXPORT extern "C" __declspec( dllexport )
    #else
        #define GMOD_DLL_EXPORT extern "C" __attribute__((visibility("default")))
    #endif

    #ifdef GMOD_ALLOW_DEPRECATED
        // Stop using this and use LUA_FUNCTION!
        #define LUA ( state->luabase )

        #define GMOD_MODULE_OPEN() GMOD_DLL_EXPORT int gmod13_open( [[maybe_unused]] lua_State* state )
        #define GMOD_MODULE_CLOSE() GMOD_DLL_EXPORT int gmod13_close( [[maybe_unused]] lua_State* state )

        #define LUA_FUNCTION( name ) int name( [[maybe_unused]] lua_State *state )
        #define LUA_FUNCTION_STATIC( name ) static LUA_FUNCTION( name )
    #else
        #define GMOD_MODULE_OPEN()                                  \
            int gmod13_open__Imp( GMOD_LUACLASS LUA );  \
            GMOD_DLL_EXPORT int gmod13_open( lua_State* L )         \
            {                                                       \
                return gmod13_open__Imp( L->luabase );              \
            }                                                       \
            int gmod13_open__Imp( [[maybe_unused]] GMOD_LUACLASS LUA )

        #define GMOD_MODULE_CLOSE()                                 \
            int gmod13_close__Imp( GMOD_LUACLASS LUA ); \
            GMOD_DLL_EXPORT int gmod13_close( lua_State* L )        \
            {                                                       \
                return gmod13_close__Imp( L->luabase );             \
            }                                                       \
            int gmod13_close__Imp( [[maybe_unused]] GMOD_LUACLASS LUA )

        #define LUA_FUNCTION( FUNC )                            \
            int FUNC##__Imp( GMOD_LUACLASS LUA );   \
            int FUNC( lua_State* L )                            \
            {                                                   \
                GMOD_LUACLASS LUA = L->luabase;     \
                LUA->SetState(L);                               \
                return FUNC##__Imp( LUA );                      \
            }                                                   \
            int FUNC##__Imp( [[maybe_unused]] GMOD_LUACLASS LUA )

        #define LUA_FUNCTION_STATIC( FUNC )                             \
            static int FUNC##__Imp( GMOD_LUACLASS LUA );    \
            static int FUNC( lua_State* L )                             \
            {                                                           \
                GMOD_LUACLASS LUA = L->luabase;             \
                LUA->SetState(L);                                       \
                return FUNC##__Imp( LUA );                              \
            }                                                           \
            static int FUNC##__Imp( [[maybe_unused]] GMOD_LUACLASS LUA )

        #define LUA_FUNCTION_DECLARE( FUNC ) \
            int FUNC( lua_State *L )

        #define LUA_FUNCTION_STATIC_DECLARE( FUNC ) \
            static int FUNC( lua_State *L )

        #define LUA_FUNCTION_IMPLEMENT( FUNC )                                                  \
            [[deprecated("Use LUA_FUNCTION_STATIC_MEMBER instead of LUA_FUNCTION_IMPLEMENT.")]] \
            static int FUNC##__Imp( [[maybe_unused]] GMOD_LUACLASS LUA )

        #define LUA_FUNCTION_WRAP( FUNC )                                                   \
            [[deprecated("Use LUA_FUNCTION_STATIC_MEMBER instead of LUA_FUNCTION_WRAP.")]]  \
            static int FUNC( lua_State *L )                                                 \
            {                                                                               \
                GMOD_LUACLASS LUA = L->luabase;                                 \
                LUA->SetState(L);                                                           \
                return FUNC##__Imp( LUA );                                                  \
            }

        #define LUA_FUNCTION_STATIC_MEMBER( FUNC )                  \
            static int FUNC( lua_State* L )                         \
            {                                                       \
                GMOD_LUACLASS LUA = L->luabase;         \
                LUA->SetState(L);                                   \
                return FUNC##__Imp( LUA );                          \
            }                                                       \
            static int FUNC##__Imp( GMOD_LUACLASS LUA )
    #endif
#endif

#endif
