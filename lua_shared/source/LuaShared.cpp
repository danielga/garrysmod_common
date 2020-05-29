#include <functional>
#include <cstdarg>
#include <cstdlib>

#include "lua.hpp"
#include "Platform.hpp"

#if defined SYSTEM_WINDOWS

#include <Windows.h>

#elif defined SYSTEM_POSIX

#include <dlfcn.h>

#endif

#if defined SYSTEM_WINDOWS

template<typename FunctionType>
static FunctionType GetSymbol( const char *name )
{
	HMODULE binary = nullptr;

#if defined ARCHITECTURE_X86

	if( !GetModuleHandleEx( 0, "garrysmod/bin/lua_shared.dll", &binary ) &&
		!GetModuleHandleEx( 0, "bin/lua_shared.dll", &binary ) )
		return nullptr;

#elif defined ARCHITECTURE_X86_64

	if( !GetModuleHandleEx( 0, "bin/win64/lua_shared.dll", &binary ) )
		return nullptr;

#endif

	FunctionType symbol_pointer = reinterpret_cast<FunctionType>( GetProcAddress( binary, name ) );
	FreeLibrary( binary );
	return symbol_pointer;
}

#elif defined SYSTEM_POSIX

template<typename FunctionType>
static FunctionType GetSymbol( const char *name )
{

#if defined SYSTEM_MACOSX

	void *binary = dlopen( "garrysmod/bin/lua_shared.dylib", RTLD_LAZY | RTLD_NOLOAD );

#elif defined SYSTEM_LINUX && defined ARCHITECTURE_X86

	void *binary = dlopen( "garrysmod/bin/lua_shared_srv.so", RTLD_LAZY | RTLD_NOLOAD );
	if( binary == nullptr )
		binary = dlopen( "garrysmod/bin/lua_shared.so", RTLD_LAZY | RTLD_NOLOAD );

	if( binary == nullptr )
		binary = dlopen( "bin/linux32/lua_shared.so", RTLD_LAZY | RTLD_NOLOAD );

	if( binary == nullptr )
		binary = dlopen( "bin/linux32/lua_shared_client.so", RTLD_LAZY | RTLD_NOLOAD );

#elif defined SYSTEM_LINUX && defined ARCHITECTURE_X86_64

	void *binary = dlopen( "bin/linux64/lua_shared.so", RTLD_LAZY | RTLD_NOLOAD );
	if( binary == nullptr )
		binary = dlopen( "bin/linux64/lua_shared_client.so", RTLD_LAZY | RTLD_NOLOAD );

#endif

	if( binary != nullptr )
	{
		FunctionType symbol_pointer = reinterpret_cast<FunctionType>( dlsym( binary, name ) );
		dlclose( binary );
		return symbol_pointer;
	}

	return nullptr;
}

#endif

