#pragma once

#include <cstdint>
#include <string>
#include <list>

namespace IGameDepotSystem
{

struct Information
{
	bool placeholder1;
	uint32_t placeholder2;
	std::string placeholder3;
	std::string placeholder4;
	bool placeholder5;
	bool placeholder6;
	bool placeholder7;
	bool placeholder8;
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
