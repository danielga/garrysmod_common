#include "GarrysMod/Lua/Interface.h"
#include "GarrysMod/Lua/LuaInterface.h"
#include "GarrysMod/ModuleHelper.hpp"

#include "lua.hpp"

namespace GarrysMod {
// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
static std::shared_ptr<ModuleHelper> s_module_singleton;

static int Cleanup(Lua::ILuaBase *LUA,
                   const bool error_on_invalid_module = false) {
  const auto module_singleton = s_module_singleton;

  auto *lua_interface = dynamic_cast<Lua::ILuaInterface *>(LUA);
  if (!module_singleton) {
    if (error_on_invalid_module) {
      lua_interface->ErrorNoHalt("[unknown] module isn't instantiated\n");
    }

    return 0;
  }

  int result = 0;
  try {
    result = module_singleton->Deinitialize(LUA);
  } catch (const std::exception &e) {
    lua_interface->ErrorNoHalt("[%s] deinitialization failure: %s\n",
                               module_singleton->ModuleName().c_str(), e.what());
  }

  s_module_singleton.reset();
  return result;
}

GMOD_MODULE_OPEN() {
  if (!ModuleFactorySingleton) {
    LUA->ThrowError("no module factory registered");
  }

  Cleanup(LUA);

  try {
    auto module = ModuleFactorySingleton();
    const int result = module->Initialize(LUA);
    s_module_singleton = std::move(module);
    return result;
  } catch (const std::exception &e) {
    LUA->PushString(e.what());
  }

  LUA->Error();
}

GMOD_MODULE_CLOSE() { return Cleanup(LUA, true); }
} // namespace GarrysMod
