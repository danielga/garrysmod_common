#ifndef GARRYSMOD_LUA_INTERFACE_H
#define GARRYSMOD_LUA_INTERFACE_H

#include "LuaBase.h"

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

    GarrysMod::Lua::ILuaBase* luabase;
};

#ifndef GMOD
    #ifdef _WIN32
        #define GMOD_DLL_EXPORT extern "C" __declspec( dllexport )
    #else
        #define GMOD_DLL_EXPORT extern "C" __attribute__((visibility("default")))
    #endif

    #if !defined( _MSC_VER ) || _MSC_VER >= 1900
        #define GMOD_NOEXCEPT noexcept
    #elif _MSC_VER >= 1700
        #include <yvals.h>
        #define GMOD_NOEXCEPT _NOEXCEPT
    #else
        #define GMOD_NOEXCEPT
    #endif

    #ifdef GMOD_ALLOW_DEPRECATED
        // Stop using this and use LUA_FUNCTION!
        #define LUA ( state->luabase )

        #define GMOD_MODULE_OPEN()  GMOD_DLL_EXPORT int gmod13_open( lua_State* state )
        #define GMOD_MODULE_CLOSE() GMOD_DLL_EXPORT int gmod13_close( lua_State* state )

        #define LUA_FUNCTION( name ) int name( lua_State *state ) GMOD_NOEXCEPT
        #define LUA_FUNCTION_STATIC( name ) static LUA_FUNCTION( name )
    #else
        #define GMOD_MODULE_OPEN()                                               \
            int gmod13_open__Imp( GarrysMod::Lua::ILuaBase* LUA ) GMOD_NOEXCEPT; \
            GMOD_DLL_EXPORT int gmod13_open( lua_State* L ) GMOD_NOEXCEPT        \
            {                                                                    \
                return gmod13_open__Imp( L->luabase );                           \
            }                                                                    \
            int gmod13_open__Imp( GarrysMod::Lua::ILuaBase* LUA ) GMOD_NOEXCEPT

        #define GMOD_MODULE_CLOSE()                                               \
            int gmod13_close__Imp( GarrysMod::Lua::ILuaBase* LUA ) GMOD_NOEXCEPT; \
            GMOD_DLL_EXPORT int gmod13_close( lua_State* L ) GMOD_NOEXCEPT        \
            {                                                                     \
                return gmod13_close__Imp( L->luabase );                           \
            }                                                                     \
            int gmod13_close__Imp( GarrysMod::Lua::ILuaBase* LUA ) GMOD_NOEXCEPT

        #define LUA_FUNCTION( FUNC )                                        \
            int FUNC##__Imp( GarrysMod::Lua::ILuaBase* LUA ) GMOD_NOEXCEPT; \
            int FUNC( lua_State* L ) GMOD_NOEXCEPT                          \
            {                                                               \
                GarrysMod::Lua::ILuaBase* LUA = L->luabase;                 \
                LUA->SetState(L);                                           \
                return FUNC##__Imp( LUA );                                  \
            }                                                               \
            int FUNC##__Imp( GarrysMod::Lua::ILuaBase* LUA ) GMOD_NOEXCEPT

        #define LUA_FUNCTION_STATIC( FUNC )                                        \
            static int FUNC##__Imp( GarrysMod::Lua::ILuaBase* LUA ) GMOD_NOEXCEPT; \
            static int FUNC( lua_State* L ) GMOD_NOEXCEPT                          \
            {                                                                      \
                GarrysMod::Lua::ILuaBase* LUA = L->luabase;                        \
                LUA->SetState(L);                                                  \
                return FUNC##__Imp( LUA );                                         \
            }                                                                      \
            static int FUNC##__Imp( GarrysMod::Lua::ILuaBase* LUA ) GMOD_NOEXCEPT

        #define LUA_FUNCTION_DECLARE( FUNC ) \
            int FUNC( lua_State *L ) GMOD_NOEXCEPT

        #define LUA_FUNCTION_STATIC_DECLARE( FUNC ) \
            static int FUNC( lua_State *L ) GMOD_NOEXCEPT

        #define LUA_FUNCTION_IMPLEMENT( FUNC ) \
            static int FUNC##__Imp( GarrysMod::Lua::ILuaBase* LUA ) GMOD_NOEXCEPT

        #define LUA_FUNCTION_WRAP( FUNC )                   \
            LUA_FUNCTION_STATIC_DECLARE( FUNC )             \
            {                                               \
                GarrysMod::Lua::ILuaBase* LUA = L->luabase; \
                LUA->SetState(L);                           \
                return FUNC##__Imp( LUA );                  \
            }
    #endif
#endif

#endif
