#pragma once

#include "LuaBase.h"
#include "SourceCompat.h"

#if IS_SERVERSIDE

class CBaseEntity;
typedef CBaseEntity BaseEntity;

#else

class C_BaseEntity;
typedef C_BaseEntity BaseEntity;

#endif

class IPhysicsObject;

namespace GarrysMod
{
	namespace Lua
	{
		class ILuaObject
		{
		public:
			virtual void Set( ILuaObject *obj ) = 0;
			virtual void SetFromStack( int i ) = 0;
			virtual void UnReference( ) = 0;

			virtual int GetType( ) = 0;
			virtual const char *GetString( ) = 0;
			virtual float GetFloat( ) = 0;
			virtual int GetInt( ) = 0;
			virtual void *GetUserData( ) = 0;

			virtual void SetMember( const char *name ) = 0;
			virtual void SetMember( const char *name, ILuaObject *obj ) = 0;
			virtual void SetMember( const char *name, float val ) = 0;
			virtual void SetMember( const char *name, bool val ) = 0;
			virtual void SetMember( const char *name, const char *val ) = 0;
			virtual void SetMember( const char *name, CFunc f ) = 0;

			virtual bool GetMemberBool( const char *name, bool b = true ) = 0;
			virtual int GetMemberInt( const char *name, int i = 0 ) = 0;
			virtual float GetMemberFloat( const char *name, float f = 0.0f ) = 0;
			virtual const char *GetMemberStr( const char *name, const char *s = "" ) = 0;
			virtual void *GetMemberUserData( const char *name, void *u = 0 ) = 0;
			virtual void *GetMemberUserData( float name, void *u = 0 ) = 0;
			virtual ILuaObject *GetMember( const char *name ) = 0;
			virtual ILuaObject *GetMember( ILuaObject *key ) = 0;

			virtual void SetMetaTable( ILuaObject *obj ) = 0;
			virtual void SetUserData( void *obj ) = 0;

			virtual void Push( ) = 0;

			virtual bool isNil( ) = 0;
			virtual bool isTable( ) = 0;
			virtual bool isString( ) = 0;
			virtual bool isNumber( ) = 0;
			virtual bool isFunction( ) = 0;
			virtual bool isUserData( ) = 0;

			virtual ILuaObject *GetMember( float fKey ) = 0;

			virtual void *Remove_Me_1( const char *name, void * = 0 ) = 0;

			virtual void SetMember( float fKey ) = 0;
			virtual void SetMember( float fKey, ILuaObject *obj ) = 0;
			virtual void SetMember( float fKey, float val ) = 0;
			virtual void SetMember( float fKey, bool val ) = 0;
			virtual void SetMember( float fKey, const char *val ) = 0;
			virtual void SetMember( float fKey, CFunc f ) = 0;

			virtual const char *GetMemberStr( float name, const char *s = "" ) = 0;

			virtual void SetMember( ILuaObject *k, ILuaObject *v ) = 0;
			virtual bool GetBool( ) = 0;

			virtual bool PushMemberFast( int iStackPos ) = 0;
			virtual void SetMemberFast( int iKey, int iValue ) = 0;

			virtual void SetFloat( float val ) = 0;
			virtual void SetString( const char *val ) = 0;

			virtual double GetDouble( ) = 0;

			virtual void SetMember_FixKey( const char *, float ) = 0;
			virtual void SetMember_FixKey( const char *, const char * ) = 0;
			virtual void SetMember_FixKey( const char *, ILuaObject * ) = 0;
			virtual void SetMember_FixKey( const char *, double ) = 0;
			virtual void SetMember_FixKey( const char *, int ) = 0;

			virtual bool isBool( ) = 0;

			virtual void SetMemberDouble( const char *, double ) = 0;

			virtual void SetMemberNil( const char * ) = 0;
			virtual void SetMemberNil( float ) = 0;

			virtual bool Debug_IsUnreferenced( ) = 0;

			virtual void Init( ) = 0;

			virtual void SetFromGlobal( const char * ) = 0;
			virtual void SetMember( const char *, unsigned long long ) = 0;
			virtual void SetReference( int ) = 0;

			virtual void RemoveMember( const char * ) = 0;
			virtual void RemoveMember( float ) = 0;

			virtual bool MemberIsNil( const char * ) = 0;

			virtual void SetMemberDouble( float, double ) = 0;
			virtual double GetMemberDouble( const char *, double ) = 0;

			virtual BaseEntity *GetMemberEntity( const char *, BaseEntity * );
			virtual void SetMemberEntity( float, BaseEntity * );
			virtual void SetMemberEntity( const char *, BaseEntity * );
			virtual bool isEntity( );
			virtual BaseEntity *GetEntity( );
			virtual void SetEntity( BaseEntity * );
			virtual void SetMemberVector( const char *, Vector * );
			virtual void SetMemberVector( const char *, Vector & );
			virtual Vector *GetMemberVector( const char *, const Vector * );
			virtual Vector *GetMemberVector( int );
			virtual Vector *GetVector( );
			virtual bool isVector( );
			virtual void SetMemberAngle( const char *, QAngle * );
			virtual QAngle *GetMemberAngle( const char *, QAngle * );
			virtual QAngle *GetAngle( );
			virtual bool isAngle( );
			virtual void SetMemberPhysObject( const char *, IPhysicsObject * );
		};
	}
}
