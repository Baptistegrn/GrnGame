// todo move functions to correct files

#include "grngame/bindings/wren/wren_api.h"

#include "grngame/core/app.h"
#include "grngame/core/init.h"
#include "grngame/core/window.h"
#include "grngame/data/file.h"
#include "grngame/dev/logging.h"
#include "grngame/platform/paths.h"
#include "grngame/renderer/palette.h"
#include "grngame/renderer/renderer.h"
#include "grngame/utils/attributes.h"
#include "grngame/utils/clear.h"
#include "grngame/utils/string_compat.h"

#ifdef EMBEDDED_ASSETS_DATA_AVAILABLE
#include "grngame/assets/load.h"
#endif

#include "khash.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define KEY_SIZE 1024
#define MODULE_SIZE_MAX_NAME 512

ModuleRegisterFn g_moduleRegistry[MAX_MODULES];
int g_moduleCount = 0;

void RegisterControllerModule(void);
void RegisterDbModule(void);
void RegisterFileModule(void);
void RegisterInputTextModule(void);
void RegisterKeyboardModule(void);
void RegisterLogModule(void);
void RegisterMouseModule(void);
void RegisterRendererModule(void);
void RegisterSoundModule(void);
void RegisterTimeModule(void);
void RegisterWindowModule(void);

static const char *CALL_SIGNATURES[17] = {"call()",
                                          "call(_)",
                                          "call(_,_)",
                                          "call(_,_,_)",
                                          "call(_,_,_,_)",
                                          "call(_,_,_,_,_)",
                                          "call(_,_,_,_,_,_)",
                                          "call(_,_,_,_,_,_,_)",
                                          "call(_,_,_,_,_,_,_,_)",
                                          "call(_,_,_,_,_,_,_,_,_)",
                                          "call(_,_,_,_,_,_,_,_,_,_)",
                                          "call(_,_,_,_,_,_,_,_,_,_,_)",
                                          "call(_,_,_,_,_,_,_,_,_,_,_,_)",
                                          "call(_,_,_,_,_,_,_,_,_,_,_,_,_)",
                                          "call(_,_,_,_,_,_,_,_,_,_,_,_,_,_)",
                                          "call(_,_,_,_,_,_,_,_,_,_,_,_,_,_,_)",
                                          "call(_,_,_,_,_,_,_,_,_,_,_,_,_,_,_,_)"};

KHASH_MAP_INIT_STR(wren_method_map, WrenForeignMethodFn)
KHASH_MAP_INIT_STR(wren_class_map, WrenForeignClassMethods)

static khash_t(wren_method_map) *g_methods = NULL;
static khash_t(wren_class_map) *g_classes = NULL;

static void MakeMethodKey(char *buffer, uint64 max, const char *module, const char *className, bool isStatic,
                          const char *signature)
{
    snprintf(buffer, max, "%s:%s:%s:%s", module, className, isStatic ? "s" : "i", signature);
}

static void MakeClassKey(char *buffer, uint64 max, const char *module, const char *className)
{
    snprintf(buffer, max, "%s:%s", module, className);
}

void InitBindingSystem()
{
    if (!g_methods)
        g_methods = kh_init(wren_method_map);
    if (!g_classes)
        g_classes = kh_init(wren_class_map);
}

void RegisterMethod(const char *module, const char *className, bool isStatic, const char *signature,
                    WrenForeignMethodFn fn)
{
    char key[KEY_SIZE];
    MakeMethodKey(key, sizeof(key), module, className, isStatic, signature);

    int32 ret;
    khint_t k = kh_put(wren_method_map, g_methods, strdup(key), &ret);
    kh_value(g_methods, k) = fn;
}

void RegisterClass(const char *module, const char *className, WrenForeignMethodFn allocateFn,
                   WrenFinalizerFn finalizeFn)
{
    char key[KEY_SIZE];
    MakeClassKey(key, sizeof(key), module, className);

    int32 ret;
    khint_t k = kh_put(wren_class_map, g_classes, strdup(key), &ret);

    WrenForeignClassMethods methods;
    methods.allocate = allocateFn;
    methods.finalize = finalizeFn;

    kh_value(g_classes, k) = methods;
}

