#pragma once

#include "LuaObject.h"

namespace GarrysMod
{
	namespace Lua
	{
		class AutoLuaObject
		{
		public:
			AutoLuaObject( ) :
				m_pLuaObj( NULL )
			{ };

			AutoLuaObject( ILuaObject *obj ) :
				m_pLuaObj( obj )
			{ };

			~AutoLuaObject( )
			{
				if( !m_pLuaObj )
					return;

				m_pLuaObj->UnReference( );
			};

			ILuaObject *operator->( )
			{
				return m_pLuaObj;
			};

			ILuaObject *operator->( ) const
			{
				return m_pLuaObj;
			};

			operator ILuaObject *( )
			{
				return m_pLuaObj;
			};

			operator ILuaObject *( ) const
			{
				return m_pLuaObj;
			};

			const AutoLuaObject &operator=( const ILuaObject *obj )
			{
				m_pLuaObj = const_cast<ILuaObject *>( obj );
				return *this;
			}

		private:
			ILuaObject *m_pLuaObj;
		};
	}
}