#include <functional>
#include <cstdarg>
#include <lua.hpp>
#include "typestring.hh"

#if defined _WIN32

#include <Windows.h>

#elif defined __linux || defined __APPLE__

#include <dlfcn.h>

#endif

template<typename FunctionType>
static FunctionType GetSymbol( const char *name )
{

#if defined _WIN32

	HMODULE binary = nullptr;
	if( GetModuleHandleEx( 0, "garrysmod/bin/lua_shared.dll", &binary ) && binary != nullptr )
	{
		FunctionType symbol_pointer = reinterpret_cast<FunctionType>( GetProcAddress( reinterpret_cast<HMODULE>( binary ), name ) );
		FreeModule( binary );
		return symbol_pointer;
	}

#elif defined __linux || defined __APPLE__

#if defined __APPLE__

	void *binary = dlopen( "garrysmod/bin/lua_shared.dylib", RTLD_LAZY | RTLD_NOLOAD );

#else

	void *binary = dlopen( "garrysmod/bin/lua_shared_srv.so", RTLD_LAZY | RTLD_NOLOAD );
	if( binary == nullptr )
		binary = dlopen( "garrysmod/bin/lua_shared.so", RTLD_LAZY | RTLD_NOLOAD );

#endif

	if( binary != nullptr)
	{
		FunctionType symbol_pointer = reinterpret_cast<FunctionType>( dlsym( binary, name ) );
		dlclose( binary );
		return symbol_pointer;
	}

#endif

	return nullptr;
}

template<typename Name, typename FunctionType>
struct Loader;

template<typename Name, typename Return, typename ...Args>
struct Loader<Name, Return( Args... )>
{
	typedef Return ( *FunctionType )( Args... );

	template<FunctionType *Target>
	static Return Function( Args... args )
	{
		FunctionType original = GetSymbol<FunctionType>( Name::data( ) );
		if( original != nullptr )
		{
			*Target = original;
			return original( std::forward<Args>( args )... );
		}

		throw std::bad_function_call( );
	}
};

static const char *lua_pushfstring_loader( lua_State *L, const char *fmt, ... )
{
	typedef const char *( *lua_pushfstring_type )( lua_State *L, const char *fmt, ... );

	lua_pushfstring_type original = GetSymbol<lua_pushfstring_type>( "lua_pushfstring" );
	if( original != nullptr )
	{
		lua_pushfstring = original;

		va_list vargs;
		va_start( vargs, fmt );
		const char *res = lua_pushvfstring( L, fmt, vargs );
		va_end( vargs );
		return res;
	}

	throw std::bad_function_call( );
}

static int luaL_error_loader( lua_State *L, const char *fmt, ... )
{
	typedef int ( *luaL_error_type )( lua_State *L, const char *fmt, ... );

	luaL_error_type original = GetSymbol<luaL_error_type>( "luaL_error" );
	if( original != nullptr )
	{
		luaL_error = original;

		va_list vargs;
		va_start( vargs, fmt );
		const char *res = lua_pushvfstring( L, fmt, vargs );
		va_end( vargs );
		return lua_error( L );
	}

	throw std::bad_function_call( );
}

extern "C"
{
	void ( *lua_getfenv )( lua_State *, int ) = Loader<typestring_is( "lua_getfenv" ), void ( lua_State *, int )>::Function<&lua_getfenv>;
	int ( *lua_setfenv )( lua_State *, int ) = Loader<typestring_is( "lua_setfenv" ), int ( lua_State *, int )>::Function<&lua_setfenv>;
	const char *( *lua_pushvfstring )( lua_State *, const char *, va_list ) = Loader<typestring_is( "lua_pushvfstring" ), const char *( lua_State *, const char *, va_list )>::Function<&lua_pushvfstring>;
	int ( *luaL_typerror )( lua_State *, int, const char * ) = Loader<typestring_is( "luaL_typerror" ), int ( lua_State *, int, const char * )>::Function<&luaL_typerror>;
	int ( *lua_error )( lua_State * ) = Loader<typestring_is( "lua_error" ), int ( lua_State * )>::Function<&lua_error>;

	const char *( *lua_pushfstring )( lua_State *, const char *, ... ) = lua_pushfstring_loader;
	int ( *luaL_error )( lua_State *, const char *, ... ) = luaL_error_loader;
}
