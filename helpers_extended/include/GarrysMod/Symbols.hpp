#pragma once

#include <vector>

#include "Symbol.hpp"

namespace Symbols
{

#if IS_SERVERSIDE

extern const std::vector<Symbol> HandleClientLuaError;
extern const std::vector<Symbol> FileSystemFactory;

#endif

extern const Symbol g_pFullFileSystem;

}
