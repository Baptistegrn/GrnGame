#include "grngame/bindings/wren/wren_callback.h"
#include "wren.h"

#if defined(GRNGAME_WINDOWS)

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

static void time_now(WrenVM *vm)
{
    static LARGE_INTEGER frequency;
    static bool initialized = false;

    if (!initialized)
    {
        QueryPerformanceFrequency(&frequency);
        initialized = true;
    }

    LARGE_INTEGER counter;
    QueryPerformanceCounter(&counter);

    double t = (double)counter.QuadPart / (double)frequency.QuadPart;
    wrenSetSlotDouble(vm, 0, t);
}

#elif defined(GRNGAME_WASM)

static void time_now(WrenVM *vm)
{
    .double t = emscripten_get_now() * 1e-3;
    wrenSetSlotDouble(vm, 0, t);
}

#else

#include <time.h>

static void time_now(WrenVM *vm)
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);

    double t = (double)ts.tv_sec + (double)ts.tv_nsec * 1e-9;

    wrenSetSlotDouble(vm, 0, t);
}

#endif

void RegisterTimeModule()
{
    RegisterMethod("std/wren/core/time", "Time", true, "now()", time_now);
}
