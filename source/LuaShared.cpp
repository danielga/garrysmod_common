#include <functional>
#include <cstdarg>
#include <lua.hpp>
#include <Platform.hpp>

#if defined SYSTEM_WINDOWS

#include <Windows.h>

#elif defined SYSTEM_POSIX

#include <dlfcn.h>

#ifdef SYSTEM_MACOSX_BAD

#include <stdexcept>

#endif

#endif

template<typename FunctionType>
static FunctionType GetSymbol( const char *name )
{

#if defined SYSTEM_WINDOWS

	HMODULE binary = nullptr;
	if( GetModuleHandleEx( 0, "garrysmod/bin/lua_shared.dll", &binary ) && binary != nullptr )
	{
		FunctionType symbol_pointer = reinterpret_cast<FunctionType>( GetProcAddress( reinterpret_cast<HMODULE>( binary ), name ) );
		FreeModule( binary );
		return symbol_pointer;
	}

#elif defined SYSTEM_POSIX

#if defined SYSTEM_MACOSX

	void *binary = dlopen( "garrysmod/bin/lua_shared.dylib", RTLD_LAZY | RTLD_NOLOAD );

#else

	void *binary = dlopen( "garrysmod/bin/lua_shared_srv.so", RTLD_LAZY | RTLD_NOLOAD );
	if( binary == nullptr )
		binary = dlopen( "garrysmod/bin/lua_shared.so", RTLD_LAZY | RTLD_NOLOAD );

#endif

	if( binary != nullptr )
	{
		FunctionType symbol_pointer = reinterpret_cast<FunctionType>( dlsym( binary, name ) );
		dlclose( binary );
		return symbol_pointer;
	}

#endif

	return nullptr;
}

template<const char *Name, typename FunctionType>
struct Loader;

template<typename Return, const char *Name, typename ...Args>
struct Loader<Name, Return( Args... )>
{
	typedef Return ( *FunctionType )( Args... );

	template<FunctionType *Target>
	static Return Function( Args... args )
	{
		FunctionType original = GetSymbol<FunctionType>( Name );
		if( original != nullptr )
		{
			*Target = original;

#ifndef SYSTEM_MACOSX_BAD

			return original( std::forward<Args>( args )... );

#else

			return original( args... );

#endif

		}

#ifndef SYSTEM_MACOSX_BAD

		throw std::bad_function_call( );

#else

		throw std::runtime_error( "unable to find function" );

#endif

	}
};

static const char *lua_pushfstring_loader( lua_State *L, const char *fmt, ... )
{
	typedef const char *( *lua_pushfstring_type )( lua_State *, const char *, ... );

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

#ifndef SYSTEM_MACOSX_BAD
	
	throw std::bad_function_call( );
	
#else

	throw std::runtime_error( "unable to find function" );

#endif

}

static int luaL_error_loader( lua_State *L, const char *fmt, ... )
{
	typedef int ( *luaL_error_type )( lua_State *, const char *, ... );

	luaL_error_type original = GetSymbol<luaL_error_type>( "luaL_error" );
	if( original != nullptr )
	{
		luaL_error = original;

		va_list vargs;
		va_start( vargs, fmt );
		lua_pushvfstring( L, fmt, vargs );
		va_end( vargs );
		return lua_error( L );
	}

#ifndef SYSTEM_MACOSX_BAD
	
	throw std::bad_function_call( );
	
#else

	throw std::runtime_error( "unable to find function" );

#endif

}

#define CreateLoader( ret, name, args ) \
	char __ ## name ## _name__[] = #name; \
	ret ( *name ) args = Loader<__ ## name ## _name__, ret args>::Function<&name>

