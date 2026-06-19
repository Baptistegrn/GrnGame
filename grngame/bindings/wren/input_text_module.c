#include "grngame/bindings/wren/wren_bind.h"
#include "grngame/bindings/wren/wren_callback.h"
#include "grngame/input/input_text.h"
#include "wren.h"


static void input_text_get(WrenVM *vm)
{
    wrenSetSlotString(vm, 0, InputTextGet());
}

static void input_text_clear(WrenVM *vm)
{
    InputTextClear();
}

static void input_drop_file(WrenVM *vm)
{
    const char *file = InputDropFile();
    wrenSetSlotString(vm, 0, file ? file : "");
}

static void input_text_put_text(WrenVM *vm)
{
    const char *text = wrenGetSlotString(vm, 1);
    InputTextPutText(text);
}

static void input_text_remove_text_int(WrenVM *vm)
{
    uint32 size = (uint32)wrenGetSlotDouble(vm, 1);
    InputTextRemoveTextInt(size);
}

static void input_text_remove_text_char(WrenVM *vm)
{
    const char *pattern = wrenGetSlotString(vm, 1);
    InputTextRemoveTextChar(pattern);
}

static void input_text_replace(WrenVM *vm)
{
    const char *pattern = wrenGetSlotString(vm, 1);
    const char *replace = wrenGetSlotString(vm, 2);

    InputTextReplace(pattern, replace);
}

void RegisterInputTextModule()
{
    const char *module = "std/wren/input/text";
    const char *cls = "InputText";
    bool is_static = true;

    RegisterMethod(module, cls, is_static, "get()", input_text_get);
    RegisterMethod(module, cls, is_static, "clear()", input_text_clear);
    RegisterMethod(module, cls, is_static, "drop_file()", input_drop_file);
    RegisterMethod(module, cls, is_static, "put_text(_)", input_text_put_text);
    RegisterMethod(module, cls, is_static, "remove_text_int(_)", input_text_remove_text_int);
    RegisterMethod(module, cls, is_static, "remove_text_char(_)", input_text_remove_text_char);
    RegisterMethod(module, cls, is_static, "replace(_,_)", input_text_replace);
}