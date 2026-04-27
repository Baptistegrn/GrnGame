#include "grngame/bindings/wren/wren_engine.hpp"

#include "grngame/core/app.h"
#include "grngame/dev/logging.h"
#include "grngame/dev/tracy.h"

namespace
{
static inline bool CheckWrenCallResult(WrenInterpretResult result, const char *method_name)
{
    if (result != WREN_RESULT_SUCCESS)
        LOG_ERROR("Error in %s", method_name);
    return result == WREN_RESULT_SUCCESS;
}
} // namespace

WrenEngine::WrenEngine() = default;

WrenEngine::~WrenEngine()
{
    CallOnDestroy();

    ReleaseHandle(main_class);
    ReleaseHandle(on_start);
    ReleaseHandle(on_update);
    ReleaseHandle(on_fixed_update);
    ReleaseHandle(on_render);
    ReleaseHandle(on_destroy);

    if (vm)
    {
        wrenFreeVM(vm);
        vm = nullptr;
    }
}

bool WrenEngine::Init(const char *main_script_name)
{
    PROFILE_ZONE_START(initwren, "initialisation of wrenvm");
    main_module = main_script_name;

    WrenConfiguration config;
    wrenInitConfiguration(&config);
    config.userData = this;
    config.writeFn = &WrenEngine::WriteCallback;
    config.errorFn = &WrenEngine::ErrorCallback;
    config.bindForeignMethodFn = &WrenEngine::BindForeignMethodCallback;
    config.bindForeignClassFn = &WrenEngine::BindForeignClassCallback;
    config.loadModuleFn = &WrenEngine::LoadModuleCallback;

    vm = wrenNewVM(&config);
    if (!vm)
    {
        LOG_ERROR("Failed to create Wren VM");
        return false;
    }

    if (!LoadMainScript(main_script_name))
        return false;

    if (!wrenHasVariable(vm, main_module.c_str(), "Main"))
    {
        LOG_ERROR("Class 'Main' not found in script '%s.wren'", main_script_name);
        return false;
    }

    wrenEnsureSlots(vm, 1);
    wrenGetVariable(vm, main_module.c_str(), "Main", 0);

    main_class = wrenGetSlotHandle(vm, 0);

    on_start = wrenMakeCallHandle(vm, "on_start()");
    on_update = wrenMakeCallHandle(vm, "on_update(_)");
    on_fixed_update = wrenMakeCallHandle(vm, "on_fixed_update(_)");
    on_render = wrenMakeCallHandle(vm, "on_render()");
    on_destroy = wrenMakeCallHandle(vm, "on_destroy()");

    LOG_INFO("Successfully initialized Wren engine with script '%s.wren'", main_script_name);
    PROFILE_ZONE_END(initwren);
    PROFILE_ZONE_START(onstart, "start function");
    CallOnStart();
    PROFILE_ZONE_END(onstart);
    return true;
}

bool WrenEngine::CallOnStart() const
{
    if (!main_class || !on_start)
        return false;

    wrenEnsureSlots(vm, 1);
    wrenSetSlotHandle(vm, 0, main_class);

    return CheckWrenCallResult(wrenCall(vm, on_start), "on_start");
}

bool WrenEngine::CallOnUpdate(float delta) const
{
    if (!main_class || !on_update)
        return false;

    wrenEnsureSlots(vm, 2);
    wrenSetSlotHandle(vm, 0, main_class);
    wrenSetSlotDouble(vm, 1, (double)delta);

    return CheckWrenCallResult(wrenCall(vm, on_update), "on_update");
}

bool WrenEngine::CallOnFixedUpdate(float delta) const
{
    if (!main_class || !on_fixed_update)
        return false;

    wrenEnsureSlots(vm, 2);
    wrenSetSlotHandle(vm, 0, main_class);
    wrenSetSlotDouble(vm, 1, (double)delta);

    return CheckWrenCallResult(wrenCall(vm, on_fixed_update), "on_fixed_update");
}

bool WrenEngine::CallOnRender() const
{
    if (!main_class || !on_render)
        return false;

    wrenEnsureSlots(vm, 1);
    wrenSetSlotHandle(vm, 0, main_class);

    return CheckWrenCallResult(wrenCall(vm, on_render), "on_render");
}

bool WrenEngine::CallOnDestroy() const
{
    if (!main_class || !on_destroy)
        return false;

    wrenEnsureSlots(vm, 1);
    wrenSetSlotHandle(vm, 0, main_class);

    return CheckWrenCallResult(wrenCall(vm, on_destroy), "on_destroy");
}

void WrenEngine::ReleaseHandle(WrenHandle *&handle) const
{
    if (vm && handle)
    {
        wrenReleaseHandle(vm, handle);
    }
    handle = nullptr;
}