void WriteFn(WrenVM *vm, const char *text)
{
    (void)vm;
    LOG_INFO("%s", text);
}

void ErrorFn(WrenVM *vm, WrenErrorType errorType, const char *module, int32 line, const char *msg)
{
    (void)vm;
    if (errorType == WREN_ERROR_COMPILE)
    {
        LOG_ERROR("[%s line %d] [Error] %s\n", module, line, msg);
    }
    else if (errorType == WREN_ERROR_STACK_TRACE)
    {
        LOG_ERROR("[%s line %d] in %s\n", module, line, msg);
    }
    else if (errorType == WREN_ERROR_RUNTIME)
    {
        LOG_ERROR("[Runtime Error] %s\n", msg);
    }
}

WrenForeignMethodFn BindMethodFn(WrenVM *vm, const char *module, const char *className, bool isStatic,
                                 const char *signature)
{
    (void)vm;
    if (!g_methods)
        return NULL;

    char key[KEY_SIZE];
    MakeMethodKey(key, sizeof(key), module, className, isStatic, signature);

    khint_t k = kh_get(wren_method_map, g_methods, key);
    if (k != kh_end(g_methods))
    {
        return kh_value(g_methods, k);
    }

    return NULL;
}

WrenForeignClassMethods BindClassFn(WrenVM *vm, const char *module, const char *className)
{
    (void)vm;
    WrenForeignClassMethods fallback = {NULL, NULL};
    if (!g_classes)
        return fallback;

    char key[KEY_SIZE];
    MakeClassKey(key, sizeof(key), module, className);

    khint_t k = kh_get(wren_class_map, g_classes, key);
    if (k != kh_end(g_classes))
    {
        return kh_value(g_classes, k);
    }

    return fallback;
}

static void LoadModuleComplete(WrenVM *vm, const char *module, WrenLoadModuleResult result)
{
    (void)vm;
    (void)module;
    if (result.source)
    {
        free((void *)result.source);
    }
}

WrenLoadModuleResult LoadModuleFn(WrenVM *vm, const char *name)
{
    (void)vm;
    WrenLoadModuleResult result = {0};
    result.onComplete = LoadModuleComplete;
    result.source = NULL;

    char filename[MODULE_SIZE_MAX_NAME];
    snprintf(filename, sizeof(filename), "%s.wren", name);

#ifdef EMBEDDED_ASSETS_DATA_AVAILABLE
    {
        EmbeddedAsset *asset = GetEmbeddedAsset(filename);

        if (asset)
        {
            uint32 sz = asset->size;
            char *buf = malloc(sz + 1);
            memcpy(buf, asset->data, sz);
            buf[sz] = '\0';
            result.source = buf;
        }
        else
        {
            LOG_ERROR("Wren Import Error: Failed to find module '%s' in embedded files", name);
        }
    }
#else
    {
        char script_path[MODULE_SIZE_MAX_NAME + 15];
        snprintf(script_path, sizeof(script_path), "%s", filename);

        char *path = PathFromExecutableDirectory(script_path);
        if (path)
        {
            result.source = ReturnFileString(path);
            free(path);
        }

        if (!result.source)
        {
            LOG_ERROR("Wren Import Error: Failed to find module '%s'", script_path);
        }
    }
#endif

    return result;
}

static bool WrenGetObjectField(const char *module, const char *variable, const char *field, WrenHandle **obj,
                               WrenHandle **call)
{
    WrenVM *vm = g_app.wren_manager.vm;

    if (!WrenGetVariable(module, variable))
        return false;

    wrenEnsureSlots(vm, 2);
    *obj = wrenGetSlotHandle(vm, 0);
    *call = wrenMakeCallHandle(vm, field);
    wrenSetSlotHandle(vm, 0, *obj);
    return true;
}

static void WrenReleaseFieldHandles(WrenVM *vm, WrenHandle **obj, WrenHandle **call)
{
    wrenReleaseHandle(vm, *obj);
    wrenReleaseHandle(vm, *call);
}

