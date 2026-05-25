#include "wren_callback.h"
#include "grngame/core/app.h"
#include "grngame/dev/logging.h"
#include "grngame/platform/paths.h"
#include "grngame/utils/file.h"
#include "khash.h"
#include "wren.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

KHASH_MAP_INIT_STR(wren_method_map, WrenForeignMethodFn)
KHASH_MAP_INIT_STR(wren_class_map, WrenForeignClassMethods)

static khash_t(wren_method_map) *g_methods = NULL;
static khash_t(wren_class_map) *g_classes = NULL;

// key : "module:class:static:signature"
static void MakeMethodKey(char *buffer, size_t max, const char *module, const char *className, bool isStatic,
                          const char *signature)
{
    snprintf(buffer, max, "%s:%s:%s:%s", module, className, isStatic ? "s" : "i", signature);
}

// key : "module:class"
static void MakeClassKey(char *buffer, size_t max, const char *module, const char *className)
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

    int ret;
    khint_t k = kh_put(wren_method_map, g_methods, strdup(key), &ret);
    kh_value(g_methods, k) = fn;
}

void RegisterClass(const char *module, const char *className, WrenForeignMethodFn allocateFn,
                   WrenFinalizerFn finalizeFn)
{
    char key[KEY_SIZE];
    MakeClassKey(key, sizeof(key), module, className);

    int ret;
    khint_t k = kh_put(wren_class_map, g_classes, strdup(key), &ret);

    WrenForeignClassMethods methods;
    methods.allocate = allocateFn;
    methods.finalize = finalizeFn;

    kh_value(g_classes, k) = methods;
}

void WriteFn(WrenVM *vm, const char *text)
{
    LOG_INFO("%s", text);
}

void ErrorFn(WrenVM *vm, WrenErrorType errorType, const char *module, int line, const char *msg)
{
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
    if (result.source)
    {
        free((void *)result.source);
    }
}

WrenLoadModuleResult LoadModuleFn(WrenVM *vm, const char *name)
{
    WrenLoadModuleResult result = {0};
    result.onComplete = LoadModuleComplete;
    result.source = NULL;

    char filename[MODULE_SIZE_MAX_NAME];
    snprintf(filename, sizeof(filename), "%s.wren", name);

    if (g_app.info.embedded_assets)
    {
        // TODO: O(n) -> O(1)
        for (int i = 0; g_app.info.embedded_assets[i].name != NULL; i++)
        {
            if (strcmp(filename, g_app.info.embedded_assets[i].name) == 0)
            {
                result.source = strdup((const char *)g_app.info.embedded_assets[i].data);
                return result;
            }
        }
        LOG_ERROR("Wren Import Error: Failed to find module '%s'", filename);
    }
    else
    {
        char *file_full_link = FindFilePathFromName(filename);
        if (file_full_link)
        {
            result.source = ReturnFileString(file_full_link);
            free(file_full_link);
        }
        else
        {
            LOG_ERROR("Wren Import Error: Failed to find module '%s'", filename);
        }
        return result;
    }

    return result;
}