#pragma once

#ifdef __EMSCRIPTEN__

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

static inline EM_BOOL WebResumeAudioMouseCallback(int event_type, const EmscriptenMouseEvent *mouse_event,
                                                  void *user_data)
{
    (void)event_type;
    (void)mouse_event;
    (void)user_data;

    WebResumeAudioContext();
    return EM_TRUE;
}

static inline EM_BOOL WebResumeAudioKeyCallback(int event_type, const EmscriptenKeyboardEvent *keyboard_event,
                                                void *user_data)
{
    (void)event_type;
    (void)keyboard_event;
    (void)user_data;

    WebResumeAudioContext();
    return EM_TRUE;
}

static inline EM_BOOL WebResumeAudioTouchCallback(int event_type, const EmscriptenTouchEvent *touch_event,
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

#define WEB_LOOP(func) emscripten_set_main_loop_arg(func, NULL, 0, 1)

#else

#define WEB_LOOP(func)
#define WebInstallAudioUnlock()

#endif