bool WrenGetVariable(const char *module, const char *variable)
{
    WrenVM *vm = g_app.wren_manager.vm;

    if (!wrenHasVariable(vm, module, variable))
    {
        LOG_ERROR("Wren: variable '%s' not found in module '%s'", variable, module);
        return false;
    }

    wrenEnsureSlots(vm, 2);
    wrenGetVariable(vm, module, variable, 0);
    return true;
}

const char *WrenGetString(const char *module, const char *variable, const char *field)
{
    WrenVM *vm = g_app.wren_manager.vm;
    WrenHandle *obj = NULL;
    WrenHandle *call = NULL;

    if (!WrenGetObjectField(module, variable, field, &obj, &call))
        return NULL;

    WrenInterpretResult result = wrenCall(vm, call);

    WrenReleaseFieldHandles(vm, &obj, &call);

    if (result != WREN_RESULT_SUCCESS)
    {
        LOG_ERROR("Wren: failed to get string field '%s'", field);
        return NULL;
    }

    return wrenGetSlotString(vm, 0);
}

float64 WrenGetDouble(const char *module, const char *variable, const char *field)
{
    WrenVM *vm = g_app.wren_manager.vm;
    WrenHandle *obj = NULL;
    WrenHandle *call = NULL;

    if (!WrenGetObjectField(module, variable, field, &obj, &call))
        return 0.0;

    WrenInterpretResult result = wrenCall(vm, call);

    WrenReleaseFieldHandles(vm, &obj, &call);

    if (result != WREN_RESULT_SUCCESS)
    {
        LOG_ERROR("Wren: failed to get float64 field '%s'", field);
        return 0.0;
    }

    return wrenGetSlotDouble(vm, 0);
}

int32 WrenGetInt(const char *module, const char *variable, const char *field)
{
    return (int32)WrenGetDouble(module, variable, field);
}

bool WrenGetBool(const char *module, const char *variable, const char *field)
{
    WrenVM *vm = g_app.wren_manager.vm;
    WrenHandle *obj = NULL;
    WrenHandle *call = NULL;

    if (!WrenGetObjectField(module, variable, field, &obj, &call))
        return false;

    WrenInterpretResult result = wrenCall(vm, call);

    WrenReleaseFieldHandles(vm, &obj, &call);

    if (result != WREN_RESULT_SUCCESS)
    {
        LOG_ERROR("Wren: failed to get bool field '%s'", field);
        return false;
    }

    return wrenGetSlotBool(vm, 0);
}

static bool WrenGetListObject(const char *module, const char *variable, const char *field, WrenVM **vm,
                              WrenHandle **obj, WrenHandle **call)
{
    *vm = g_app.wren_manager.vm;

    if (!WrenGetObjectField(module, variable, field, obj, call))
        return false;

    WrenInterpretResult result = wrenCall(*vm, *call);

    if (result != WREN_RESULT_SUCCESS)
    {
        LOG_ERROR("Wren: failed to get list field '%s'", field);
        WrenReleaseFieldHandles(*vm, obj, call);
        return false;
    }

    if (wrenGetSlotType(*vm, 0) != WREN_TYPE_LIST)
    {
        LOG_ERROR("Wren: field '%s' is not a list", field);
        WrenReleaseFieldHandles(*vm, obj, call);
        return false;
    }

    return true;
}

int32 WrenGetListCount(const char *module, const char *variable, const char *field)
{
    WrenVM *vm = NULL;
    WrenHandle *obj = NULL;
    WrenHandle *call = NULL;

    if (UNLIKELY(!WrenGetListObject(module, variable, field, &vm, &obj, &call)))
        return 0;

    int32 count = wrenGetListCount(vm, 0);
    WrenReleaseFieldHandles(vm, &obj, &call);
    return count;
}

