#include "grngame/bindings/wren/wren_engine.hpp"

#include <cstdlib>
#include <cstring>

#include "grngame/audio/sound.h"
#include "grngame/audio/sound_info.h"
#include "grngame/bindings/utils.hpp"
#include "grngame/bindings/wren/sound_module.hpp"
#include "grngame/dev/logging.h"

WrenEngine::WrenEngine() = default;

WrenEngine::~WrenEngine()
{
    CallOnDestroy();

    ReleaseHandle(on_start);
    ReleaseHandle(on_update);
    ReleaseHandle(on_fixed_update);
    ReleaseHandle(on_render);
    ReleaseHandle(on_destroy);
    ReleaseHandle(call_zero_args);
    ReleaseHandle(call_one_arg);

    if (vm)
    {
        wrenFreeVM(vm);
        vm = nullptr;
    }
}

bool WrenEngine::Init(const char *main_script_name)
{

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

    call_zero_args = wrenMakeCallHandle(vm, "call()");
    call_one_arg = wrenMakeCallHandle(vm, "call(_)");

    if (!LoadMainScript(main_script_name))
        return false;

    on_start = FindTopLevelFunction("on_start");
    on_update = FindTopLevelFunction("on_update");
    on_fixed_update = FindTopLevelFunction("on_fixed_update");
    on_render = FindTopLevelFunction("on_render");
    on_destroy = FindTopLevelFunction("on_destroy");

    LOG_INFO("Successfully initialized Wren engine with script '%s.wren'", main_script_name);
    CallOnStart();
    return true;
}

bool WrenEngine::LoadMainScript(const char *main_script_name)
{
    std::string main_source;
    const std::filesystem::path path_of_script = BuildModulePath(main_script_name, ".wren");

    if (!ReadTextFile(path_of_script, main_source))
    {
        LOG_ERROR("Failed to read Wren script '%s'", path_of_script.string().c_str());
        return false;
    }

    const WrenInterpretResult result = wrenInterpret(vm, main_module.c_str(), main_source.c_str());
    if (result != WREN_RESULT_SUCCESS)
    {
        LOG_ERROR("Failed to interpret Wren script '%s.wren'", main_script_name);
        return false;
    }

    return true;
}

WrenHandle *WrenEngine::FindTopLevelFunction(const char *function_name) const
{
    if (!wrenHasVariable(vm, main_module.c_str(), function_name))
        return nullptr;

    wrenEnsureSlots(vm, 1);
    wrenSetSlotNull(vm, 0);
    wrenGetVariable(vm, main_module.c_str(), function_name, 0);

    const WrenType slot_type = wrenGetSlotType(vm, 0);
    if (slot_type == WREN_TYPE_NULL)
        return nullptr;

    if (slot_type == WREN_TYPE_NUM || slot_type == WREN_TYPE_BOOL || slot_type == WREN_TYPE_STRING)
        return nullptr;

    return wrenGetSlotHandle(vm, 0);
}

bool WrenEngine::CallOnStart() const
{
    if (!on_start)
        return false;

    wrenEnsureSlots(vm, 1);
    wrenSetSlotHandle(vm, 0, on_start);

    const WrenInterpretResult result = wrenCall(vm, call_zero_args);
    if (result != WREN_RESULT_SUCCESS)
    {
        LOG_ERROR("Error in on_start");
        return false;
    }

    return true;
}

bool WrenEngine::CallOnUpdate(float delta) const
{
    if (!on_update)
        return false;

    wrenEnsureSlots(vm, 2);
    wrenSetSlotHandle(vm, 0, on_update);
    wrenSetSlotDouble(vm, 1, (double)delta);

    const WrenInterpretResult result = wrenCall(vm, call_one_arg);
    if (result != WREN_RESULT_SUCCESS)
    {
        LOG_ERROR("Error in on_update");
        return false;
    }

    return true;
}

