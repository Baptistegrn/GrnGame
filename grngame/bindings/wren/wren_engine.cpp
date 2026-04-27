#include "grngame/bindings/wren/wren_engine.hpp"

#include <cstdlib>
#include <cstring>
#include <filesystem>

#include "grngame/assets/asset_manager.h"
#include "grngame/audio/sound.h"
#include "grngame/audio/sound_info.h"
#include "grngame/bindings/utils.hpp"
#include "grngame/bindings/wren/controller_module.hpp"
#include "grngame/bindings/wren/renderer_module.hpp"
#include "grngame/bindings/wren/sound_module.hpp"
#include "grngame/bindings/wren/utils.hpp"
#include "grngame/bindings/wren/window_module.hpp"
#include "grngame/core/app.h"
#include "grngame/dev/logging.h"
#include "grngame/dev/tracy.h"

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

bool WrenEngine::LoadMainScript(const char *main_script_name)
{
    std::string main_source;

    const EmbeddedAsset *asset = nullptr;
    if (g_app.info.embedded_assets)
    {
        std::string name_with_ext = std::string(main_script_name) + ".wren";
        for (int i = 0; g_app.info.embedded_assets[i].name != NULL; i++)
        {
            if (std::strcmp(g_app.info.embedded_assets[i].name, name_with_ext.c_str()) == 0)
            {
                asset = &g_app.info.embedded_assets[i];
                break;
            }
            if (std::strcmp(g_app.info.embedded_assets[i].name, main_script_name) == 0 && !asset)
            {
                asset = &g_app.info.embedded_assets[i];
            }
        }
    }

    if (asset)
    {
        main_source.assign((const char *)asset->data, asset->size);
        LOG_INFO("[wren] Successfully loaded embedded main script: '%s'", main_script_name);
    }
    else
    {
        const std::filesystem::path path_of_script = BuildModulePath(main_script_name, ".wren");
        if (!ReadTextFile(path_of_script, main_source))
        {
            LOG_ERROR("Failed to read Wren script '%s'", path_of_script.string().c_str());
            return false;
        }
    }

    const WrenInterpretResult result = wrenInterpret(vm, main_module.c_str(), main_source.c_str());
    if (result != WREN_RESULT_SUCCESS)
    {
        LOG_ERROR("Failed to interpret Wren script '%s.wren'", main_script_name);
        return false;
    }

    return true;
}

bool WrenEngine::CallOnStart() const
{
    if (!main_class || !on_start)
        return false;

    wrenEnsureSlots(vm, 1);
    wrenSetSlotHandle(vm, 0, main_class);

    const WrenInterpretResult result = wrenCall(vm, on_start);
    if (result != WREN_RESULT_SUCCESS)
    {
        LOG_ERROR("Error in on_start");
        return false;
    }

    return true;
}

bool WrenEngine::CallOnUpdate(float delta) const
{
    if (!main_class || !on_update)
        return false;

    wrenEnsureSlots(vm, 2);
    wrenSetSlotHandle(vm, 0, main_class);
    wrenSetSlotDouble(vm, 1, (double)delta);

    const WrenInterpretResult result = wrenCall(vm, on_update);
    if (result != WREN_RESULT_SUCCESS)
    {
        LOG_ERROR("Error in on_update");
        return false;
    }

    return true;
}

bool WrenEngine::CallOnFixedUpdate(float delta) const
{
    if (!main_class || !on_fixed_update)
        return false;

    wrenEnsureSlots(vm, 2);
    wrenSetSlotHandle(vm, 0, main_class);
    wrenSetSlotDouble(vm, 1, (double)delta);

    const WrenInterpretResult result = wrenCall(vm, on_fixed_update);
    if (result != WREN_RESULT_SUCCESS)
    {
        LOG_ERROR("Error in on_fixed_update");
        return false;
    }

    return true;
}

bool WrenEngine::CallOnRender() const
{
    if (!main_class || !on_render)
        return false;

    wrenEnsureSlots(vm, 1);
    wrenSetSlotHandle(vm, 0, main_class);

    const WrenInterpretResult result = wrenCall(vm, on_render);
    if (result != WREN_RESULT_SUCCESS)
    {
        LOG_ERROR("Error in on_render");
        return false;
    }

    return true;
}

bool WrenEngine::CallOnDestroy() const
{
    if (!main_class || !on_destroy)
        return false;

    wrenEnsureSlots(vm, 1);
    wrenSetSlotHandle(vm, 0, main_class);

    const WrenInterpretResult result = wrenCall(vm, on_destroy);
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
    if (strcmp(text, "\n") == 0 || strcmp(text, "") == 0)
    {
        return;
    }
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
    if (auto fn = BindForeignMethodCallbackSound(vm, module, class_name, is_static, signature))
        return fn;
    if (auto fn = BindForeignMethodCallbackController(vm, module, class_name, is_static, signature))
        return fn;
    if (auto fn = BindForeignMethodCallbackUtils(vm, module, class_name, is_static, signature))
        return fn;
    if (auto fn = BindForeignMethodCallbackWindow(vm, module, class_name, is_static, signature))
        return fn;
    if (auto fn = BindForeignMethodCallbackRenderer(vm, module, class_name, is_static, signature))
        return fn;
    return nullptr;
}

WrenForeignClassMethods WrenEngine::BindForeignClassCallback(WrenVM *vm, const char *module, const char *class_name)
{
    auto methods = BindForeignClassCallbackSound(vm, module, class_name);
    if (methods.allocate || methods.finalize)
        return methods;
    methods = BindForeignClassCallbackWindow(vm, module, class_name);
    if (methods.allocate || methods.finalize)
        return methods;
    methods = BindForeignClassCallbackController(vm, module, class_name);
    if (methods.allocate || methods.finalize)
        return methods;
    methods = BindForeignClassCallbackRenderer(vm, module, class_name);
    if (methods.allocate || methods.finalize)
        return methods;
    return BindForeignClassCallbackUtils(vm, module, class_name);
}

WrenLoadModuleResult WrenEngine::LoadModuleCallback(WrenVM *vm, const char *module_name)
{
    (void)vm;

    WrenLoadModuleResult result;
    result.onComplete = &WrenEngine::LoadModuleCompleteCallback;
    result.source = nullptr;
    result.userData = nullptr;

    const EmbeddedAsset *asset = nullptr;
    if (g_app.info.embedded_assets)
    {
        std::string name_with_ext = std::string(module_name) + ".wren";
        for (int i = 0; g_app.info.embedded_assets[i].name != NULL; i++)
        {
            if (std::strcmp(g_app.info.embedded_assets[i].name, name_with_ext.c_str()) == 0)
            {
                asset = &g_app.info.embedded_assets[i];
                break;
            }
            if (std::strcmp(g_app.info.embedded_assets[i].name, module_name) == 0 && !asset)
            {
                asset = &g_app.info.embedded_assets[i];
            }
        }
    }

    std::string source;
    if (asset)
    {
        source.assign((const char *)asset->data, asset->size);
    }
    else
    {
        const std::filesystem::path path_of_module = BuildModulePath(module_name, ".wren");
        if (!ReadTextFile(path_of_module, source))
        {
            LOG_ERROR("[wren] Failed to load module '%s' from '%s'", module_name, path_of_module.string().c_str());
            return result;
        }
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