float64 WrenGetListDouble(const char *module, const char *variable, const char *field, int32 index)
{
    WrenVM *vm = NULL;
    WrenHandle *obj = NULL;
    WrenHandle *call = NULL;

    if (UNLIKELY(index < 0))
        index = 0;

    if (UNLIKELY(!WrenGetListObject(module, variable, field, &vm, &obj, &call)))
        return 0.0;
    wrenGetListElement(vm, 0, index, 1);
    float64 result = wrenGetSlotDouble(vm, 1);
    WrenReleaseFieldHandles(vm, &obj, &call);
    return result;
}

const char *WrenGetListString(const char *module, const char *variable, const char *field, int32 index)
{
    WrenVM *vm = NULL;
    WrenHandle *obj = NULL;
    WrenHandle *call = NULL;

    if (UNLIKELY(index < 0))
        index = 0;

    if (UNLIKELY(!WrenGetListObject(module, variable, field, &vm, &obj, &call)))
        return NULL;
    wrenGetListElement(vm, 0, index, 1);

    if (UNLIKELY(wrenGetSlotType(vm, 1) != WREN_TYPE_STRING))
    {
        LOG_ERROR("Wren: element %d is not a string", index);
        WrenReleaseFieldHandles(vm, &obj, &call);
        return NULL;
    }

    const char *result = wrenGetSlotString(vm, 1);
    WrenReleaseFieldHandles(vm, &obj, &call);
    return result;
}

static void WrenSetWriteFn(WrenWriteFn writeFn)
{
    g_app.wren_manager.config.writeFn = writeFn;
}

static void WrenSetErrorFn(WrenErrorFn errorFn)
{
    g_app.wren_manager.config.errorFn = errorFn;
}

static void WrenSetBindMethodFn(WrenBindForeignMethodFn bindMethodFn)
{
    g_app.wren_manager.config.bindForeignMethodFn = bindMethodFn;
}

static void WrenSetBindClassFn(WrenBindForeignClassFn bindClassFn)
{
    g_app.wren_manager.config.bindForeignClassFn = bindClassFn;
}

static void WrenSetLoadModuleFn(WrenLoadModuleFn loadModuleFn)
{
    g_app.wren_manager.config.loadModuleFn = loadModuleFn;
}

static void WrenStartVM()
{
    g_app.wren_manager.vm = wrenNewVM(&(g_app.wren_manager.config));
}

static bool WrenInterpret(const char *filename)
{
    char *module_name = FileStem(filename);
#ifdef EMBEDDED_ASSETS_DATA_AVAILABLE
    {
        EmbeddedAsset *asset = GetEmbeddedAsset(filename);
        if (!asset)
        {
            LOG_ERROR("Failed to find script '%s' in embedded files", filename);
            return false;
        }

        uint32 sz = asset->size;
        char *buf = malloc(sz + 1);
        memcpy(buf, asset->data, sz);
        buf[sz] = '\0';
        char *source = buf;
        WrenInterpretResult result = wrenInterpret(g_app.wren_manager.vm, module_name, source);
        free(buf);

        if (result != WREN_RESULT_SUCCESS)
        {
            LOG_ERROR("wrenInterpret failed for embedded '%s'", filename);
            return false;
        }

        return true;
    }

#else
    {
        char *path = PathFromExecutableDirectory(filename);
        if (!path)
            return false;

        char *file_content = ReturnFileString(path);
        free(path);

        if (!file_content)
        {
            LOG_ERROR("Failed to read script '%s'", filename);
            return false;
        }

        WrenInterpretResult result = wrenInterpret(g_app.wren_manager.vm, module_name, file_content);
        free(file_content);

        if (result != WREN_RESULT_SUCCESS)
        {
            LOG_ERROR("wrenInterpret failed for '%s'", filename);
            return false;
        }

        return true;
    }
#endif
}

static void WrenSetCallHandle()
{
    for (int i = 0; i <= 16; i++)
    {
        g_app.wren_manager.call[i] = wrenMakeCallHandle(g_app.wren_manager.vm, CALL_SIGNATURES[i]);
    }
}

static void RegisterWrenModules(void)
{
    InitBindingSystem();
    RegisterControllerModule();
    RegisterDbModule();
    RegisterFileModule();
    RegisterInputTextModule();
    RegisterKeyboardModule();
    RegisterLogModule();
    RegisterMouseModule();
    RegisterRendererModule();
    RegisterSoundModule();
    RegisterTimeModule();
    RegisterWindowModule();
}

