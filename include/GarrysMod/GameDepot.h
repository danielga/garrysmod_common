#pragma once

#include <cstdint>
#include <string>
#include <list>

namespace IGameDepotSystem
{

struct Information
{
	uint32_t placeholder1;
	uint32_t depot;
	std::string title;
	std::string folder;
	bool mounted;
	bool placeholder6;
	bool owned;
	bool installed;
};

}

namespace GameDepot
{

class System
{
public:
	virtual void Refresh( ) = 0;
	virtual void Clear( ) = 0;
	virtual void Save( ) = 0;
	virtual void SetMount( uint32_t, bool ) = 0;
	virtual std::list<IGameDepotSystem::Information> &GetList( ) = 0;
	virtual void MountAsMapFix( uint32_t ) = 0;
};

}
