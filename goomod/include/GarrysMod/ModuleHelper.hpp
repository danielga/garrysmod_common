#pragma once

#include "ModuleBase.hpp"

#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace GarrysMod {
class ModuleHelper : public ModuleBase {
public:
  int Initialize(Lua::ILuaBase* LUA) override;
  int Deinitialize(Lua::ILuaBase* LUA) override;

  const std::string& ModuleName() const;

protected:
  ModuleHelper(const std::string& module_name);

  void
  AddSubModuleFactory(SubModuleFactory&& factory);

  bool EnableThink(Lua::ILuaBase *LUA, MemberFunctionWrapperType wrapper,
                   int function_reference) const;
  bool DisableThink(Lua::ILuaBase *LUA) const;

  int Think(Lua::ILuaBase *LUA) override;

private:
  const std::string m_module_name;
  bool m_think_enabled = false;

  std::vector<SubModuleFactory> m_submodule_factories;
  std::vector<std::shared_ptr<ModuleBase>> m_submodules;
};

typedef std::function<std::shared_ptr<ModuleHelper>()> ModuleFactory;

extern const ModuleFactory ModuleFactorySingleton;
} // namespace GarrysMod
