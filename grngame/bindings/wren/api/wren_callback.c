#include "grngame/bindings/wren/wren_api.h"
#include "grngame/core/app.h"
#include "grngame/dev/logging.h"
#include "grngame/utils/attributes.h"
#include <grngame/utils/string_compat.h>
#include <stdbool.h>

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

void WrenSetCallHandle()
{
    for (int i = 0; i <= 16; i++)
    {
        g_app.wren_manager.call[i] = wrenMakeCallHandle(g_app.wren_manager.vm, CALL_SIGNATURES[i]);
    }
}

static bool IndexCallBackCorrect(int16 index)
{
    if (UNLIKELY(index < 0 || index >= 4))
    {
        LOG_WARNING("Event Callback index %d doesn't exist, can't call the callback", index);
        return false;
    }
    return true;
}

static bool CallBackIsRegistered(int16 index)
{
    WrenCallback *cb = &g_app.wren_manager.registry.callbacks[index];
    if (UNLIKELY(!cb->is_registered))
    {
        LOG_WARNING("Event Callback index %d is not registered, can't call the callback", index);
        return false;
    }
    return true;
}

static bool CallBackArityCorrect(int16 index, uint8 arg_count)
{
    WrenCallback *cb = &g_app.wren_manager.registry.callbacks[index];
    if (UNLIKELY(cb->arity != arg_count))
    {
        LOG_WARNING("Event Callback index %d expects %d argument(s), got %d", index, cb->arity, arg_count);
        return false;
    }
    return true;
}

static CallbackArg *CallbackPrepare(int16 index, void *data, uint8 arg_count)
{
    WrenCallback *cb = &g_app.wren_manager.registry.callbacks[index];
    WrenVM *vm = g_app.wren_manager.vm;
    CallbackArg *args = (CallbackArg *)data;

    wrenEnsureSlots(vm, arg_count + 1);
    wrenSetSlotHandle(vm, 0, cb->handle);
    return args;
}

static bool WrenCallCallback(int16 index, uint8 arg_count)
{
    WrenVM *vm = g_app.wren_manager.vm;
    WrenInterpretResult result = wrenCall(vm, g_app.wren_manager.call[arg_count]);

    if (UNLIKELY(result != WREN_RESULT_SUCCESS))
    {
        LOG_WARNING("Event Callback index %d: runtime error during call", index);
        return false;
    }

    return true;
}

bool CallWrenCallback(int16 index, void *data, uint8 arg_count)
{
    if (!IndexCallBackCorrect(index))
        return false;

    if (!CallBackIsRegistered(index))
        return false;

    if (!CallBackArityCorrect(index, arg_count))
        return false;

    WrenVM *vm = g_app.wren_manager.vm;
    CallbackArg *args = CallbackPrepare(index, data, arg_count);

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

    return WrenCallCallback(index, arg_count);
}