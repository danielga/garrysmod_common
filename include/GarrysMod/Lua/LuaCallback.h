#pragma once

#include "LuaObject.h"

namespace GarrysMod
{
	namespace Lua
	{
		class ILuaCallback
		{
		public:
			virtual ILuaObject *CreateLuaObject( ) = 0;
			virtual void DestroyLuaObject( ILuaObject *pObject ) = 0;

			virtual void ErrorPrint( const char *strError ) = 0;
			virtual void Msg( const char *strMsg ) = 0;

			virtual bool CanRunScript( const char *strFilename, unsigned long CRC ) = 0;
			virtual void onRunScript( const char *strFilename, bool bRun, const char *strScriptContents ) = 0;
		};
	}
}