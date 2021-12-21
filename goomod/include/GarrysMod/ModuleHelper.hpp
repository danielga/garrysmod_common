#pragma once

#include "ModuleBase.hpp"

#include <vector>

namespace GarrysMod {
class ModuleHelper : public ModuleBase {
protected:
  int Initialize(Lua::ILuaBase *LUA) override;
  int Deinitialize(Lua::ILuaBase *LUA) override;

  void
  AddSubModuleFactory(std::function<std::shared_ptr<ModuleBase>()> &&factory);

  bool EnableThink(Lua::ILuaBase *LUA, MemberFunctionWrapperType wrapper,
                   int function_reference) const;
  bool DisableThink(Lua::ILuaBase *LUA) const;

  int Think(Lua::ILuaBase *LUA) override;

private:
  bool m_think_enabled = false;

  std::vector<std::function<std::shared_ptr<ModuleBase>()>>
      m_submodule_factories;
  std::vector<std::shared_ptr<ModuleBase>> m_submodules;
};
} // namespace GarrysMod
