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

	if( binary != nullptr )
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
	lua_State *( *lua_newstate )( lua_Alloc f, void *ud ) = Loader<typestring_is( "lua_newstate" ), lua_State *( lua_Alloc f, void *ud )>::Function<&lua_newstate>;
	void ( *lua_close )( lua_State *L ) = Loader<typestring_is( "lua_close" ), void ( lua_State *L )>::Function<&lua_close>;
	lua_State *( *lua_newthread )( lua_State *L ) = Loader<typestring_is( "lua_newthread" ), lua_State *( lua_State *L )>::Function<&lua_newthread>;
	lua_CFunction ( *lua_atpanic )( lua_State *L, lua_CFunction panicf ) = Loader<typestring_is( "lua_atpanic" ), lua_CFunction ( lua_State *L, lua_CFunction panicf )>::Function<&lua_atpanic>;
	int ( *lua_gettop )( lua_State *L ) = Loader<typestring_is( "lua_gettop" ), int ( lua_State *L )>::Function<&lua_gettop>;
	void ( *lua_settop )( lua_State *L, int idx ) = Loader<typestring_is( "lua_settop" ), void ( lua_State *L, int idx )>::Function<&lua_settop>;
	void ( *lua_pushvalue )( lua_State *L, int idx ) = Loader<typestring_is( "lua_pushvalue" ), void ( lua_State *L, int idx )>::Function<&lua_pushvalue>;
	void ( *lua_remove )( lua_State *L, int idx ) = Loader<typestring_is( "lua_remove" ), void ( lua_State *L, int idx )>::Function<&lua_remove>;
	void ( *lua_insert )( lua_State *L, int idx ) = Loader<typestring_is( "lua_insert" ), void ( lua_State *L, int idx )>::Function<&lua_insert>;
	void ( *lua_replace )( lua_State *L, int idx ) = Loader<typestring_is( "lua_replace" ), void ( lua_State *L, int idx )>::Function<&lua_replace>;
	int ( *lua_checkstack )( lua_State *L, int sz ) = Loader<typestring_is( "lua_checkstack" ), int ( lua_State *L, int sz )>::Function<&lua_checkstack>;
	void ( *lua_xmove )( lua_State *from, lua_State *to, int n ) = Loader<typestring_is( "lua_xmove" ), void ( lua_State *from, lua_State *to, int n )>::Function<&lua_xmove>;
	int ( *lua_isnumber )( lua_State *L, int idx ) = Loader<typestring_is( "lua_isnumber" ), int ( lua_State *L, int idx )>::Function<&lua_isnumber>;
	int ( *lua_isstring )( lua_State *L, int idx ) = Loader<typestring_is( "lua_isstring" ), int ( lua_State *L, int idx )>::Function<&lua_isstring>;
	int ( *lua_iscfunction )( lua_State *L, int idx ) = Loader<typestring_is( "lua_iscfunction" ), int ( lua_State *L, int idx )>::Function<&lua_iscfunction>;
	int ( *lua_isuserdata )( lua_State *L, int idx ) = Loader<typestring_is( "lua_isuserdata" ), int ( lua_State *L, int idx )>::Function<&lua_isuserdata>;
	int ( *lua_type )( lua_State *L, int idx ) = Loader<typestring_is( "lua_type" ), int ( lua_State *L, int idx )>::Function<&lua_type>;
	const char *( *lua_typename )( lua_State *L, int tp ) = Loader<typestring_is( "lua_typename" ), const char *( lua_State *L, int tp )>::Function<&lua_typename>;
	int ( *lua_equal )( lua_State *L, int idx1, int idx2 ) = Loader<typestring_is( "lua_equal" ), int ( lua_State *L, int idx1, int idx2 )>::Function<&lua_equal>;
	int ( *lua_rawequal )( lua_State *L, int idx1, int idx2 ) = Loader<typestring_is( "lua_rawequal" ), int ( lua_State *L, int idx1, int idx2 )>::Function<&lua_rawequal>;
	int ( *lua_lessthan )( lua_State *L, int idx1, int idx2 ) = Loader<typestring_is( "lua_lessthan" ), int ( lua_State *L, int idx1, int idx2 )>::Function<&lua_lessthan>;
	lua_Number ( *lua_tonumber )( lua_State *L, int idx ) = Loader<typestring_is( "lua_tonumber" ), lua_Number ( lua_State *L, int idx )>::Function<&lua_tonumber>;
	lua_Integer ( *lua_tointeger )( lua_State *L, int idx ) = Loader<typestring_is( "lua_tointeger" ), lua_Integer ( lua_State *L, int idx )>::Function<&lua_tointeger>;
	int ( *lua_toboolean )( lua_State *L, int idx ) = Loader<typestring_is( "lua_toboolean" ), int ( lua_State *L, int idx )>::Function<&lua_toboolean>;
	const char *( *lua_tolstring )( lua_State *L, int idx, size_t* len ) = Loader<typestring_is( "lua_tolstring" ), const char *( lua_State *L, int idx, size_t* len )>::Function<&lua_tolstring>;
	size_t ( *lua_objlen )( lua_State *L, int idx ) = Loader<typestring_is( "lua_objlen" ), size_t ( lua_State *L, int idx )>::Function<&lua_objlen>;
	lua_CFunction ( *lua_tocfunction )( lua_State *L, int idx ) = Loader<typestring_is( "lua_tocfunction" ), lua_CFunction ( lua_State *L, int idx )>::Function<&lua_tocfunction>;
	void *( *lua_touserdata )( lua_State *L, int idx ) = Loader<typestring_is( "lua_touserdata" ), void *( lua_State *L, int idx )>::Function<&lua_touserdata>;
	lua_State *( *lua_tothread )( lua_State *L, int idx ) = Loader<typestring_is( "lua_tothread" ), lua_State *( lua_State *L, int idx )>::Function<&lua_tothread>;
	const void *( *lua_topointer )( lua_State *L, int idx ) = Loader<typestring_is( "lua_topointer" ), const void *( lua_State *L, int idx )>::Function<&lua_topointer>;
	void ( *lua_pushnil )( lua_State *L ) = Loader<typestring_is( "lua_pushnil" ), void ( lua_State *L )>::Function<&lua_pushnil>;
	void ( *lua_pushnumber )( lua_State *L, lua_Number n ) = Loader<typestring_is( "lua_pushnumber" ), void ( lua_State *L, lua_Number n )>::Function<&lua_pushnumber>;
	void ( *lua_pushinteger )( lua_State *L, lua_Integer n ) = Loader<typestring_is( "lua_pushinteger" ), void ( lua_State *L, lua_Integer n )>::Function<&lua_pushinteger>;
	void ( *lua_pushlstring )( lua_State *L, const char *s, size_t l ) = Loader<typestring_is( "lua_pushlstring" ), void ( lua_State *L, const char *s, size_t l )>::Function<&lua_pushlstring>;
	void ( *lua_pushstring )( lua_State *L, const char *s ) = Loader<typestring_is( "lua_pushstring" ), void ( lua_State *L, const char *s )>::Function<&lua_pushstring>;
	void ( *lua_pushcclosure )( lua_State *L, lua_CFunction fn, int n ) = Loader<typestring_is( "lua_pushcclosure" ), void ( lua_State *L, lua_CFunction fn, int n )>::Function<&lua_pushcclosure>;
	void ( *lua_pushboolean )( lua_State *L, int b ) = Loader<typestring_is( "lua_pushboolean" ), void ( lua_State *L, int b )>::Function<&lua_pushboolean>;
	void ( *lua_pushlightuserdata )( lua_State *L, void *p ) = Loader<typestring_is( "lua_pushlightuserdata" ), void ( lua_State *L, void *p )>::Function<&lua_pushlightuserdata>;
	int ( *lua_pushthread )( lua_State *L ) = Loader<typestring_is( "lua_pushthread" ), int ( lua_State *L )>::Function<&lua_pushthread>;
	void ( *lua_gettable )( lua_State *L, int idx ) = Loader<typestring_is( "lua_gettable" ), void ( lua_State *L, int idx )>::Function<&lua_gettable>;
	void ( *lua_getfield )( lua_State *L, int idx, const char *k ) = Loader<typestring_is( "lua_getfield" ), void ( lua_State *L, int idx, const char *k )>::Function<&lua_getfield>;
	void ( *lua_rawget )( lua_State *L, int idx ) = Loader<typestring_is( "lua_rawget" ), void ( lua_State *L, int idx )>::Function<&lua_rawget>;
	void ( *lua_rawgeti )( lua_State *L, int idx, int n ) = Loader<typestring_is( "lua_rawgeti" ), void ( lua_State *L, int idx, int n )>::Function<&lua_rawgeti>;
	void ( *lua_createtable )( lua_State *L, int narr, int nrec ) = Loader<typestring_is( "lua_createtable" ), void ( lua_State *L, int narr, int nrec )>::Function<&lua_createtable>;
	void *( *lua_newuserdata )( lua_State *L, size_t sz ) = Loader<typestring_is( "lua_newuserdata" ), void *( lua_State *L, size_t sz )>::Function<&lua_newuserdata>;
	int ( *lua_getmetatable )( lua_State *L, int objindex ) = Loader<typestring_is( "lua_getmetatable" ), int ( lua_State *L, int objindex )>::Function<&lua_getmetatable>;
	void ( *lua_settable )( lua_State *L, int idx ) = Loader<typestring_is( "lua_settable" ), void ( lua_State *L, int idx )>::Function<&lua_settable>;
	void ( *lua_setfield )( lua_State *L, int idx, const char *k ) = Loader<typestring_is( "lua_setfield" ), void ( lua_State *L, int idx, const char *k )>::Function<&lua_setfield>;
	void ( *lua_rawset )( lua_State *L, int idx ) = Loader<typestring_is( "lua_rawset" ), void ( lua_State *L, int idx )>::Function<&lua_rawset>;
	void ( *lua_rawseti )( lua_State *L, int idx, int n ) = Loader<typestring_is( "lua_rawseti" ), void ( lua_State *L, int idx, int n )>::Function<&lua_rawseti>;
	int ( *lua_setmetatable )( lua_State *L, int objindex ) = Loader<typestring_is( "lua_setmetatable" ), int ( lua_State *L, int objindex )>::Function<&lua_setmetatable>;
	void ( *lua_call )( lua_State *L, int nargs, int nresults ) = Loader<typestring_is( "lua_call" ), void ( lua_State *L, int nargs, int nresults )>::Function<&lua_call>;
	int ( *lua_pcall )( lua_State *L, int nargs, int nresults, int errfunc ) = Loader<typestring_is( "lua_pcall" ), int ( lua_State *L, int nargs, int nresults, int errfunc )>::Function<&lua_pcall>;
	int ( *lua_cpcall )( lua_State *L, lua_CFunction func, void *ud ) = Loader<typestring_is( "lua_cpcall" ), int ( lua_State *L, lua_CFunction func, void *ud )>::Function<&lua_cpcall>;
	int ( *lua_load )( lua_State *L, lua_Reader reader, void *dt, const char *chunkname ) = Loader<typestring_is( "lua_load" ), int ( lua_State *L, lua_Reader reader, void *dt, const char *chunkname )>::Function<&lua_load>;
	int ( *lua_dump )( lua_State *L, lua_Writer writer, void *data ) = Loader<typestring_is( "lua_dump" ), int ( lua_State *L, lua_Writer writer, void *data )>::Function<&lua_dump>;
	int ( *lua_resume_real )( lua_State *L, int narg ) = Loader<typestring_is( "lua_resume_real" ), int ( lua_State *L, int narg )>::Function<&lua_resume_real>;
	int ( *lua_yield )( lua_State *L, int nresults ) = Loader<typestring_is( "lua_yield" ), int ( lua_State *L, int nresults )>::Function<&lua_yield>;
	int ( *lua_resume )( lua_State *L, int narg ) = Loader<typestring_is( "lua_resume" ), int ( lua_State *L, int narg )>::Function<&lua_resume>;
	int ( *lua_status )( lua_State *L ) = Loader<typestring_is( "lua_status" ), int ( lua_State *L )>::Function<&lua_status>;
	int ( *lua_gc )( lua_State *L, int what, int data ) = Loader<typestring_is( "lua_gc" ), int ( lua_State *L, int what, int data )>::Function<&lua_gc>;
	int ( *lua_next )( lua_State *L, int idx ) = Loader<typestring_is( "lua_next" ), int ( lua_State *L, int idx )>::Function<&lua_next>;
	void ( *lua_concat )( lua_State *L, int n ) = Loader<typestring_is( "lua_concat" ), void ( lua_State *L, int n )>::Function<&lua_concat>;
	lua_Alloc ( *lua_getallocf )( lua_State *L, void **ud ) = Loader<typestring_is( "lua_getallocf" ), lua_Alloc ( lua_State *L, void **ud )>::Function<&lua_getallocf>;
	void ( *lua_setallocf )( lua_State *L, lua_Alloc f, void *ud ) = Loader<typestring_is( "lua_setallocf" ), void ( lua_State *L, lua_Alloc f, void *ud )>::Function<&lua_setallocf>;
	void ( *lua_setlevel )( lua_State *from, lua_State *to ) = Loader<typestring_is( "lua_setlevel" ), void ( lua_State *from, lua_State *to )>::Function<&lua_setlevel>;
	int ( *lua_getstack )( lua_State *L, int level, lua_Debug *ar ) = Loader<typestring_is( "lua_getstack" ), int ( lua_State *L, int level, lua_Debug *ar )>::Function<&lua_getstack>;
	int ( *lua_getinfo )( lua_State *L, const char *what, lua_Debug *ar ) = Loader<typestring_is( "lua_getinfo" ), int ( lua_State *L, const char *what, lua_Debug *ar )>::Function<&lua_getinfo>;
	const char *( *lua_getlocal )( lua_State *L, const lua_Debug *ar, int n ) = Loader<typestring_is( "lua_getlocal" ), const char *( lua_State *L, const lua_Debug *ar, int n )>::Function<&lua_getlocal>;
	const char *( *lua_setlocal )( lua_State *L, const lua_Debug *ar, int n ) = Loader<typestring_is( "lua_setlocal" ), const char *( lua_State *L, const lua_Debug *ar, int n )>::Function<&lua_setlocal>;
	const char *( *lua_getupvalue )( lua_State *L, int funcindex, int n ) = Loader<typestring_is( "lua_getupvalue" ), const char *( lua_State *L, int funcindex, int n )>::Function<&lua_getupvalue>;
	const char *( *lua_setupvalue )( lua_State *L, int funcindex, int n ) = Loader<typestring_is( "lua_setupvalue" ), const char *( lua_State *L, int funcindex, int n )>::Function<&lua_setupvalue>;
	int ( *lua_sethook )( lua_State *L, lua_Hook func, int mask, int count ) = Loader<typestring_is( "lua_sethook" ), int ( lua_State *L, lua_Hook func, int mask, int count )>::Function<&lua_sethook>;
	lua_Hook ( *lua_gethook )( lua_State *L ) = Loader<typestring_is( "lua_gethook" ), lua_Hook ( lua_State *L )>::Function<&lua_gethook>;
	int ( *lua_gethookmask )( lua_State *L ) = Loader<typestring_is( "lua_gethookmask" ), int ( lua_State *L )>::Function<&lua_gethookmask>;
	int ( *lua_gethookcount )( lua_State *L ) = Loader<typestring_is( "lua_gethookcount" ), int ( lua_State *L )>::Function<&lua_gethookcount>;
	void *( *lua_upvalueid )( lua_State *L, int idx, int n ) = Loader<typestring_is( "lua_upvalueid" ), void *( lua_State *L, int idx, int n )>::Function<&lua_upvalueid>;
	void ( *lua_upvaluejoin )( lua_State *L, int idx1, int n1, int idx2, int n2 ) = Loader<typestring_is( "lua_upvaluejoin" ), void ( lua_State *L, int idx1, int n1, int idx2, int n2 )>::Function<&lua_upvaluejoin>;
	int ( *lua_loadx )( lua_State *L, lua_Reader reader, void *dt, const char *chunkname, const char *mode ) = Loader<typestring_is( "lua_loadx" ), int ( lua_State *L, lua_Reader reader, void *dt, const char *chunkname, const char *mode )>::Function<&lua_loadx>;

	const char *( *lua_pushfstring )( lua_State *, const char *, ... ) = lua_pushfstring_loader;
	int ( *luaL_error )( lua_State *, const char *, ... ) = luaL_error_loader;
}
