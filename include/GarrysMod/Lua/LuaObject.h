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
class VMatrix;

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
			virtual ILuaObject *GetMember( const char *name, ILuaObject *obj ) = 0;
			virtual ILuaObject *GetMember( ILuaObject *key, ILuaObject *obj ) = 0;

			virtual void SetMetaTable( ILuaObject *obj ) = 0;
			virtual void SetUserData( void *obj ) = 0;

			virtual void Push( ) = 0;

			virtual bool isNil( ) = 0;
			virtual bool isTable( ) = 0;
			virtual bool isString( ) = 0;
			virtual bool isNumber( ) = 0;
			virtual bool isFunction( ) = 0;
			virtual bool isUserData( ) = 0;

			virtual ILuaObject *GetMember( float fKey, ILuaObject* obj ) = 0;

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

			virtual bool RemoveMe( ) = 0;

			virtual void Init( ) = 0;

			virtual void SetFromGlobal( const char * ) = 0;

			virtual int GetStringLen( unsigned int * ) = 0;

			virtual unsigned int GetMemberUInt( const char *, unsigned int ) = 0;

			virtual void SetMember( const char *, unsigned long long ) = 0;
			virtual void SetMember( const char *, int ) = 0;
			virtual void SetReference( int ) = 0;

			virtual void RemoveMember( const char * ) = 0;
			virtual void RemoveMember( float ) = 0;

			virtual bool MemberIsNil( const char * ) = 0;

			virtual void SetMemberDouble( float, double ) = 0;
			virtual double GetMemberDouble( const char *, double ) = 0;
			// NOTE: All members below do NOT exist in ILuaObjects returned from the menusystem!

			virtual BaseEntity *GetMemberEntity( const char *, BaseEntity * ) = 0;
			virtual void SetMemberEntity( float, BaseEntity * ) = 0;
			virtual void SetMemberEntity( const char *, BaseEntity * ) = 0;
			virtual bool isEntity( ) = 0;
			virtual BaseEntity *GetEntity( ) = 0;
			virtual void SetEntity( BaseEntity * ) = 0;

			virtual void SetMemberVector( const char *, Vector * ) = 0;
			virtual void SetMemberVector( const char *, Vector & ) = 0;
			virtual void SetMemberVector( float, Vector * ) = 0;
			virtual Vector *GetMemberVector( const char *, const Vector * ) = 0;
			virtual Vector *GetMemberVector( int ) = 0;
			virtual Vector *GetVector( ) = 0;
			virtual bool isVector( ) = 0;

			virtual void SetMemberAngle( const char *, QAngle * ) = 0;
			virtual void SetMemberAngle( const char *, QAngle & ) = 0;
			virtual QAngle *GetMemberAngle( const char *, QAngle * ) = 0;
			virtual QAngle *GetAngle( ) = 0;
			virtual bool isAngle( ) = 0;

			virtual void SetMemberMatrix( const char *, VMatrix const * ) = 0;
			virtual void SetMemberMatrix( const char *, VMatrix const & ) = 0;
			virtual void SetMemberMatrix( float, VMatrix const * ) = 0;
			virtual void SetMemberMatrix( int, VMatrix const * ) = 0;

			virtual void SetMemberPhysObject( const char *, IPhysicsObject * ) = 0;
			virtual double GetMemberDouble( float, double ) = 0;
		};

		class CLuaObject : public ILuaObject
		{
		public:
			inline ILuaBase *GetLua( ) const
			{
				return m_pLua;
			}

			inline void SetLua( ILuaBase *Lua )
			{
				m_pLua = Lua;
			}

			inline int GetReference( ) const
			{
				return m_iReference;
			}

			inline int GetInternalType( ) const
			{
				return m_iType;
			}

		private:
			bool m_bUserData;
			int m_iType;
			int m_iReference;
			ILuaBase* m_pLua;
		};
	}
}