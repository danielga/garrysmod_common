#pragma once

#include "LuaBase.h"
#include <string>

namespace Bootil
{
	class Buffer;
}

class Color;
struct lua_Debug;

namespace GarrysMod
{
	namespace Lua
	{
		class IThreadedCall;
		class ILuaGameCallback;
		class ILuaObject;

		class ILuaInterface : public ILuaBase
		{
		public:
			virtual void *AddThreadedCall( IThreadedCall * ) = 0; // returns something?
			virtual bool Init( ILuaGameCallback *, bool ) = 0;
			virtual void Shutdown( ) = 0;
			virtual void Cycle( ) = 0;
			virtual lua_State *GetLuaState( ) = 0;
			virtual ILuaObject *Global( ) = 0;
			virtual ILuaObject *GetObject( int index ) = 0;
			virtual void DELETE_ME_5437( int ) = 0;
			virtual void _DELETE_ME2466( int ) = 0;
			virtual void PushLuaObject( ILuaObject *obj ) = 0;
			virtual void PushLuaFunction( CFunc func ) = 0;
			virtual void LuaError( const char *err, int index ) = 0;
			virtual void TypeError( const char *name, int index ) = 0;
			virtual void CallInternal( int args, int rets ) = 0;
			virtual void CallInternalNoReturns( int args ) = 0;
			virtual bool CallInternalGetBool( int args ) = 0;
			virtual const char *CallInternalGetString( int args ) = 0;
			virtual bool CallInternalGet( int args, ILuaObject *obj ) = 0;
			virtual void _DELETE_ME( ILuaObject *, void * ) = 0;
			virtual void NewGlobalTable( const char *name ) = 0;
			virtual ILuaObject *NewTemporaryObject( ) = 0;
			virtual bool isUserData( int index ) = 0;
			virtual ILuaObject *GetMetaTableObject( const char *name, int type ) = 0;
			virtual ILuaObject *GetMetaTableObject( int index ) = 0;
			virtual ILuaObject *GetReturn( int index ) = 0;
			virtual bool IsServer( ) = 0;
			virtual bool IsClient( ) = 0;
			virtual bool IsDedicatedServer( ) = 0;
			virtual void DestroyObject( ILuaObject *obj ) = 0;
			virtual ILuaObject *CreateObject( ) = 0;
			virtual void SetMember( ILuaObject *table, ILuaObject *key, ILuaObject *value ) = 0;
			virtual void GetNewTable( ) = 0;
			virtual void SetMember( ILuaObject *table, float key ) = 0;
			virtual void SetMember( ILuaObject *table, float key, ILuaObject *value ) = 0;
			virtual void SetMember( ILuaObject *table, const char *key ) = 0;
			virtual void SetMember( ILuaObject *table, const char *key, ILuaObject *value ) = 0;
			virtual void SetIsServer( bool isServer ) = 0;
			virtual void PushLong( long num ) = 0;
			virtual int GetFlags( int index ) = 0;
			virtual bool FindOnObjectsMetaTable( int objIndex, int keyIndex ) = 0;
			virtual bool FindObjectOnTable( int tableIndex, int keyIndex ) = 0;
			virtual void SetMemberFast( ILuaObject *table, int keyIndex, int valueIndex ) = 0;
			virtual bool RunString( const char *filename, const char *path, const char *stringToRun, bool run, bool showErrors ) = 0;
			virtual bool IsEqual( ILuaObject *objA, ILuaObject *objB ) = 0;
			virtual void Error( const char *err ) = 0;
			virtual const char *GetStringOrError( int index ) = 0;
			virtual bool RunLuaModule( const char *name ) = 0;
			virtual bool FindAndRunScript( const char *filename, bool run, bool showErrors, const char * ) = 0;
			virtual void SetPathID( const char *pathID ) = 0;
			virtual const char *GetPathID( ) = 0;
			virtual void ErrorNoHalt( const char *fmt, ... ) = 0;
			virtual void Msg( const char *fmt, ... ) = 0;
			virtual void PushPath( const char *path ) = 0;
			virtual void PopPath( ) = 0;
			virtual const char *GetPath( ) = 0;
			virtual int GetColor( int index ) = 0;
			virtual void PushColor( int r, int g, int b, int a ) = 0;
			virtual int GetStack( int level, lua_Debug *dbg ) = 0;
			virtual int GetInfo( const char *what, lua_Debug *dbg ) = 0;
			virtual const char *GetLocal( lua_Debug *dbg, int n ) = 0;
			virtual const char *GetUpvalue( int funcIndex, int n ) = 0;
			virtual bool RunStringEx( const char *filename, const char *path, const char *stringToRun, bool run, bool printErrors, bool dontPushErrors, bool noReturns ) = 0;
			virtual void DELETE_ME1( const char *, int ) = 0;
			virtual size_t GetDataString( int index, const char **str ) = 0;
			virtual void ErrorFromLua( const char *fmt, ... ) = 0;
			virtual void GetCurrentLocation( ) = 0;
			virtual void MsgColour( const Color &col, const char *fmt, ... ) = 0;
			virtual void SetState( lua_State *state ) = 0;
			virtual void DELETE_ME2( ) = 0;
			virtual void GetCurrentFile( std::string &outStr ) = 0;
			virtual void CompileString( Bootil::Buffer &dumper, const std::string &stringToCompile ) = 0;
			virtual void ThreadLock( ) = 0;
			virtual void ThreadUnlock( ) = 0;
			virtual bool CallFunctionProtected( int, int, bool ) = 0;
			virtual void Require( const char *name ) = 0;
			virtual const char *GetActualTypeName( int type ) = 0;
			virtual void SetupInfiniteLoopProtection( ) = 0;
			virtual void PreCreateTable( int arrelems, int nonarrelems );
			virtual void PushPooledString( int index );
			virtual const char *GetPooledString( int index );
		};
	}
}
