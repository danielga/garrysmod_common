#include "GarrysMod/ModuleHelper.hpp"
#include "GarrysMod/Lua/LuaInterface.h"

#include "lua.hpp"

namespace GarrysMod {
int ModuleHelper::Initialize(Lua::ILuaBase *LUA) {
  CreateModuleMetaType(LUA);

  ModuleBase::Initialize(LUA);

  for (auto &factory : m_submodule_factories) {
    auto submodule = factory();
    submodule->Initialize(LUA);
    m_submodules.emplace_back(std::move(submodule));
  }

  return 0;
}

int ModuleHelper::Deinitialize(Lua::ILuaBase *LUA) {
  for (auto &submodule : m_submodules) {
    submodule->Deinitialize(LUA);
  }

  m_submodules.clear();

  ModuleBase::Deinitialize(LUA);

  DestroyModuleMetaType(LUA);

  return 0;
}

const std::string& ModuleHelper::ModuleName() const {
    return m_module_name;
}

ModuleHelper::ModuleHelper(const std::string& module_name) : m_module_name(module_name) {}

void ModuleHelper::AddSubModuleFactory(SubModuleFactory&& factory) {
  m_submodule_factories.emplace_back(std::move(factory));
}

bool ModuleHelper::EnableThink(Lua::ILuaBase *LUA,
                               MemberFunctionWrapperType wrapper,
                               int function_reference) const {
  if (m_think_enabled) {
    return true;
  }

  LUA->GetField(Lua::INDEX_GLOBAL, "debug");
  if (!LUA->IsType(-1, Lua::Type::Table)) {
    LUA->Pop(1);
    return false;
  }

  LUA->GetField(-1, "traceback");
  LUA->Remove(-2);
  if (!LUA->IsType(-1, Lua::Type::Function)) {
    LUA->Pop(1);
    return false;
  }

  LUA->GetField(Lua::INDEX_GLOBAL, "hook");
  if (!LUA->IsType(-1, Lua::Type::Table)) {
    LUA->Pop(2); // Pop debug.traceback and whatever we got now
    return false;
  }

  LUA->GetField(-1, "Add");
  LUA->Remove(-2);
  if (!LUA->IsType(-1, Lua::Type::Function)) {
    LUA->Pop(2); // Pop debug.traceback and whatever we got now
    return false;
  }

  LUA->PushString("Think");
  LUA->PushFormattedString("module.%s.Think", ModuleName().c_str());
  PushMemberFunction(LUA, wrapper, function_reference);

  // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
  if (LUA->PCall(3, 0, -5) != 0) {
    dynamic_cast<Lua::ILuaInterface *>(LUA)->ErrorNoHalt("\n%s\n\n",
                                                         LUA->GetString(-1));
    LUA->Pop(1); // Pop error string
  }

  LUA->Pop(1); // Pop debug.traceback

  return true;
}

bool ModuleHelper::DisableThink(Lua::ILuaBase *LUA) const {
  if (!m_think_enabled) {
    return true;
  }

  LUA->GetField(Lua::INDEX_GLOBAL, "debug");
  if (!LUA->IsType(-1, Lua::Type::Table)) {
    LUA->Pop(1);
    return false;
  }

  LUA->GetField(-1, "traceback");
  LUA->Remove(-2);
  if (!LUA->IsType(-1, Lua::Type::Function)) {
    LUA->Pop(1);
    return false;
  }

  LUA->GetField(Lua::INDEX_GLOBAL, "hook");
  if (!LUA->IsType(-1, Lua::Type::Table)) {
    LUA->Pop(2); // Pop debug.traceback and whatever we got now
    return false;
  }

  LUA->GetField(-1, "Remove");
  LUA->Remove(-2);
  if (!LUA->IsType(-1, Lua::Type::Function)) {
    LUA->Pop(2); // Pop debug.traceback and whatever we got now
    return false;
  }

  LUA->PushString("Think");
  LUA->PushFormattedString("module.%s.Think", ModuleName().c_str());

  if (LUA->PCall(2, 0, -4) != 0) {
    dynamic_cast<Lua::ILuaInterface *>(LUA)->ErrorNoHalt("\n%s\n\n",
                                                         LUA->GetString(-1));
    LUA->Pop(1); // Pop error string
  }

  LUA->Pop(1); // Pop debug.traceback

  return true;
}

int ModuleHelper::Think(Lua::ILuaBase *LUA) {
  for (auto &submodule : m_submodules) {
    const int results = submodule->Think(LUA);
    if (results != 0) {
      return results;
    }
  }

  return ModuleBase::Think(LUA);
}
} // namespace GarrysMod
