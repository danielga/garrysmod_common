#pragma once

#include <functional>
#include <memory>
#include <stdexcept>
#include <unordered_map>

struct lua_State;

namespace GarrysMod {
namespace Lua {
class ILuaBase;
}

class ModuleBase : public std::enable_shared_from_this<ModuleBase> {
public:
  virtual ~ModuleBase() = default;

  virtual int Initialize(Lua::ILuaBase *LUA);
  virtual int Deinitialize(Lua::ILuaBase *LUA);

protected:
  static constexpr int lua_noref = -2; // LUA_NOREF

  typedef int (*FunctionType)(Lua::ILuaBase *LUA);
  typedef int (*MemberFunctionWrapperType)(Lua::ILuaBase *LUA, void *object,
                                           void *function);

  struct LuaObjectBase {
    virtual ~LuaObjectBase() = default;
  };

  template <typename Class> struct WeakLuaObject : public LuaObjectBase {
    WeakLuaObject(std::weak_ptr<Class> &&object)
        : weak_object(std::move(object)) {}

    std::weak_ptr<Class> weak_object;
  };

  template <typename MemberFunctionalType>
  struct FunctionLuaReference : public LuaObjectBase {
    FunctionLuaReference(std::shared_ptr<MemberFunctionalType> &&fn,
                         int lua_ref)
        : function(std::move(fn)), lua_reference(lua_ref) {}

    std::shared_ptr<MemberFunctionalType> function;
    int lua_reference;
  };

  ModuleBase();

  virtual int Think(Lua::ILuaBase *LUA);

  static int PushFunction(Lua::ILuaBase *LUA, FunctionType function);
  static int PushClosure(Lua::ILuaBase *LUA, FunctionType function, int nargs);

  int PushMemberFunction(Lua::ILuaBase *LUA, MemberFunctionWrapperType wrapper,
                         int function_reference) const;
  int PushMemberClosure(Lua::ILuaBase *LUA, MemberFunctionWrapperType wrapper,
                        int function_reference, int nargs) const;

  static void CreateModuleMetaType(Lua::ILuaBase *LUA);
  static void DestroyModuleMetaType(Lua::ILuaBase *LUA);

  template <typename ModuleClass>
  static int SpecializedMemberFunctionWrapper(Lua::ILuaBase *LUA, void *object,
                                              void *function) {
    typedef std::function<int(ModuleClass *, Lua::ILuaBase *)>
        MemberFunctionalType;

    auto module = static_cast<ModuleClass *>(object);

    auto weak_function =
        static_cast<WeakLuaObject<MemberFunctionalType> *>(function);
    auto func =
        weak_function != nullptr ? weak_function->weak_object.lock() : nullptr;
    if (!func) {
      throw std::runtime_error("function to be called is invalid");
    }

    return (*func)(module, LUA);
  }

  static int CreateLuaReference(Lua::ILuaBase *LUA,
                                std::unique_ptr<LuaObjectBase> value);
  static void DestroyLuaReference(Lua::ILuaBase *LUA, int reference);

  template <typename MemberFunctionType, typename MemberFunctionalType>
  static int CreateFunctionLuaReference(
      Lua::ILuaBase *LUA, MemberFunctionType function,
      std::unordered_map<void *, FunctionLuaReference<MemberFunctionalType>>
          &member_functions) {
    auto fn = *reinterpret_cast<void **>(&function);

    auto it = member_functions.find(fn);
    if (it != member_functions.end()) {
      return it->second.lua_reference;
    }

    auto shared_function =
        std::make_shared<MemberFunctionalType>(std::mem_fn(function));

    const int lua_reference = CreateLuaReference(
        LUA,
        std::make_unique<WeakLuaObject<MemberFunctionalType>>(shared_function));

    member_functions.emplace(fn,
                             FunctionLuaReference<MemberFunctionalType>(
                                 std::move(shared_function), lua_reference));

    return lua_reference;
  }

  template <typename MemberFunctionType, typename MemberFunctionalType>
  static void DestroyFunctionLuaReference(
      Lua::ILuaBase *LUA, MemberFunctionType function,
      std::unordered_map<void *, FunctionLuaReference<MemberFunctionalType>>
          &member_functions) {
    auto fn = *reinterpret_cast<void **>(&function);

    auto it = member_functions.find(fn);
    if (it == member_functions.end()) {
      return;
    }

    DestroyLuaReference(LUA, it->second.lua_reference);

    member_functions.erase(it);
  }

private:
  ModuleBase(const ModuleBase &) = delete;
  ModuleBase(ModuleBase &&) = delete;

  void CreateModuleLuaReference(Lua::ILuaBase *LUA);
  void DestroyModuleLuaReference(Lua::ILuaBase *LUA);

  static int GetNumberOfUpvalues(Lua::ILuaBase *LUA);

  static int FunctionWrapper(lua_State *L);
  static int MemberFunctionWrapper(lua_State *L);

  static int GarbageCollect(lua_State *L);

  int m_module_reference;

  // Allows 'ModuleHelper::Think' to call 'ModuleBase::Think' on submodules.
  friend class ModuleHelper;
};

typedef std::function<std::shared_ptr<ModuleBase>()> ModuleFactory;

extern const ModuleFactory ModuleFactorySingleton;
} // namespace GarrysMod