bool WrenEngine::CallOnFixedUpdate(float delta) const
{
    if (!on_fixed_update)
        return false;

    wrenEnsureSlots(vm, 2);
    wrenSetSlotHandle(vm, 0, on_fixed_update);
    wrenSetSlotDouble(vm, 1, (double)delta);

    const WrenInterpretResult result = wrenCall(vm, call_one_arg);
    if (result != WREN_RESULT_SUCCESS)
    {
        LOG_ERROR("Error in on_fixed_update");
        return false;
    }

    return true;
}

bool WrenEngine::CallOnRender() const
{
    if (!on_render)
        return false;

    wrenEnsureSlots(vm, 1);
    wrenSetSlotHandle(vm, 0, on_render);

    const WrenInterpretResult result = wrenCall(vm, call_zero_args);
    if (result != WREN_RESULT_SUCCESS)
    {
        LOG_ERROR("Error in on_render");
        return false;
    }

    return true;
}

bool WrenEngine::CallOnDestroy() const
{
    if (!on_destroy)
        return false;

    wrenEnsureSlots(vm, 1);
    wrenSetSlotHandle(vm, 0, on_destroy);

    const WrenInterpretResult result = wrenCall(vm, call_zero_args);
    if (result != WREN_RESULT_SUCCESS)
    {
        LOG_ERROR("Error in on_destroy");
        return false;
    }

    return true;
}

void WrenEngine::ReleaseHandle(WrenHandle *&handle) const
{
    if (vm && handle)
    {
        wrenReleaseHandle(vm, handle);
    }
    handle = nullptr;
}

void WrenEngine::WriteCallback(WrenVM *vm, const char *text)
{
    (void)vm;
    LOG_INFO("[wren] %s", text);
}

void WrenEngine::ErrorCallback(WrenVM *vm, WrenErrorType type, const char *module, int line, const char *message)
{
    (void)vm;

    switch (type)
    {
    case WREN_ERROR_COMPILE:
        LOG_ERROR("[wren][compile] %s:%d %s", module ? module : "<unknown>", line, message ? message : "");
        break;
    case WREN_ERROR_RUNTIME:
        LOG_ERROR("[wren][runtime] %s", message ? message : "");
        break;
    case WREN_ERROR_STACK_TRACE:
        LOG_ERROR("[wren][trace] %s:%d %s", module ? module : "<unknown>", line, message ? message : "");
        break;
    default:
        LOG_ERROR("[wren][unknown] %s", message ? message : "");
        break;
    }
}

// bind every methods of every modules
WrenForeignMethodFn WrenEngine::BindForeignMethodCallback(WrenVM *vm, const char *module, const char *class_name,
                                                          bool is_static, const char *signature)
{
    return BindForeignMethodCallbackSound(vm, module, class_name, is_static, signature);
}

WrenForeignClassMethods WrenEngine::BindForeignClassCallback(WrenVM *vm, const char *module, const char *class_name)
{
    return BindForeignClassCallbackSound(vm, module, class_name);
}

WrenLoadModuleResult WrenEngine::LoadModuleCallback(WrenVM *vm, const char *module_name)
{
    (void)vm;

    WrenLoadModuleResult result;
    result.onComplete = &WrenEngine::LoadModuleCompleteCallback;
    result.source = nullptr;
    result.userData = nullptr;

    std::string source;
    const std::filesystem::path path_of_module = BuildModulePath(module_name, ".wren");
    if (!ReadTextFile(path_of_module, source))
    {
        LOG_ERROR("[wren] Failed to load module '%s' from '%s'", module_name, path_of_module.string().c_str());
        return result;
    }
    // todo use bump allocator
    char *owned_source = (char *)std::malloc(source.size() + 1);
    if (!owned_source)
    {
        LOG_ERROR("[wren] Failed to allocate source for module '%s'", module_name);
        return result;
    }

    std::memcpy(owned_source, source.data(), source.size());
    owned_source[source.size()] = '\0';
    result.source = owned_source;
    return result;
}

void WrenEngine::LoadModuleCompleteCallback(WrenVM *vm, const char *module_name, WrenLoadModuleResult result)
{
    (void)vm;
    (void)module_name;
    if (result.source)
        std::free((void *)result.source);
}
