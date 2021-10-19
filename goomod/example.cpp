#include <GarrysMod/Lua/LuaBase.h>
#include <GarrysMod/Module.hpp>
#include <GarrysMod/SubModule.hpp>

#include <cstdio>
#include <stdexcept>

class TestSubModule : public GarrysMod::SubModule<TestSubModule> {
public:
  TestSubModule() { printf("test submodule loaded\n"); }

  ~TestSubModule() override { printf("test submodule unloaded\n"); }

protected:
  typedef GarrysMod::SubModule<TestSubModule> BaseClass;

  int Initialize(GarrysMod::Lua::ILuaBase *LUA) override {
    /// Don't forget to call GarrysMod::SubModule<TestSubModule>::Initialize (or
    /// BaseClass::Initialize, if you create an alias like above).
    BaseClass::Initialize(LUA);

    printf("test submodule initialized\n");

    PushMemberFunction(LUA, &TestSubModule::HelloWorld);
    LUA->SetField(GarrysMod::Lua::INDEX_GLOBAL, "hello");

    LUA->PushString("bar");
    PushMemberClosure(LUA, &TestSubModule::Foo, 1);
    LUA->SetField(GarrysMod::Lua::INDEX_GLOBAL, "foo");

    return 0;
  }

  int Deinitialize(GarrysMod::Lua::ILuaBase *LUA) override {
    printf("test submodule deinitialized\n");
    /// Don't forget to call GarrysMod::SubModule<TestSubModule>::Deinitialize
    /// (or BaseClass::Deinitialize, if you create an alias like above).
    return BaseClass::Deinitialize(LUA);
  }

  int Think(GarrysMod::Lua::ILuaBase *LUA) override {
    printf("test submodule Think\n");
    return BaseClass::Think(LUA);
  }

  virtual int HelloWorld(GarrysMod::Lua::ILuaBase *) {
    printf("Hello world!\n");
    return 0;
  }

  int Foo(GarrysMod::Lua::ILuaBase *LUA) {
    const char *msg = LUA->CheckString(LUA->GetUpvalueIndex(1));
    throw std::runtime_error(msg);
  }
};

class TestModule : public GarrysMod::Module<TestModule> {
public:
  TestModule() {
    printf("test module loaded\n");

    AddSubModuleFactory(TestSubModule::CreateSubModuleFactory());
  }

  ~TestModule() override { printf("test module unloaded\n"); }

protected:
  typedef GarrysMod::Module<TestModule> BaseClass;

  int Initialize(GarrysMod::Lua::ILuaBase *LUA) override {
    /// Don't forget to call GarrysMod::Module<TestModule>::Initialize (or
    /// BaseClass::Initialize, if you create an alias like above).
    BaseClass::Initialize(LUA);

    printf("test module initialized\n");

    EnableThink(LUA);

    PushFunction(LUA, &TestModule::Test);
    LUA->SetField(GarrysMod::Lua::INDEX_GLOBAL, "test");

    return 0;
  }

  int Deinitialize(GarrysMod::Lua::ILuaBase *LUA) override {
    printf("test module deinitialized\n");

    LUA->PushNil();
    LUA->SetField(GarrysMod::Lua::INDEX_GLOBAL, "hello");

    DisableThink(LUA);

    /// Don't forget to call GarrysMod::Module<TestModule>::Deinitialize (or
    /// BaseClass::Deinitialize, if you create an alias like above).
    return BaseClass::Deinitialize(LUA);
  }

  int Think(GarrysMod::Lua::ILuaBase *LUA) override {
    printf("test module Think\n");
    return BaseClass::Think(LUA);
  }

  static int Test(GarrysMod::Lua::ILuaBase *LUA) {
    LUA->PushString("test");
    return 1;
  }
};

namespace GarrysMod {
/// This is necessary to tell GOOMod how to initialize our module.
const ModuleFactory ModuleFactorySingleton = TestModule::CreateModuleFactory();
} // namespace GarrysMod
