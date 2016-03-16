#pragma once

#include <cstdint>

class CLanguage
{
public:
	virtual void ChangeLanguage( const char * ) = 0;
	virtual void ChangeLanguage_Steam( const char * ) = 0;
	virtual void GetString( const char *, wchar_t *, int32_t ) = 0;
};
