#include <functional>
#include <cstdarg>
#include <lua.hpp>
#include "typestring.hh"
#include <Platform.hpp>

#if defined SYSTEM_WINDOWS

#include <Windows.h>

#elif defined SYSTEM_POSIX

#include <dlfcn.h>

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

	throw std::bad_function_call( );
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

	throw std::bad_function_call( );
}

extern "C"
{
	void ( *lua_getfenv )( lua_State *, int ) = Loader<typestring_is( "lua_getfenv" ), void ( lua_State *, int )>::Function<&lua_getfenv>;
	int ( *lua_setfenv )( lua_State *, int ) = Loader<typestring_is( "lua_setfenv" ), int ( lua_State *, int )>::Function<&lua_setfenv>;
	const char *( *lua_pushvfstring )( lua_State *, const char *, va_list ) = Loader<typestring_is( "lua_pushvfstring" ), const char *( lua_State *, const char *, va_list )>::Function<&lua_pushvfstring>;
	int ( *lua_error )( lua_State * ) = Loader<typestring_is( "lua_error" ), int ( lua_State * )>::Function<&lua_error>;
	lua_State *( *lua_newstate )( lua_Alloc, void * ) = Loader<typestring_is( "lua_newstate" ), lua_State *( lua_Alloc, void * )>::Function<&lua_newstate>;
	void ( *lua_close )( lua_State * ) = Loader<typestring_is( "lua_close" ), void ( lua_State * )>::Function<&lua_close>;
	lua_State *( *lua_newthread )( lua_State * ) = Loader<typestring_is( "lua_newthread" ), lua_State *( lua_State * )>::Function<&lua_newthread>;
	lua_CFunction ( *lua_atpanic )( lua_State *, lua_CFunction ) = Loader<typestring_is( "lua_atpanic" ), lua_CFunction ( lua_State *, lua_CFunction )>::Function<&lua_atpanic>;
	int ( *lua_gettop )( lua_State * ) = Loader<typestring_is( "lua_gettop" ), int ( lua_State * )>::Function<&lua_gettop>;
	void ( *lua_settop )( lua_State *, int ) = Loader<typestring_is( "lua_settop" ), void ( lua_State *, int )>::Function<&lua_settop>;
	void ( *lua_pushvalue )( lua_State *, int ) = Loader<typestring_is( "lua_pushvalue" ), void ( lua_State *, int )>::Function<&lua_pushvalue>;
	void ( *lua_remove )( lua_State *, int ) = Loader<typestring_is( "lua_remove" ), void ( lua_State *, int )>::Function<&lua_remove>;
	void ( *lua_insert )( lua_State *, int ) = Loader<typestring_is( "lua_insert" ), void ( lua_State *, int )>::Function<&lua_insert>;
	void ( *lua_replace )( lua_State *, int ) = Loader<typestring_is( "lua_replace" ), void ( lua_State *, int )>::Function<&lua_replace>;
	int ( *lua_checkstack )( lua_State *, int ) = Loader<typestring_is( "lua_checkstack" ), int ( lua_State *, int )>::Function<&lua_checkstack>;
	void ( *lua_xmove )( lua_State *, lua_State *, int ) = Loader<typestring_is( "lua_xmove" ), void ( lua_State *, lua_State *, int )>::Function<&lua_xmove>;
	int ( *lua_isnumber )( lua_State *, int ) = Loader<typestring_is( "lua_isnumber" ), int ( lua_State *, int )>::Function<&lua_isnumber>;
	int ( *lua_isstring )( lua_State *, int ) = Loader<typestring_is( "lua_isstring" ), int ( lua_State *, int )>::Function<&lua_isstring>;
	int ( *lua_iscfunction )( lua_State *, int ) = Loader<typestring_is( "lua_iscfunction" ), int ( lua_State *, int )>::Function<&lua_iscfunction>;
	int ( *lua_isuserdata )( lua_State *, int ) = Loader<typestring_is( "lua_isuserdata" ), int ( lua_State *, int )>::Function<&lua_isuserdata>;
	int ( *lua_type )( lua_State *, int ) = Loader<typestring_is( "lua_type" ), int ( lua_State *, int )>::Function<&lua_type>;
	const char *( *lua_typename )( lua_State *, int ) = Loader<typestring_is( "lua_typename" ), const char *( lua_State *, int )>::Function<&lua_typename>;
	int ( *lua_equal )( lua_State *, int, int ) = Loader<typestring_is( "lua_equal" ), int ( lua_State *, int, int )>::Function<&lua_equal>;
	int ( *lua_rawequal )( lua_State *, int, int ) = Loader<typestring_is( "lua_rawequal" ), int ( lua_State *, int, int )>::Function<&lua_rawequal>;
	int ( *lua_lessthan )( lua_State *, int, int ) = Loader<typestring_is( "lua_lessthan" ), int ( lua_State *, int, int )>::Function<&lua_lessthan>;
	lua_Number ( *lua_tonumber )( lua_State *, int ) = Loader<typestring_is( "lua_tonumber" ), lua_Number ( lua_State *, int )>::Function<&lua_tonumber>;
	lua_Integer ( *lua_tointeger )( lua_State *, int ) = Loader<typestring_is( "lua_tointeger" ), lua_Integer ( lua_State *, int )>::Function<&lua_tointeger>;
	int ( *lua_toboolean )( lua_State *, int ) = Loader<typestring_is( "lua_toboolean" ), int ( lua_State *, int )>::Function<&lua_toboolean>;
	const char *( *lua_tolstring )( lua_State *, int, size_t * ) = Loader<typestring_is( "lua_tolstring" ), const char *( lua_State *, int, size_t * )>::Function<&lua_tolstring>;
	size_t ( *lua_objlen )( lua_State *, int ) = Loader<typestring_is( "lua_objlen" ), size_t ( lua_State *, int )>::Function<&lua_objlen>;
	lua_CFunction ( *lua_tocfunction )( lua_State *, int ) = Loader<typestring_is( "lua_tocfunction" ), lua_CFunction ( lua_State *, int )>::Function<&lua_tocfunction>;
	void *( *lua_touserdata )( lua_State *, int ) = Loader<typestring_is( "lua_touserdata" ), void *( lua_State *, int )>::Function<&lua_touserdata>;
	lua_State *( *lua_tothread )( lua_State *, int ) = Loader<typestring_is( "lua_tothread" ), lua_State *( lua_State *, int )>::Function<&lua_tothread>;
	const void *( *lua_topointer )( lua_State *, int ) = Loader<typestring_is( "lua_topointer" ), const void *( lua_State *, int )>::Function<&lua_topointer>;
	void ( *lua_pushnil )( lua_State * ) = Loader<typestring_is( "lua_pushnil" ), void ( lua_State * )>::Function<&lua_pushnil>;
	void ( *lua_pushnumber )( lua_State *, lua_Number ) = Loader<typestring_is( "lua_pushnumber" ), void ( lua_State *, lua_Number )>::Function<&lua_pushnumber>;
	void ( *lua_pushinteger )( lua_State *, lua_Integer ) = Loader<typestring_is( "lua_pushinteger" ), void ( lua_State *, lua_Integer )>::Function<&lua_pushinteger>;
	void ( *lua_pushlstring )( lua_State *, const char *, size_t ) = Loader<typestring_is( "lua_pushlstring" ), void ( lua_State *, const char *, size_t )>::Function<&lua_pushlstring>;
	void ( *lua_pushstring )( lua_State *, const char * ) = Loader<typestring_is( "lua_pushstring" ), void ( lua_State *, const char * )>::Function<&lua_pushstring>;
	void ( *lua_pushcclosure )( lua_State *, lua_CFunction, int ) = Loader<typestring_is( "lua_pushcclosure" ), void ( lua_State *, lua_CFunction, int )>::Function<&lua_pushcclosure>;
	void ( *lua_pushboolean )( lua_State *, int ) = Loader<typestring_is( "lua_pushboolean" ), void ( lua_State *, int )>::Function<&lua_pushboolean>;
	void ( *lua_pushlightuserdata )( lua_State *, void * ) = Loader<typestring_is( "lua_pushlightuserdata" ), void ( lua_State *, void * )>::Function<&lua_pushlightuserdata>;
	int ( *lua_pushthread )( lua_State * ) = Loader<typestring_is( "lua_pushthread" ), int ( lua_State * )>::Function<&lua_pushthread>;
	void ( *lua_gettable )( lua_State *, int ) = Loader<typestring_is( "lua_gettable" ), void ( lua_State *, int )>::Function<&lua_gettable>;
	void ( *lua_getfield )( lua_State *, int, const char * ) = Loader<typestring_is( "lua_getfield" ), void ( lua_State *, int, const char * )>::Function<&lua_getfield>;
	void ( *lua_rawget )( lua_State *, int ) = Loader<typestring_is( "lua_rawget" ), void ( lua_State *, int )>::Function<&lua_rawget>;
	void ( *lua_rawgeti )( lua_State *, int, int ) = Loader<typestring_is( "lua_rawgeti" ), void ( lua_State *, int, int )>::Function<&lua_rawgeti>;
	void ( *lua_createtable )( lua_State *, int, int ) = Loader<typestring_is( "lua_createtable" ), void ( lua_State *, int, int )>::Function<&lua_createtable>;
	void *( *lua_newuserdata )( lua_State *, size_t ) = Loader<typestring_is( "lua_newuserdata" ), void *( lua_State *, size_t )>::Function<&lua_newuserdata>;
	int ( *lua_getmetatable )( lua_State *, int ) = Loader<typestring_is( "lua_getmetatable" ), int ( lua_State *, int )>::Function<&lua_getmetatable>;
	void ( *lua_settable )( lua_State *, int ) = Loader<typestring_is( "lua_settable" ), void ( lua_State *, int )>::Function<&lua_settable>;
	void ( *lua_setfield )( lua_State *, int, const char * ) = Loader<typestring_is( "lua_setfield" ), void ( lua_State *, int, const char * )>::Function<&lua_setfield>;
	void ( *lua_rawset )( lua_State *, int ) = Loader<typestring_is( "lua_rawset" ), void ( lua_State *, int )>::Function<&lua_rawset>;
	void ( *lua_rawseti )( lua_State *, int, int ) = Loader<typestring_is( "lua_rawseti" ), void ( lua_State *, int, int )>::Function<&lua_rawseti>;
	int ( *lua_setmetatable )( lua_State *, int ) = Loader<typestring_is( "lua_setmetatable" ), int ( lua_State *, int )>::Function<&lua_setmetatable>;
	void ( *lua_call )( lua_State *, int, int ) = Loader<typestring_is( "lua_call" ), void ( lua_State *, int, int )>::Function<&lua_call>;
	int ( *lua_pcall )( lua_State *, int, int, int ) = Loader<typestring_is( "lua_pcall" ), int ( lua_State *, int, int, int )>::Function<&lua_pcall>;
	int ( *lua_cpcall )( lua_State *, lua_CFunction, void * ) = Loader<typestring_is( "lua_cpcall" ), int ( lua_State *, lua_CFunction, void * )>::Function<&lua_cpcall>;
	int ( *lua_load )( lua_State *, lua_Reader, void *, const char * ) = Loader<typestring_is( "lua_load" ), int ( lua_State *, lua_Reader, void *, const char * )>::Function<&lua_load>;
	int ( *lua_dump )( lua_State *, lua_Writer, void * ) = Loader<typestring_is( "lua_dump" ), int ( lua_State *, lua_Writer, void * )>::Function<&lua_dump>;
	int ( *lua_resume_real )( lua_State *, int ) = Loader<typestring_is( "lua_resume_real" ), int ( lua_State *, int )>::Function<&lua_resume_real>;
	int ( *lua_yield )( lua_State *, int ) = Loader<typestring_is( "lua_yield" ), int ( lua_State *, int )>::Function<&lua_yield>;
	int ( *lua_resume )( lua_State *, int ) = Loader<typestring_is( "lua_resume" ), int ( lua_State *, int )>::Function<&lua_resume>;
	int ( *lua_status )( lua_State * ) = Loader<typestring_is( "lua_status" ), int ( lua_State * )>::Function<&lua_status>;
	int ( *lua_gc )( lua_State *, int, int ) = Loader<typestring_is( "lua_gc" ), int ( lua_State *, int, int )>::Function<&lua_gc>;
	int ( *lua_next )( lua_State *, int ) = Loader<typestring_is( "lua_next" ), int ( lua_State *, int )>::Function<&lua_next>;
	void ( *lua_concat )( lua_State *, int ) = Loader<typestring_is( "lua_concat" ), void ( lua_State *, int )>::Function<&lua_concat>;
	lua_Alloc ( *lua_getallocf )( lua_State *, void ** ) = Loader<typestring_is( "lua_getallocf" ), lua_Alloc ( lua_State *, void ** )>::Function<&lua_getallocf>;
	void ( *lua_setallocf )( lua_State *, lua_Alloc, void * ) = Loader<typestring_is( "lua_setallocf" ), void ( lua_State *, lua_Alloc, void * )>::Function<&lua_setallocf>;
	void ( *lua_setlevel )( lua_State *, lua_State * ) = Loader<typestring_is( "lua_setlevel" ), void ( lua_State *, lua_State * )>::Function<&lua_setlevel>;
	int ( *lua_getstack )( lua_State *, int , lua_Debug * ) = Loader<typestring_is( "lua_getstack" ), int ( lua_State *, int, lua_Debug * )>::Function<&lua_getstack>;
	int ( *lua_getinfo )( lua_State *, const char *, lua_Debug * ) = Loader<typestring_is( "lua_getinfo" ), int ( lua_State *, const char *, lua_Debug * )>::Function<&lua_getinfo>;
	const char *( *lua_getlocal )( lua_State *, const lua_Debug *, int ) = Loader<typestring_is( "lua_getlocal" ), const char *( lua_State *, const lua_Debug *, int )>::Function<&lua_getlocal>;
	const char *( *lua_setlocal )( lua_State *, const lua_Debug *, int ) = Loader<typestring_is( "lua_setlocal" ), const char *( lua_State *, const lua_Debug *, int )>::Function<&lua_setlocal>;
	const char *( *lua_getupvalue )( lua_State *, int, int ) = Loader<typestring_is( "lua_getupvalue" ), const char *( lua_State *, int, int )>::Function<&lua_getupvalue>;
	const char *( *lua_setupvalue )( lua_State *, int, int ) = Loader<typestring_is( "lua_setupvalue" ), const char *( lua_State *, int, int )>::Function<&lua_setupvalue>;
	int ( *lua_sethook )( lua_State *, lua_Hook, int, int ) = Loader<typestring_is( "lua_sethook" ), int ( lua_State *, lua_Hook, int, int )>::Function<&lua_sethook>;
	lua_Hook ( *lua_gethook )( lua_State * ) = Loader<typestring_is( "lua_gethook" ), lua_Hook ( lua_State * )>::Function<&lua_gethook>;
	int ( *lua_gethookmask )( lua_State * ) = Loader<typestring_is( "lua_gethookmask" ), int ( lua_State * )>::Function<&lua_gethookmask>;
	int ( *lua_gethookcount )( lua_State * ) = Loader<typestring_is( "lua_gethookcount" ), int ( lua_State * )>::Function<&lua_gethookcount>;
	void *( *lua_upvalueid )( lua_State *, int, int ) = Loader<typestring_is( "lua_upvalueid" ), void *( lua_State *, int, int )>::Function<&lua_upvalueid>;
	void ( *lua_upvaluejoin )( lua_State *, int, int, int, int ) = Loader<typestring_is( "lua_upvaluejoin" ), void ( lua_State *, int, int, int, int )>::Function<&lua_upvaluejoin>;
	int ( *lua_loadx )( lua_State *, lua_Reader, void *, const char *, const char * ) = Loader<typestring_is( "lua_loadx" ), int ( lua_State *, lua_Reader, void *, const char *, const char * )>::Function<&lua_loadx>;

	int ( *luaL_typerror )( lua_State *, int, const char * ) = Loader<typestring_is( "luaL_typerror" ), int ( lua_State *, int, const char * )>::Function<&luaL_typerror>;
	void ( *luaL_openlib )( lua_State *, const char *, const luaL_Reg *, int ) = Loader<typestring_is( "luaL_openlib" ), void ( lua_State *, const char *, const luaL_Reg *, int )>::Function<&luaL_openlib>;
	void ( *luaL_register )( lua_State *, const char *, const luaL_Reg * ) = Loader<typestring_is( "luaL_register" ), void ( lua_State *, const char *, const luaL_Reg * )>::Function<&luaL_register>;
	int ( *luaL_getmetafield )( lua_State *, int, const char * ) = Loader<typestring_is( "luaL_getmetafield" ), int ( lua_State *, int, const char * )>::Function<&luaL_getmetafield>;
	int ( *luaL_callmeta )( lua_State *, int, const char * ) = Loader<typestring_is( "luaL_callmeta" ), int ( lua_State *, int, const char * )>::Function<&luaL_callmeta>;
	int ( *luaL_argerror )( lua_State *, int, const char * ) = Loader<typestring_is( "luaL_argerror" ), int ( lua_State *, int, const char * )>::Function<&luaL_argerror>;
	const char *( *luaL_checklstring )( lua_State *, int, size_t * ) = Loader<typestring_is( "luaL_checklstring" ), const char *( lua_State *, int, size_t * )>::Function<&luaL_checklstring>;
	const char *( *luaL_optlstring )( lua_State *, int, const char *, size_t * ) = Loader<typestring_is( "luaL_optlstring" ), const char *( lua_State *, int, const char *, size_t * )>::Function<&luaL_optlstring>;
	lua_Number ( *luaL_checknumber )( lua_State *, int ) = Loader<typestring_is( "luaL_checknumber" ), lua_Number ( lua_State *, int )>::Function<&luaL_checknumber>;
	lua_Number ( *luaL_optnumber )( lua_State *, int, lua_Number ) = Loader<typestring_is( "luaL_optnumber" ), lua_Number ( lua_State *, int, lua_Number )>::Function<&luaL_optnumber>;
	lua_Integer ( *luaL_checkinteger )( lua_State *, int ) = Loader<typestring_is( "luaL_checkinteger" ), lua_Integer ( lua_State *, int )>::Function<&luaL_checkinteger>;
	lua_Integer ( *luaL_optinteger )( lua_State *, int, lua_Integer ) = Loader<typestring_is( "luaL_optinteger" ), lua_Integer ( lua_State *, int, lua_Integer )>::Function<&luaL_optinteger>;
	void ( *luaL_checkstack )( lua_State *, int, const char * ) = Loader<typestring_is( "luaL_checkstack" ), void ( lua_State *, int, const char * )>::Function<&luaL_checkstack>;
	void ( *luaL_checktype )( lua_State *, int, int ) = Loader<typestring_is( "luaL_checktype" ), void ( lua_State *, int, int )>::Function<&luaL_checktype>;
	void ( *luaL_checkany )( lua_State *, int ) = Loader<typestring_is( "luaL_checkany" ), void ( lua_State *, int narg )>::Function<&luaL_checkany>;
	int ( *luaL_newmetatable_type )( lua_State *, const char *, int ) = Loader<typestring_is( "luaL_newmetatable_type" ), int ( lua_State *, const char *, int )>::Function<&luaL_newmetatable_type>;
	int ( *luaL_newmetatable )( lua_State *, const char * ) = Loader<typestring_is( "luaL_newmetatable" ), int ( lua_State *, const char * )>::Function<&luaL_newmetatable>;
	void *( *luaL_checkudata )( lua_State *, int, const char * ) = Loader<typestring_is( "luaL_checkudata" ), void *( lua_State *, int, const char * )>::Function<&luaL_checkudata>;
	void ( *luaL_where )( lua_State *, int ) = Loader<typestring_is( "luaL_where" ), void ( lua_State *, int )>::Function<&luaL_where>;
	int ( *luaL_checkoption )( lua_State *, int, const char *, const char *const[] ) = Loader<typestring_is( "luaL_checkoption" ), int ( lua_State *, int, const char *, const char *const[] )>::Function<&luaL_checkoption>;
	int ( *luaL_ref )( lua_State *, int ) = Loader<typestring_is( "luaL_ref" ), int ( lua_State *, int )>::Function<&luaL_ref>;
	void ( *luaL_unref )( lua_State *, int, int ) = Loader<typestring_is( "luaL_unref" ), void ( lua_State *, int, int )>::Function<&luaL_unref>;
	int ( *luaL_loadfile )( lua_State *, const char * ) = Loader<typestring_is( "luaL_loadfile" ), int ( lua_State *, const char * )>::Function<&luaL_loadfile>;
	int ( *luaL_loadbuffer )( lua_State *, const char *, size_t, const char * ) = Loader<typestring_is( "luaL_loadbuffer" ), int ( lua_State *, const char *, size_t, const char * )>::Function<&luaL_loadbuffer>;
	int ( *luaL_loadstring )( lua_State *, const char * ) = Loader<typestring_is( "luaL_loadstring" ), int ( lua_State *, const char *s )>::Function<&luaL_loadstring>;
	lua_State *( *luaL_newstate )( ) = Loader<typestring_is( "luaL_newstate" ), lua_State *( )>::Function<&luaL_newstate>;
	const char *( *luaL_gsub )( lua_State *, const char *, const char *, const char * ) = Loader<typestring_is( "luaL_gsub" ), const char *( lua_State *, const char *, const char *, const char * )>::Function<&luaL_gsub>;
	const char *( *luaL_findtable )( lua_State *, int, const char *, int ) = Loader<typestring_is( "luaL_findtable" ), const char *( lua_State *, int, const char *, int )>::Function<&luaL_findtable>;
	int ( *luaL_fileresult )( lua_State *, int, const char * ) = Loader<typestring_is( "luaL_fileresult" ), int ( lua_State *, int, const char * )>::Function<&luaL_fileresult>;
	int ( *luaL_execresult )( lua_State *, int ) = Loader<typestring_is( "luaL_execresult" ), int ( lua_State *, int )>::Function<&luaL_execresult>;
	int ( *luaL_loadfilex )( lua_State *, const char *filename, const char *mode ) = Loader<typestring_is( "luaL_loadfilex" ), int ( lua_State *, const char *, const char * )>::Function<&luaL_loadfilex>;
	int ( *luaL_loadbufferx )( lua_State *, const char *buff, size_t sz, const char *name, const char *mode ) = Loader<typestring_is( "luaL_loadbufferx" ), int ( lua_State *, const char *, size_t, const char *, const char * )>::Function<&luaL_loadbufferx>;
	void ( *luaL_traceback )( lua_State *, lua_State *, const char *msg, int level ) = Loader<typestring_is( "luaL_traceback" ), void ( lua_State *, lua_State *, const char *, int )>::Function<&luaL_traceback>;
	void ( *luaL_buffinit )( lua_State *, luaL_Buffer * ) = Loader<typestring_is( "luaL_buffinit" ), void ( lua_State *, luaL_Buffer * )>::Function<&luaL_buffinit>;
	char *( *luaL_prepbuffer )( luaL_Buffer * ) = Loader<typestring_is( "luaL_prepbuffer" ), char *( luaL_Buffer * )>::Function<&luaL_prepbuffer>;
	void ( *luaL_addlstring )( luaL_Buffer *, const char *, size_t ) = Loader<typestring_is( "luaL_addlstring" ), void ( luaL_Buffer *, const char *, size_t )>::Function<&luaL_addlstring>;
	void ( *luaL_addstring )( luaL_Buffer *, const char * ) = Loader<typestring_is( "luaL_addstring" ), void ( luaL_Buffer *, const char * )>::Function<&luaL_addstring>;
	void ( *luaL_addvalue )( luaL_Buffer * ) = Loader<typestring_is( "luaL_addvalue" ), void ( luaL_Buffer * )>::Function<&luaL_addvalue>;
	void ( *luaL_pushresult )( luaL_Buffer * ) = Loader<typestring_is( "luaL_pushresult" ), void ( luaL_Buffer * )>::Function<&luaL_pushresult>;

	const char *( *lua_pushfstring )( lua_State *, const char *, ... ) = lua_pushfstring_loader;
	int ( *luaL_error )( lua_State *, const char *, ... ) = luaL_error_loader;
}