bool WrenInit()
{
    g_app.wren_manager = (WrenManager){0};
    RegisterWrenModules();
    wrenInitConfiguration(&(g_app.wren_manager.config));
    WrenSetWriteFn(WriteFn);
    WrenSetErrorFn(ErrorFn);
    WrenSetBindMethodFn(BindMethodFn);
    WrenSetBindClassFn(BindClassFn);
    WrenSetLoadModuleFn(LoadModuleFn);
    WrenStartVM();
    WrenSetCallHandle();

    const char *link = "scripts/main.wren";

    if (!WrenInterpret(link))
    {
        LOG_ERROR("Failed to interpret main.wren");
        return false;
    }

    if (!WrenLoadMainHandles("main"))
    {
        LOG_ERROR("Failed to load Wren handles from 'main' module");
        return false;
    }

    if (!WrenCallOnStart())
    {
        LOG_ERROR("Failed to run Wren on_start");
        return false;
    }

    return true;
}

bool ReloadWrenScript(const char *filename)
{

    if (g_app.wren_manager.vm)
    {
        ShutdownScripts();
    }

    bool success = WrenInit();
    if (!success)
    {
        LOG_INFO("Impossible to reload Wren failed");
        return false;
    }
    LOG_INFO("Wren hot-reload completed successfully");
    return success;
}

static bool CheckWrenCallResult(WrenInterpretResult result, const char *method_name)
{
    if (result != WREN_RESULT_SUCCESS)
        LOG_ERROR("Error in %s", method_name);
    return result == WREN_RESULT_SUCCESS;
}

static void SafeReleaseHandle(WrenVM *vm, WrenHandle **handle)
{
    if (vm && handle && *handle)
    {
        wrenReleaseHandle(vm, *handle);
        *handle = NULL;
    }
}

bool WrenLoadMainHandles(const char *main_module_name)
{

    if (!wrenHasVariable(g_app.wren_manager.vm, main_module_name, "Main"))
    {
        LOG_ERROR("Class 'Main' not found in script module '%s'", main_module_name);
        return false;
    }

    wrenEnsureSlots(g_app.wren_manager.vm, 1);
    wrenGetVariable(g_app.wren_manager.vm, main_module_name, "Main", 0);

    g_app.wren_manager.main_class = wrenGetSlotHandle(g_app.wren_manager.vm, 0);
    g_app.wren_manager.on_start = wrenMakeCallHandle(g_app.wren_manager.vm, "on_start()");
    g_app.wren_manager.on_update = wrenMakeCallHandle(g_app.wren_manager.vm, "on_update(_)");
    g_app.wren_manager.on_fixed_update = wrenMakeCallHandle(g_app.wren_manager.vm, "on_fixed_update(_)");
    g_app.wren_manager.on_render = wrenMakeCallHandle(g_app.wren_manager.vm, "on_render()");
    g_app.wren_manager.on_destroy = wrenMakeCallHandle(g_app.wren_manager.vm, "on_destroy()");
    g_app.wren_manager.arity = wrenMakeCallHandle(g_app.wren_manager.vm, "arity");
    return true;
}

static bool CallMainNoArgHandle(WrenHandle *handle, const char *method_name)
{
    if (!g_app.wren_manager.main_class || !handle)
        return false;

    wrenEnsureSlots(g_app.wren_manager.vm, 1);
    wrenSetSlotHandle(g_app.wren_manager.vm, 0, g_app.wren_manager.main_class);
    return CheckWrenCallResult(wrenCall(g_app.wren_manager.vm, handle), method_name);
}

static bool CallMainDeltaHandle(WrenHandle *handle, float32 delta, const char *method_name)
{
    if (!g_app.wren_manager.main_class || !handle)
        return false;

    wrenEnsureSlots(g_app.wren_manager.vm, 2);
    wrenSetSlotHandle(g_app.wren_manager.vm, 0, g_app.wren_manager.main_class);
    wrenSetSlotDouble(g_app.wren_manager.vm, 1, (float64)delta);
    return CheckWrenCallResult(wrenCall(g_app.wren_manager.vm, handle), method_name);
}

