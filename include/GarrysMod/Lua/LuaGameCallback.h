#pragma once

class Color;

namespace GarrysMod
{
	namespace Lua
	{
		class ILuaObject;
		class ILuaInterface;

		class ILuaGameCallback
		{
		public:
			virtual ILuaObject *CreateLuaObject( ) = 0;
			virtual void DestroyLuaObject( ILuaObject *pObject ) = 0;

			virtual void ErrorPrint( const char *error, bool print ) = 0;

			virtual void Msg( const char *msg, bool useless ) = 0;
			virtual void MsgColour( const char *msg, const Color &color ) = 0;

			virtual void LuaError( const std::string &error ) = 0;

			virtual void InterfaceCreated( ILuaInterface *iface ) = 0;
		};
	}
}
