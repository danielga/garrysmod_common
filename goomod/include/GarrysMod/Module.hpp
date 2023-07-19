#pragma once

#include "ModuleHelper.hpp"

namespace GarrysMod {
template <typename ModuleClass> class Module : public ModuleHelper {
public:
  static inline ModuleFactory CreateModuleFactory(const std::string& module_name) noexcept {
    return [=] { return std::make_shared<ModuleClass>(module_name); };
  }
  
#ifdef GMOD_MODULE_NAME
  static inline ModuleFactory CreateModuleFactory() noexcept {
    return CreateModuleFactory(GMOD_MODULE_NAME);
  }
#endif

protected:
  typedef int (ModuleClass::*MemberFunctionType)(Lua::ILuaBase *LUA);
  typedef std::function<int(ModuleClass *, Lua::ILuaBase *)>
      MemberFunctionalType;

  inline Module(const std::string& module_name) : ModuleHelper(module_name) {}

  inline bool EnableThink(Lua::ILuaBase *LUA) {
    const int lua_ref =
        CreateFunctionLuaReference(LUA, &Module::Think, m_member_functions);
    return ModuleHelper::EnableThink(
        LUA, SpecializedMemberFunctionWrapper<ModuleClass>, lua_ref);
  }

  inline int PushMemberFunction(Lua::ILuaBase *LUA,
                                MemberFunctionType function) {
    const int lua_ref =
        CreateFunctionLuaReference(LUA, function, m_member_functions);
    return ModuleBase::PushMemberFunction(
        LUA, SpecializedMemberFunctionWrapper<ModuleClass>, lua_ref);
  }

  inline int PushMemberClosure(Lua::ILuaBase *LUA, MemberFunctionType function,
                               int nargs) {
    const int lua_ref =
        CreateFunctionLuaReference(LUA, function, m_member_functions);
    return ModuleBase::PushMemberClosure(
        LUA, SpecializedMemberFunctionWrapper<ModuleClass>, lua_ref, nargs);
  }

private:
  std::unordered_map<void *, FunctionLuaReference<MemberFunctionalType>>
      m_member_functions;
};
} // namespace GarrysMod
