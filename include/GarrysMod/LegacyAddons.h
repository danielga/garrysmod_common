#pragma once

#include <string>
#include <list>

namespace ILegacyAddons
{

struct Information
{
	std::string placeholder1;
	std::string placeholder2;
	std::string placeholder3;
	std::string placeholder4;
};

}

namespace LegacyAddons
{

class System
{
public:
	virtual void Refresh( ) = 0;
	virtual std::list<ILegacyAddons::Information> &GetList( ) = 0;
};

}
