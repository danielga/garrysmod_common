#pragma once

#include "ModuleHelper.hpp"

namespace GarrysMod {
template <typename ModuleClass> class Module : public ModuleHelper {
public:
  static inline ModuleFactory CreateModuleFactory() noexcept {
    return [] { return std::make_shared<ModuleClass>(); };
  }

protected:
  typedef int (ModuleClass::*MemberFunctionType)(Lua::ILuaBase *LUA);
  typedef std::function<int(ModuleClass *, Lua::ILuaBase *)>
      MemberFunctionalType;

  bool EnableThink(Lua::ILuaBase *LUA) {
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
