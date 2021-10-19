#pragma once

#include "ModuleBase.hpp"

namespace GarrysMod {
template <typename SubModuleClass> class SubModule : public ModuleBase {
public:
  static inline ModuleFactory CreateSubModuleFactory() {
    return [] { return std::make_shared<SubModuleClass>(); };
  }

protected:
  typedef int (SubModuleClass::*MemberFunctionType)(Lua::ILuaBase *LUA);
  typedef std::function<int(SubModuleClass *, Lua::ILuaBase *)>
      MemberFunctionalType;

  inline int PushMemberFunction(Lua::ILuaBase *LUA,
                                MemberFunctionType function) {
    const int lua_ref =
        CreateFunctionLuaReference(LUA, function, m_member_functions);
    return ModuleBase::PushMemberFunction(
        LUA, SpecializedMemberFunctionWrapper<SubModuleClass>, lua_ref);
  }

  inline int PushMemberClosure(Lua::ILuaBase *LUA, MemberFunctionType function,
                               int nargs) {
    const int lua_ref =
        CreateFunctionLuaReference(LUA, function, m_member_functions);
    return ModuleBase::PushMemberClosure(
        LUA, SpecializedMemberFunctionWrapper<SubModuleClass>, lua_ref, nargs);
  }

private:
  std::unordered_map<void *, FunctionLuaReference<MemberFunctionalType>>
      m_member_functions;
};
} // namespace GarrysMod
