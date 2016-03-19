#pragma once

#include <string>
#include <list>

namespace ILegacyAddons
{

struct Information
{
	std::string name;
	std::string path;
	std::string luapath;
	std::string placeholder4;
};

}

namespace LegacyAddons
{

class System
{
public:
	virtual void Refresh( ) = 0;
	virtual const std::list<ILegacyAddons::Information> &GetList( ) const = 0;
};

}
