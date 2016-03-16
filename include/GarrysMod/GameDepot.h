#pragma once

#include <cstdint>

namespace GameDepot
{

class System
{
public:
	virtual void Refresh( ) = 0;
	virtual void Clear( ) = 0;
	virtual void Save( ) = 0;
	virtual void SetMount( uint32_t, bool ) = 0;
	virtual void GetList( ) = 0;
	virtual void MountAsMapFix( uint32_t ) = 0;
};

}
