#pragma once

#include <string>

namespace Gamemode
{

class System
{
public:
	virtual void OnJoinServer( const std::string & );
	virtual void Refresh( );
	virtual void Clear( );
	virtual void Active( );
	virtual void FindByName( const std::string & );
	virtual void SetActive( const std::string & );
	virtual void GetList( );
};

}
