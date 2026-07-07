#pragma once

#ifdef GRNGAME_WASM

#include "grngame/core/param.h"
#include <emscripten.h>
#include <emscripten/html5.h>

// clang-format off
EM_JS(void, WebResumeAudioContext, (), {
    var audioContext = null;

    if (typeof Module !== 'undefined' && Module['SDL'] && Module['SDL'].audioContext)
    {
        audioContext = Module['SDL'].audioContext;
    }
    else if (typeof Module !== 'undefined' && Module['audioContext'])
    {
        audioContext = Module['audioContext'];
    }
    else if (typeof SDL !== 'undefined' && SDL.audioContext)
    {
        audioContext = SDL.audioContext;
    }

    if (audioContext && audioContext.state === 'suspended')
    {
        audioContext.resume();
    }
});
// clang-format on

static inline EM_BOOL WebResumeAudioMouseCallback(int32 event_type, const EmscriptenMouseEvent *mouse_event,
                                                  void *user_data)
{
    (void)event_type;
    (void)mouse_event;
    (void)user_data;

    WebResumeAudioContext();
    return EM_TRUE;
}

static inline EM_BOOL WebResumeAudioKeyCallback(int32 event_type, const EmscriptenKeyboardEvent *keyboard_event,
                                                void *user_data)
{
    (void)event_type;
    (void)keyboard_event;
    (void)user_data;

    WebResumeAudioContext();
    return EM_TRUE;
}

static inline EM_BOOL WebResumeAudioTouchCallback(int32 event_type, const EmscriptenTouchEvent *touch_event,
                                                  void *user_data)
{
    (void)event_type;
    (void)touch_event;
    (void)user_data;

    WebResumeAudioContext();
    return EM_TRUE;
}

static inline void WebInstallAudioUnlock()
{
    emscripten_set_mousedown_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, NULL, true, WebResumeAudioMouseCallback);
    emscripten_set_keydown_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, NULL, true, WebResumeAudioKeyCallback);
    emscripten_set_touchstart_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, NULL, true, WebResumeAudioTouchCallback);
}

#define WEB_LOOP(func)                                                                                                 \
    do                                                                                                                 \
    {                                                                                                                  \
        /* on web loop render is set to fixed fps not vsync */                                                         \
        emscripten_set_main_loop_timing(EM_TIMING_SETTIMEOUT, (int32)(1000 / RENDER_UPDATE_WEB));                      \
        emscripten_set_main_loop_arg(func, NULL, 0, 1);                                                                \
    } while (0)

#else

#define WEB_LOOP(func)
#define WebInstallAudioUnlock()

#endif