#define CreateLoader( Return, Name, Arguments, Parameters )		\
	Return Name Arguments										\
	{															\
		typedef Return ( *Name##_type ) Arguments;				\
		static auto original = GetSymbol<Name##_type>( #Name );	\
		if( original != nullptr )								\
		{														\
			return original Parameters;							\
		}														\
																\
		std::abort( );											\
	}

extern "C"
{
	CreateLoader( void, lua_getfenv, ( lua_State *a, int b ), ( a, b ) );
	CreateLoader( int, lua_setfenv, ( lua_State *a, int b ), ( a, b ) );
	CreateLoader( const char *, lua_pushvfstring, ( lua_State *a, const char *b, va_list c ), ( a, b, c ) );
	CreateLoader( int, lua_error, ( lua_State *a ), ( a ) );
	CreateLoader( lua_State *, lua_newstate, ( lua_Alloc a, void *b ), ( a, b ) );
	CreateLoader( void, lua_close, ( lua_State *a ), ( a ) );
	CreateLoader( lua_State *, lua_newthread, ( lua_State *a ), ( a ) );
	CreateLoader( lua_CFunction, lua_atpanic, ( lua_State *a, lua_CFunction b ), ( a, b ) );
	CreateLoader( int, lua_gettop, ( lua_State *a ), ( a ) );
	CreateLoader( void, lua_settop, ( lua_State *a, int b ), ( a, b ) );
	CreateLoader( void, lua_pushvalue, ( lua_State *a, int b ), ( a, b ) );
	CreateLoader( void, lua_remove, ( lua_State *a, int b ), ( a, b ) );
	CreateLoader( void, lua_insert, ( lua_State *a, int b ), ( a, b ) );
	CreateLoader( void, lua_replace, ( lua_State *a, int b ), ( a, b ) );
	CreateLoader( int, lua_checkstack, ( lua_State *a, int b ), ( a, b ) );
	CreateLoader( void, lua_xmove, ( lua_State *a, lua_State *b, int c ), ( a, b, c ) );
	CreateLoader( int, lua_isnumber, ( lua_State *a, int b ), ( a, b ) );
	CreateLoader( int, lua_isstring, ( lua_State *a, int b ), ( a, b ) );
	CreateLoader( int, lua_iscfunction, ( lua_State *a, int b ), ( a, b ) );
	CreateLoader( int, lua_isuserdata, ( lua_State *a, int b ), ( a, b ) );
	CreateLoader( int, lua_type, ( lua_State *a, int b ), ( a, b ) );
	CreateLoader( const char *, lua_typename, ( lua_State *a, int b, int c ), ( a, b, c ) );
	CreateLoader( int, lua_equal, ( lua_State *a, int b, int c ), ( a, b, c ) );
	CreateLoader( int, lua_rawequal, ( lua_State *a, int b, int c ), ( a, b, c ) );
	CreateLoader( int, lua_lessthan, ( lua_State *a, int b, int c ), ( a, b, c ) );
	CreateLoader( lua_Number, lua_tonumber, ( lua_State *a, int b ), ( a, b ) );
	CreateLoader( lua_Integer, lua_tointeger, ( lua_State *a, int b ), ( a, b ) );
	CreateLoader( int, lua_toboolean, ( lua_State *a, int b ), ( a, b ) );
	CreateLoader( const char *, lua_tolstring, ( lua_State *a, int b, size_t *c ), ( a, b, c ) );
	CreateLoader( size_t, lua_objlen, ( lua_State *a, int b ), ( a, b ) );
	CreateLoader( lua_CFunction, lua_tocfunction, ( lua_State *a, int b ), ( a, b ) );
	CreateLoader( void *, lua_touserdata, ( lua_State *a, int b ), ( a, b ) );
	CreateLoader( lua_State *, lua_tothread, ( lua_State *a, int b ), ( a, b ) );
	CreateLoader( const void *, lua_topointer, ( lua_State *a, int b ), ( a, b ) );
	CreateLoader( void, lua_pushnil, ( lua_State *a ), ( a ) );
	CreateLoader( void, lua_pushnumber, ( lua_State *a, lua_Number b ), ( a, b ) );
	CreateLoader( void, lua_pushinteger, ( lua_State *a, lua_Integer b ), ( a, b ) );
	CreateLoader( void, lua_pushlstring, ( lua_State *a, const char *b, size_t c ), ( a, b, c ) );
	CreateLoader( void, lua_pushstring, ( lua_State *a, const char *b ), ( a, b ) );
	CreateLoader( void, lua_pushcclosure, ( lua_State *a, lua_CFunction b, int c ), ( a, b, c ) );
	CreateLoader( void, lua_pushboolean, ( lua_State *a, int b ), ( a, b ) );
	CreateLoader( void, lua_pushlightuserdata, ( lua_State *a, void *b ), ( a, b ) );
	CreateLoader( int, lua_pushthread, ( lua_State *a ), ( a ) );
	CreateLoader( void, lua_gettable, ( lua_State *a, int b ), ( a, b ) );
	CreateLoader( void, lua_getfield, ( lua_State *a, int b, const char *c ), ( a, b, c ) );
	CreateLoader( void, lua_rawget, ( lua_State *a, int b ), ( a, b ) );
	CreateLoader( void, lua_rawgeti, ( lua_State *a, int b, int c ), ( a, b, c ) );
	CreateLoader( void, lua_createtable, ( lua_State *a, int b, int c ), ( a, b, c ) );
	CreateLoader( void *, lua_newuserdata, ( lua_State *a, size_t b ), ( a, b ) );
	CreateLoader( int, lua_getmetatable, ( lua_State *a, int b ), ( a, b ) );
	CreateLoader( void, lua_settable, ( lua_State *a, int b ), ( a, b ) );
	CreateLoader( void, lua_setfield, ( lua_State *a, int b, const char *c ), ( a, b, c ) );
	CreateLoader( void, lua_rawset, ( lua_State *a, int b ), ( a, b ) );
	CreateLoader( void, lua_rawseti, ( lua_State *a, int b, int c ), ( a, b, c ) );
	CreateLoader( int, lua_setmetatable, ( lua_State *a, int b ), ( a, b ) );
	CreateLoader( void, lua_call, ( lua_State *a, int b, int c ), ( a, b, c ) );
	CreateLoader( int, lua_pcall, ( lua_State *a, int b, int c, int d ), ( a, b, c, d ) );
	CreateLoader( int, lua_cpcall, ( lua_State *a, lua_CFunction b, void *c ), ( a, b, c ) );
	CreateLoader( int, lua_load, ( lua_State *a, lua_Reader b, void *c, const char *d ), ( a, b, c, d ) );
	CreateLoader( int, lua_dump, ( lua_State *a, lua_Writer b, void *c ), ( a, b, c ) );
	CreateLoader( int, lua_resume_real, ( lua_State *a, int b ), ( a, b ) );
	CreateLoader( int, lua_yield, ( lua_State *a, int b ), ( a, b ) );
	CreateLoader( int, lua_status, ( lua_State *a ), ( a ) );
	CreateLoader( int, lua_gc, ( lua_State *a, int b, int c ), ( a, b, c ) );
	CreateLoader( int, lua_next, ( lua_State *a, int b ), ( a, b ) );
	CreateLoader( void, lua_concat, ( lua_State *a, int b ), ( a, b ) );
	CreateLoader( lua_Alloc, lua_getallocf, ( lua_State *a, void **b ), ( a, b ) );
	CreateLoader( void, lua_setallocf, ( lua_State *a, lua_Alloc b, void *c ), ( a, b, c ) );
	CreateLoader( void, lua_setlevel, ( lua_State *a, lua_State *b ), ( a, b ) );
	CreateLoader( int, lua_getstack, ( lua_State *a, int b, lua_Debug *c ), ( a, b, c ) );
	CreateLoader( int, lua_getinfo, ( lua_State *a, const char *b, lua_Debug *c ), ( a, b, c ) );
	CreateLoader( const char *, lua_getlocal, ( lua_State *a, const lua_Debug *b, int c ), ( a, b, c ) );
	CreateLoader( const char *, lua_setlocal, ( lua_State *a, const lua_Debug *b, int c ), ( a, b, c ) );
	CreateLoader( const char *, lua_getupvalue, ( lua_State *a, int b, int c ), ( a, b, c ) );
	CreateLoader( const char *, lua_setupvalue, ( lua_State *a, int b, int c ), ( a, b, c ) );
	CreateLoader( int, lua_sethook, ( lua_State *a, lua_Hook b, int c, int d ), ( a, b, c, d ) );
	CreateLoader( lua_Hook, lua_gethook, ( lua_State *a ), ( a ) );
	CreateLoader( int, lua_gethookmask, ( lua_State *a ), ( a ) );
	CreateLoader( int, lua_gethookcount, ( lua_State *a ), ( a ) );
	CreateLoader( void *, lua_upvalueid, ( lua_State *a, int b, int c ), ( a, b, c ) );
	CreateLoader( void, lua_upvaluejoin, ( lua_State *a, int b, int c, int d, int e ), ( a, b, c, d, e ) );
	CreateLoader( int, lua_loadx, ( lua_State *a, lua_Reader b, void *c, const char *d, const char *e ), ( a, b, c, d, e ) );
	CreateLoader( const lua_Number *, lua_version, ( lua_State *a ), ( a ) );
	CreateLoader( void, lua_copy, ( lua_State *a, int b, int c ), ( a, b, c ) );
	CreateLoader( lua_Number, lua_tonumberx, (lua_State *a, int b, int *c ), ( a, b, c ) );
	CreateLoader( lua_Integer, lua_tointegerx, ( lua_State* a, int b, int *c ), ( a, b, c ) );
	CreateLoader( int, lua_isyieldable, ( lua_State *a ), ( a ) );

	CreateLoader( int, luaL_typerror, ( lua_State *a, int b, const char *c ), ( a, b, c ) );
	CreateLoader( void, luaL_openlib, ( lua_State *a, const char *b, const luaL_Reg *c, int d ), ( a, b, c, d ) );
	CreateLoader( void, luaL_register, ( lua_State *a, const char *b, const luaL_Reg *c ), ( a, b, c ) );
	CreateLoader( int, luaL_getmetafield, ( lua_State *a, int b, const char *c ), ( a, b, c ) );
	CreateLoader( int, luaL_callmeta, ( lua_State *a, int b, const char *c ), ( a, b, c ) );
	CreateLoader( int, luaL_argerror, ( lua_State *a, int b, const char *c ), ( a, b, c ) );
	CreateLoader( const char *, luaL_checklstring, ( lua_State *a, int b, size_t *c ), ( a, b, c ) );
	CreateLoader( const char *, luaL_optlstring, ( lua_State *a, int b, const char *c, size_t *d ), ( a, b, c, d ) );
	CreateLoader( lua_Number, luaL_checknumber, ( lua_State *a, int b ), ( a, b ) );
	CreateLoader( lua_Number, luaL_optnumber, ( lua_State *a, int b, lua_Number c ), ( a, b, c ) );
	CreateLoader( lua_Integer, luaL_checkinteger, ( lua_State *a, int b ), ( a, b ) );
	CreateLoader( lua_Integer, luaL_optinteger, ( lua_State *a, int b, lua_Integer c ), ( a, b, c ) );
	CreateLoader( void, luaL_checkstack, ( lua_State *a, int b, const char *c ), ( a, b, c ) );
	CreateLoader( void, luaL_checktype, ( lua_State *a, int b, int c ), ( a, b, c ) );
	CreateLoader( void, luaL_checkany, ( lua_State *a, int b ), ( a, b ) );
	CreateLoader( int, luaL_newmetatable_type, ( lua_State *a, const char *b, int c ), ( a, b, c ) );
	CreateLoader( int, luaL_newmetatable, ( lua_State *a, const char *b ), ( a, b ) );
	CreateLoader( void *, luaL_checkudata, ( lua_State *a, int b, const char *c ), ( a, b, c ) );
	CreateLoader( void, luaL_where, ( lua_State *a, int b ), ( a, b ) );
	CreateLoader( int, luaL_checkoption, ( lua_State *a, int b, const char *c, const char *const d[] ), ( a, b, c, d ) );
	CreateLoader( int, luaL_ref, ( lua_State *a, int b ), ( a, b ) );
	CreateLoader( void, luaL_unref, ( lua_State *a, int b, int c ), ( a, b, c ) );
	CreateLoader( int, luaL_loadfile, ( lua_State *a, const char *b ), ( a, b ) );
	CreateLoader( int, luaL_loadbuffer, ( lua_State *a, const char *b, size_t c, const char *d ), ( a, b, c, d ) );
	CreateLoader( int, luaL_loadstring, ( lua_State *a, const char *b ), ( a, b ) );
	CreateLoader( lua_State *, luaL_newstate, ( ), ( ) );
	CreateLoader( const char *, luaL_gsub, ( lua_State *a, const char *b, const char *c, const char *d ), ( a, b, c, d ) );
	CreateLoader( const char *, luaL_findtable, ( lua_State *a, int b, const char *c, int d ), ( a, b, c, d ) );
	CreateLoader( int, luaL_fileresult, ( lua_State *a, int b, const char *c ), ( a, b, c ) );
	CreateLoader( int, luaL_execresult, ( lua_State *a, int b ), ( a, b ) );
	CreateLoader( int, luaL_loadfilex, ( lua_State *a, const char *b, const char *c ), ( a, b, c ) );
	CreateLoader( int, luaL_loadbufferx, ( lua_State *a, const char *b, size_t c, const char *d, const char *e ), ( a, b, c, d, e ) );
	CreateLoader( void, luaL_traceback, ( lua_State *a, lua_State *b, const char *c, int d ), ( a, b, c, d ) );
	CreateLoader( void, luaL_setfuncs, ( lua_State *a, const luaL_Reg *b, int c ), ( a, b, c ) );
	CreateLoader( void, luaL_pushmodule, ( lua_State *a, const char *b, int c ), ( a, b, c ) );
	CreateLoader( void *, luaL_testudata, ( lua_State *a, int b, const char *c ), ( a, b, c ) );
	CreateLoader( void, luaL_setmetatable, ( lua_State *a, const char *b ), ( a, b ) );
	CreateLoader( void, luaL_buffinit, ( lua_State *a, luaL_Buffer *b ), ( a, b ) );
	CreateLoader( char *, luaL_prepbuffer, ( luaL_Buffer *a ), ( a ) );
	CreateLoader( void, luaL_addlstring, ( luaL_Buffer *a, const char *b, size_t c ), ( a, b, c ) );
	CreateLoader( void, luaL_addstring, ( luaL_Buffer *a, const char *b ), ( a, b ) );
	CreateLoader( void, luaL_addvalue, ( luaL_Buffer *a ), ( a ) );
	CreateLoader( void, luaL_pushresult, ( luaL_Buffer *a ), ( a ) );

	const char *lua_pushfstring( lua_State *L, const char *fmt, ... )
	{
		typedef const char *( *lua_pushfstring_type )( lua_State *, const char *, ... );

		lua_pushfstring_type original = GetSymbol<lua_pushfstring_type>( "lua_pushfstring" );
		if( original != nullptr )
		{
			va_list vargs;
			va_start( vargs, fmt );
			const char *res = lua_pushvfstring( L, fmt, vargs );
			va_end( vargs );
			return res;
		}

		std::abort( );
	}

	int luaL_error( lua_State *L, const char *fmt, ... )
	{
		typedef int ( *luaL_error_type )( lua_State *, const char *, ... );

		luaL_error_type original = GetSymbol<luaL_error_type>( "luaL_error" );
		if( original != nullptr )
		{
			va_list vargs;
			va_start( vargs, fmt );
			lua_pushvfstring( L, fmt, vargs );
			va_end( vargs );
			return lua_error( L );
		}

		std::abort( );
	}

	CreateLoader( int, luaopen_base, ( lua_State *a ), ( a ) );
	CreateLoader( int, luaopen_math, ( lua_State *a ), ( a ) );
	CreateLoader( int, luaopen_string, ( lua_State *a ), ( a ) );
	CreateLoader( int, luaopen_table, ( lua_State *a ), ( a ) );
	CreateLoader( int, luaopen_os, ( lua_State *a ), ( a ) );
	CreateLoader( int, luaopen_package, ( lua_State *a ), ( a ) );
	CreateLoader( int, luaopen_debug, ( lua_State *a ), ( a ) );
	CreateLoader( int, luaopen_bit, ( lua_State *a ), ( a ) );
	CreateLoader( int, luaopen_jit, ( lua_State *a ), ( a ) );

	CreateLoader( void, luaL_openlibs, ( lua_State *a ), ( a ) );

	CreateLoader( int, luaJIT_setmode, ( lua_State *a, int b, int c ), ( a, b, c ) );
	CreateLoader( void, LUAJIT_VERSION_SYM, ( ), ( ) );
	typedef void ( *luaJIT_profile_callback )( void *data, lua_State *L, int samples, int vmstate );
	CreateLoader( void, luaJIT_profile_start, ( lua_State *a, const char *b, luaJIT_profile_callback c, void *d ), ( a, b, c, d ) );
	CreateLoader( void, luaJIT_profile_stop, ( lua_State *a ), ( a ) );
	CreateLoader( const char *, luaJIT_profile_dumpstack, ( lua_State* a, const char *b, int c, size_t *d ), ( a, b, c, d ) );
}