bool WrenCallOnStart()
{
    return CallMainNoArgHandle(g_app.wren_manager.on_start, "on_start");
}

bool WrenCallOnUpdate(float32 delta)
{
    return CallMainDeltaHandle(g_app.wren_manager.on_update, delta, "on_update");
}

bool WrenCallOnFixedUpdate(float32 delta)
{
    return CallMainDeltaHandle(g_app.wren_manager.on_fixed_update, delta, "on_fixed_update");
}

bool WrenCallOnRender()
{
    return CallMainNoArgHandle(g_app.wren_manager.on_render, "on_render");
}

bool WrenCallOnDestroy()
{
    return CallMainNoArgHandle(g_app.wren_manager.on_destroy, "on_destroy");
}

void WrenFree()
{
    if (!g_app.wren_manager.vm)
        return;

    SafeReleaseHandle(g_app.wren_manager.vm, &g_app.wren_manager.main_class);
    SafeReleaseHandle(g_app.wren_manager.vm, &g_app.wren_manager.on_start);
    SafeReleaseHandle(g_app.wren_manager.vm, &g_app.wren_manager.on_update);
    SafeReleaseHandle(g_app.wren_manager.vm, &g_app.wren_manager.on_fixed_update);
    SafeReleaseHandle(g_app.wren_manager.vm, &g_app.wren_manager.on_render);
    SafeReleaseHandle(g_app.wren_manager.vm, &g_app.wren_manager.on_destroy);
    SafeReleaseHandle(g_app.wren_manager.vm, &g_app.wren_manager.arity);

    for (int i = 0; i < 4; i++)
    {
        if (g_app.wren_manager.registry.callbacks[i].is_registered)
        {
            SafeReleaseHandle(g_app.wren_manager.vm, &g_app.wren_manager.registry.callbacks[i].handle);
            g_app.wren_manager.registry.callbacks[i].is_registered = false;
        }
    }

    wrenFreeVM(g_app.wren_manager.vm);
    g_app.wren_manager.vm = NULL;
}

bool CallWrenCallback(int16 index, void *data, uint8 arg_count)
{
    if (UNLIKELY(index < 0 || index >= 4))
    {
        LOG_WARNING("Event Callback index %d doesn't exist, can't call the callback", index);
        return false;
    }

    WrenCallback *cb = &g_app.wren_manager.registry.callbacks[index];

    if (UNLIKELY(!cb->is_registered))
    {
        LOG_WARNING("Event Callback index %d is not registered, can't call the callback", index);
        return false;
    }

    if (UNLIKELY(cb->arity != arg_count))
    {
        LOG_WARNING("Event Callback index %d expects %d argument(s), got %d", index, cb->arity, arg_count);
        return false;
    }

    WrenVM *vm = g_app.wren_manager.vm;
    CallbackArg *args = (CallbackArg *)data;

    wrenEnsureSlots(vm, arg_count + 1);
    wrenSetSlotHandle(vm, 0, cb->handle);

    for (uint8 i = 0; i < arg_count; i++)
    {
        switch (args[i].type)
        {
        case CB_ARG_NUM:
            wrenSetSlotDouble(vm, i + 1, args[i].as.num);
            break;
        case CB_ARG_BOOL:
            wrenSetSlotBool(vm, i + 1, args[i].as.boolean);
            break;
        case CB_ARG_STRING:
            wrenSetSlotString(vm, i + 1, args[i].as.string);
            break;
        default:
            LOG_WARNING("Event Callback index %d: unknown arg type at position %d", index, i);
            return false;
        }
    }

    WrenInterpretResult result = wrenCall(vm, g_app.wren_manager.call[arg_count]);

    if (UNLIKELY(result != WREN_RESULT_SUCCESS))
    {
        LOG_WARNING("Event Callback index %d: runtime error during call", index);
        return false;
    }

    return true;
}
