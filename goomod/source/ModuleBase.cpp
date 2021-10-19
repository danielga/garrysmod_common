#include "GarrysMod/ModuleBase.hpp"
#include "GarrysMod/Lua/Interface.h"
#include "GarrysMod/Lua/LuaInterface.h"

#include "lua.hpp"

namespace GarrysMod {
// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
static int s_object_metatype = LUA_REFNIL;

int ModuleBase::Initialize(Lua::ILuaBase *LUA) {
  CreateModuleLuaReference(LUA);
  return 0;
}

int ModuleBase::Deinitialize(Lua::ILuaBase *LUA) {
  DestroyModuleLuaReference(LUA);
  return 0;
}

int ModuleBase::PushFunction(Lua::ILuaBase *LUA, FunctionType function) {
  LUA->PushUserdata(reinterpret_cast<void *>(function));
  LUA->PushCClosure(FunctionWrapper, 1);
  return 1;
}

int ModuleBase::PushClosure(Lua::ILuaBase *LUA, FunctionType function,
                            int nargs) {
  LUA->PushUserdata(reinterpret_cast<void *>(function));
  LUA->PushCClosure(FunctionWrapper, nargs + 1);
  return 1;
}

int ModuleBase::PushMemberFunction(Lua::ILuaBase *LUA,
                                   MemberFunctionWrapperType wrapper,
                                   int function_reference) const {
  LUA->PushUserdata(reinterpret_cast<void *>(wrapper));
  LUA->ReferencePush(m_module_reference);
  LUA->ReferencePush(function_reference);
  LUA->PushCClosure(MemberFunctionWrapper, 3);
  return 1;
}

int ModuleBase::PushMemberClosure(Lua::ILuaBase *LUA,
                                  MemberFunctionWrapperType wrapper,
                                  int function_reference, int nargs) const {
  LUA->PushUserdata(reinterpret_cast<void *>(wrapper));
  LUA->ReferencePush(m_module_reference);
  LUA->ReferencePush(function_reference);
  LUA->PushCClosure(MemberFunctionWrapper, nargs + 3);
  return 1;
}

void ModuleBase::CreateModuleMetaType(Lua::ILuaBase *LUA) {
  if (s_object_metatype != LUA_REFNIL) {
    return;
  }

  s_object_metatype = LUA->CreateMetaTable("GOOModObject");

  LUA->PushCFunction(GarbageCollect);
  LUA->SetField(-2, "__gc");

  LUA->Pop(1);
}

void ModuleBase::DestroyModuleMetaType(Lua::ILuaBase *LUA) {
  if (s_object_metatype == LUA_REFNIL) {
    return;
  }

  LUA->PushMetaTable(s_object_metatype);

  LUA->PushNil();
  LUA->SetField(-2, "__gc");

  LUA->Pop(1);

  s_object_metatype = LUA_REFNIL;
}

int ModuleBase::CreateLuaReference(Lua::ILuaBase *LUA,
                                   std::unique_ptr<LuaObjectBase> value) {
  LUA->PushUserType(value.release(), s_object_metatype);

  LUA->PushMetaTable(s_object_metatype);
  LUA->SetMetaTable(-2);

  return LUA->ReferenceCreate();
}

void ModuleBase::DestroyLuaReference(Lua::ILuaBase *LUA, int reference) {
  if (reference < 0) {
    return;
  }

  LUA->ReferencePush(reference);

  // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
  delete LUA->GetUserType<LuaObjectBase>(-1, s_object_metatype);

  LUA->SetUserType(-1, nullptr);

  LUA->Pop(1);

  LUA->ReferenceFree(reference);
}

int ModuleBase::Think(Lua::ILuaBase * /*unused*/) { return 0; }

ModuleBase::ModuleBase() : m_module_reference(LUA_NOREF) {}

void ModuleBase::CreateModuleLuaReference(Lua::ILuaBase *LUA) {
  m_module_reference = CreateLuaReference(
      LUA, std::make_unique<WeakLuaObject<ModuleBase>>(weak_from_this()));
}

void ModuleBase::DestroyModuleLuaReference(Lua::ILuaBase *LUA) {
  DestroyLuaReference(LUA, m_module_reference);
  m_module_reference = LUA_NOREF;
}

int ModuleBase::GetNumberOfUpvalues(Lua::ILuaBase *LUA) {
  lua_Debug dbg;
  if (LUA->GetStack(0, &dbg) != 1) {
    LUA->ThrowError("unable to get runtime stack information");
  }

  if (LUA->GetInfo("u", &dbg) != 1) {
    LUA->ThrowError("unable to get function invocation information");
  }

  return dbg.nups;
}

int ModuleBase::FunctionWrapper(lua_State *L) {
  auto *LUA = L->luabase;
  LUA->SetState(L);

  const int nups = GetNumberOfUpvalues(LUA);

  auto function = reinterpret_cast<FunctionType>(
      LUA->GetUserdata(GarrysMod::Lua::ILuaBase::GetUpvalueIndex(nups)));

  try {
    if (function == nullptr) {
      throw std::runtime_error("function is invalid");
    }

    return function(LUA);
  } catch (const std::exception &e) {
    LUA->PushString(e.what());
  }

  LUA->Error();
}

int ModuleBase::MemberFunctionWrapper(lua_State *L) {
  auto *LUA = L->luabase;
  LUA->SetState(L);

  const int nups = GetNumberOfUpvalues(LUA);

  void *lua_wrapper =
      LUA->GetUserdata(GarrysMod::Lua::ILuaBase::GetUpvalueIndex(nups - 2));
  auto wrapper = *reinterpret_cast<MemberFunctionWrapperType *>(&lua_wrapper);
  auto *weak_module = LUA->GetUserType<WeakLuaObject<ModuleBase>>(
      GarrysMod::Lua::ILuaBase::GetUpvalueIndex(nups - 1), s_object_metatype);
  void *function = LUA->GetUserType<void>(
      GarrysMod::Lua::ILuaBase::GetUpvalueIndex(nups), s_object_metatype);

  try {
    if (wrapper == nullptr) {
      throw std::runtime_error("function wrapper is invalid");
    }

    auto module =
        weak_module != nullptr ? weak_module->weak_object.lock() : nullptr;
    if (!module) {
      throw std::runtime_error("module isn't instantiated");
    }

    return wrapper(LUA, module.get(), function);
  } catch (const std::exception &e) {
    LUA->PushString(e.what());
  }

  LUA->Error();
}

int ModuleBase::GarbageCollect(lua_State *L) {
  auto *LUA = L->luabase;
  LUA->SetState(L);

  // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
  delete LUA->GetUserType<LuaObjectBase>(1, s_object_metatype);

  LUA->SetUserType(1, nullptr);

  return 0;
}
} // namespace GarrysMod
