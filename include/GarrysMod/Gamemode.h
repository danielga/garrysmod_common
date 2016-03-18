#pragma once

#include <string>
#include <list>

namespace IGamemodeSystem
{

struct Information
{
	bool placeholder1;
	bool placeholder2;
	std::string placeholder3;
	std::string placeholder4;
	std::string placeholder5;
	std::string placeholder6;
	std::string placeholder7;
};

}

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
	virtual std::list<IGamemodeSystem::Information> &GetList( );
};

}
