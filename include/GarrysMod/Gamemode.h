#pragma once

#include <string>
#include <list>

namespace IGamemodeSystem
{

struct Information
{
	bool placeholder1;
	bool menusystem;
	std::string title;
	std::string name;
	std::string maps;
	std::string basename;
	std::string workshopid;
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
	virtual const IGamemodeSystem::Information &Active( );
	virtual void FindByName( const std::string & );
	virtual void SetActive( const std::string & );
	virtual const std::list<IGamemodeSystem::Information> &GetList( ) const;
};

}
