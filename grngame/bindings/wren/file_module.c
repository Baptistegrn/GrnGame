#include "grngame/bindings/wren/wren_bind.h"
#include "grngame/bindings/wren/wren_callback.h"
#include "grngame/dev/logging.h"
#include "grngame/utils/file.h"
#include "wren.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

static bool warning = true;

static void warn_file_io()
{
    if (warning)
    {
        LOG_WARNING("FileSystem I/O is expensive, avoid using File methods in update() or draw().");
        warning = false;
    }
}

static void file_exists(WrenVM *vm)
{
    const char *path = wrenGetSlotString(vm, 1);
    wrenSetSlotBool(vm, 0, FileExist(path));
}

static void file_read(WrenVM *vm)
{
    warn_file_io();
    const char *path = wrenGetSlotString(vm, 1);
    char *content = ReturnFileString(path);

    if (!content)
    {
        wrenSetSlotNull(vm, 0);
        return;
    }

    wrenSetSlotString(vm, 0, content);
    free(content);
}

static void file_write(WrenVM *vm)
{
    warn_file_io();
    const char *path = wrenGetSlotString(vm, 1);
    const char *content = wrenGetSlotString(vm, 2);
    wrenSetSlotBool(vm, 0, WriteFileString(path, content, false));
}

static void file_append(WrenVM *vm)
{
    warn_file_io();
    const char *path = wrenGetSlotString(vm, 1);
    const char *content = wrenGetSlotString(vm, 2);
    wrenSetSlotBool(vm, 0, WriteFileString(path, content, true));
}

void RegisterFileModule()
{
    const char *module = "std/wren/utils/file";
    const char *cls = "File";
    bool is_static = true;

    RegisterMethod(module, cls, is_static, "exists(_)", file_exists);
    RegisterMethod(module, cls, is_static, "read(_)", file_read);
    RegisterMethod(module, cls, is_static, "write(_,_)", file_write);
    RegisterMethod(module, cls, is_static, "append(_,_)", file_append);
}