extern "C"
{
	CreateLoader( void , lua_getfenv, ( lua_State *, int ) );
	CreateLoader( int , lua_setfenv, ( lua_State *, int ) );
	CreateLoader( const char *, lua_pushvfstring, ( lua_State *, const char *, va_list ) );
	CreateLoader( int , lua_error, ( lua_State * ) );
	CreateLoader( lua_State *, lua_newstate, ( lua_Alloc, void * ) );
	CreateLoader( void , lua_close, ( lua_State * ) );
	CreateLoader( lua_State *, lua_newthread, ( lua_State * ) );
	CreateLoader( lua_CFunction , lua_atpanic, ( lua_State *, lua_CFunction ) );
	CreateLoader( int , lua_gettop, ( lua_State * ) );
	CreateLoader( void , lua_settop, ( lua_State *, int ) );
	CreateLoader( void , lua_pushvalue, ( lua_State *, int ) );
	CreateLoader( void , lua_remove, ( lua_State *, int ) );
	CreateLoader( void , lua_insert, ( lua_State *, int ) );
	CreateLoader( void , lua_replace, ( lua_State *, int ) );
	CreateLoader( int , lua_checkstack, ( lua_State *, int ) );
	CreateLoader( void , lua_xmove, ( lua_State *, lua_State *, int ) );
	CreateLoader( int , lua_isnumber, ( lua_State *, int ) );
	CreateLoader( int , lua_isstring, ( lua_State *, int ) );
	CreateLoader( int , lua_iscfunction, ( lua_State *, int ) );
	CreateLoader( int , lua_isuserdata, ( lua_State *, int ) );
	CreateLoader( int , lua_type, ( lua_State *, int ) );
	CreateLoader( const char *, lua_typename, ( lua_State *, int ) );
	CreateLoader( int , lua_equal, ( lua_State *, int, int ) );
	CreateLoader( int , lua_rawequal, ( lua_State *, int, int ) );
	CreateLoader( int , lua_lessthan, ( lua_State *, int, int ) );
	CreateLoader( lua_Number , lua_tonumber, ( lua_State *, int ) );
	CreateLoader( lua_Integer , lua_tointeger, ( lua_State *, int ) );
	CreateLoader( int , lua_toboolean, ( lua_State *, int ) );
	CreateLoader( const char *, lua_tolstring, ( lua_State *, int, size_t * ) );
	CreateLoader( size_t , lua_objlen, ( lua_State *, int ) );
	CreateLoader( lua_CFunction , lua_tocfunction, ( lua_State *, int ) );
	CreateLoader( void *, lua_touserdata, ( lua_State *, int ) );
	CreateLoader( lua_State *, lua_tothread, ( lua_State *, int ) );
	CreateLoader( const void *, lua_topointer, ( lua_State *, int ) );
	CreateLoader( void , lua_pushnil, ( lua_State * ) );
	CreateLoader( void , lua_pushnumber, ( lua_State *, lua_Number ) );
	CreateLoader( void , lua_pushinteger, ( lua_State *, lua_Integer ) );
	CreateLoader( void , lua_pushlstring, ( lua_State *, const char *, size_t ) );
	CreateLoader( void , lua_pushstring, ( lua_State *, const char * ) );
	CreateLoader( void , lua_pushcclosure, ( lua_State *, lua_CFunction, int ) );
	CreateLoader( void , lua_pushboolean, ( lua_State *, int ) );
	CreateLoader( void , lua_pushlightuserdata, ( lua_State *, void * ) );
	CreateLoader( int , lua_pushthread, ( lua_State * ) );
	CreateLoader( void , lua_gettable, ( lua_State *, int ) );
	CreateLoader( void , lua_getfield, ( lua_State *, int, const char * ) );
	CreateLoader( void , lua_rawget, ( lua_State *, int ) );
	CreateLoader( void , lua_rawgeti, ( lua_State *, int, int ) );
	CreateLoader( void , lua_createtable, ( lua_State *, int, int ) );
	CreateLoader( void *, lua_newuserdata, ( lua_State *, size_t ) );
	CreateLoader( int , lua_getmetatable, ( lua_State *, int ) );
	CreateLoader( void , lua_settable, ( lua_State *, int ) );
	CreateLoader( void , lua_setfield, ( lua_State *, int, const char * ) );
	CreateLoader( void , lua_rawset, ( lua_State *, int ) );
	CreateLoader( void , lua_rawseti, ( lua_State *, int, int ) );
	CreateLoader( int , lua_setmetatable, ( lua_State *, int ) );
	CreateLoader( void , lua_call, ( lua_State *, int, int ) );
	CreateLoader( int , lua_pcall, ( lua_State *, int, int, int ) );
	CreateLoader( int , lua_cpcall, ( lua_State *, lua_CFunction, void * ) );
	CreateLoader( int , lua_load, ( lua_State *, lua_Reader, void *, const char * ) );
	CreateLoader( int , lua_dump, ( lua_State *, lua_Writer, void * ) );
	CreateLoader( int , lua_resume_real, ( lua_State *, int ) );
	CreateLoader( int , lua_yield, ( lua_State *, int ) );
	CreateLoader( int , lua_resume, ( lua_State *, int ) );
	CreateLoader( int , lua_status, ( lua_State * ) );
	CreateLoader( int , lua_gc, ( lua_State *, int, int ) );
	CreateLoader( int , lua_next, ( lua_State *, int ) );
	CreateLoader( void , lua_concat, ( lua_State *, int ) );
	CreateLoader( lua_Alloc , lua_getallocf, ( lua_State *, void ** ) );
	CreateLoader( void , lua_setallocf, ( lua_State *, lua_Alloc, void * ) );
	CreateLoader( void , lua_setlevel, ( lua_State *, lua_State * ) );
	CreateLoader( int , lua_getstack, ( lua_State *, int , lua_Debug * ) );
	CreateLoader( int , lua_getinfo, ( lua_State *, const char *, lua_Debug * ) );
	CreateLoader( const char *, lua_getlocal, ( lua_State *, const lua_Debug *, int ) );
	CreateLoader( const char *, lua_setlocal, ( lua_State *, const lua_Debug *, int ) );
	CreateLoader( const char *, lua_getupvalue, ( lua_State *, int, int ) );
	CreateLoader( const char *, lua_setupvalue, ( lua_State *, int, int ) );
	CreateLoader( int , lua_sethook, ( lua_State *, lua_Hook, int, int ) );
	CreateLoader( lua_Hook , lua_gethook, ( lua_State * ) );
	CreateLoader( int , lua_gethookmask, ( lua_State * ) );
	CreateLoader( int , lua_gethookcount, ( lua_State * ) );
	CreateLoader( void *, lua_upvalueid, ( lua_State *, int, int ) );
	CreateLoader( void , lua_upvaluejoin, ( lua_State *, int, int, int, int ) );
	CreateLoader( int , lua_loadx, ( lua_State *, lua_Reader, void *, const char *, const char * ) );

	CreateLoader( int , luaL_typerror, ( lua_State *, int, const char * ) );
	CreateLoader( void , luaL_openlib, ( lua_State *, const char *, const luaL_Reg *, int ) );
	CreateLoader( void , luaL_register, ( lua_State *, const char *, const luaL_Reg * ) );
	CreateLoader( int , luaL_getmetafield, ( lua_State *, int, const char * ) );
	CreateLoader( int , luaL_callmeta, ( lua_State *, int, const char * ) );
	CreateLoader( int , luaL_argerror, ( lua_State *, int, const char * ) );
	CreateLoader( const char *, luaL_checklstring, ( lua_State *, int, size_t * ) );
	CreateLoader( const char *, luaL_optlstring, ( lua_State *, int, const char *, size_t * ) );
	CreateLoader( lua_Number , luaL_checknumber, ( lua_State *, int ) );
	CreateLoader( lua_Number , luaL_optnumber, ( lua_State *, int, lua_Number ) );
	CreateLoader( lua_Integer , luaL_checkinteger, ( lua_State *, int ) );
	CreateLoader( lua_Integer , luaL_optinteger, ( lua_State *, int, lua_Integer ) );
	CreateLoader( void , luaL_checkstack, ( lua_State *, int, const char * ) );
	CreateLoader( void , luaL_checktype, ( lua_State *, int, int ) );
	CreateLoader( void , luaL_checkany, ( lua_State *, int ) );
	CreateLoader( int , luaL_newmetatable_type, ( lua_State *, const char *, int ) );
	CreateLoader( int , luaL_newmetatable, ( lua_State *, const char * ) );
	CreateLoader( void *, luaL_checkudata, ( lua_State *, int, const char * ) );
	CreateLoader( void , luaL_where, ( lua_State *, int ) );
	CreateLoader( int , luaL_checkoption, ( lua_State *, int, const char *, const char *const[] ) );
	CreateLoader( int , luaL_ref, ( lua_State *, int ) );
	CreateLoader( void , luaL_unref, ( lua_State *, int, int ) );
	CreateLoader( int , luaL_loadfile, ( lua_State *, const char * ) );
	CreateLoader( int , luaL_loadbuffer, ( lua_State *, const char *, size_t, const char * ) );
	CreateLoader( int , luaL_loadstring, ( lua_State *, const char * ) );
	CreateLoader( lua_State *, luaL_newstate, ( ) );
	CreateLoader( const char *, luaL_gsub, ( lua_State *, const char *, const char *, const char * ) );
	CreateLoader( const char *, luaL_findtable, ( lua_State *, int, const char *, int ) );
	CreateLoader( int , luaL_fileresult, ( lua_State *, int, const char * ) );
	CreateLoader( int , luaL_execresult, ( lua_State *, int ) );
	CreateLoader( int , luaL_loadfilex, ( lua_State *, const char *filename, const char *mode ) );
	CreateLoader( int , luaL_loadbufferx, ( lua_State *, const char *buff, size_t sz, const char *name, const char *mode ) );
	CreateLoader( void , luaL_traceback, ( lua_State *, lua_State *, const char *msg, int level ) );
	CreateLoader( void , luaL_buffinit, ( lua_State *, luaL_Buffer * ) );
	CreateLoader( char *, luaL_prepbuffer, ( luaL_Buffer * ) );
	CreateLoader( void , luaL_addlstring, ( luaL_Buffer *, const char *, size_t ) );
	CreateLoader( void , luaL_addstring, ( luaL_Buffer *, const char * ) );
	CreateLoader( void , luaL_addvalue, ( luaL_Buffer * ) );
	CreateLoader( void , luaL_pushresult, ( luaL_Buffer * ) );

	const char *( *lua_pushfstring )( lua_State *, const char *, ... ) = lua_pushfstring_loader;
	int ( *luaL_error )( lua_State *, const char *, ... ) = luaL_error_loader;
}
