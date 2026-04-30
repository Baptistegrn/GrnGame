#include "grngame/bindings/wren/file_module.hpp"
#include "grngame/bindings/wren/wren_engine.hpp"
#include "grngame/utils/file.h"

#include <cstdlib>
#include <cstring>

static void file_exists(WrenVM *vm)
{
    const char *path = wren_get<const char *>(vm, 1);
    wren_set<bool>(vm, 0, FileExist(path));
}

static void file_read(WrenVM *vm)
{
    const char *path = wren_get<const char *>(vm, 1);
    char *content = ReturnFileString(path);

    if (!content)
    {
        wrenSetSlotNull(vm, 0);
        return;
    }

    wren_set<const char *>(vm, 0, content);
    std::free(content);
}

static void file_write(WrenVM *vm)
{
    const char *path = wren_get<const char *>(vm, 1);
    const char *content = wren_get<const char *>(vm, 2);
    wren_set<bool>(vm, 0, WriteFileString(path, content, false));
}

static void file_append(WrenVM *vm)
{
    const char *path = wren_get<const char *>(vm, 1);
    const char *content = wren_get<const char *>(vm, 2);
    wren_set<bool>(vm, 0, WriteFileString(path, content, true));
}

WrenForeignClassMethods BindForeignClassCallbackFile(WrenVM *vm, const char *module, const char *class_name)
{
    (void)vm;
    (void)module;
    (void)class_name;
    return {nullptr, nullptr};
}

WrenForeignMethodFn BindForeignMethodCallbackFile(WrenVM *vm, const char *module, const char *class_name,
                                                  bool is_static, const char *signature)
{
    (void)vm;
    if (!module || !class_name || !signature)
        return nullptr;

    if (std::strcmp(module, "std/wren/utils/file") == 0)
    {
        if (std::strcmp(class_name, "File") == 0 && is_static)
        {
            if (std::strcmp(signature, "exists(_)") == 0)
                return file_exists;
            if (std::strcmp(signature, "read(_)") == 0)
                return file_read;
            if (std::strcmp(signature, "write(_,_)") == 0)
                return file_write;
            if (std::strcmp(signature, "append(_,_)") == 0)
                return file_append;
        }
    }

    return nullptr;
}