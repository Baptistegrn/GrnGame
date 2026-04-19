#pragma once

#include "../input/input_manager.h"
#include "../math/types.h"
#include "SDL3/SDL_render.h"
#include "grngame/assets/asset_manager.h"
#include "grngame/audio/sound_manager.h"
#include "grngame/bindings/dascript/da_script_bind.h"
#include "grngame/bindings/wren/wren_bind.h"
#include "grngame/dev/logging.h"
#include "grngame/renderer/renderer.h"

BEGIN_DECLARATIONS

typedef enum
{
    SCRIPT_LANGUAGE_DASCRIPT = 0,
    SCRIPT_LANGUAGE_WREN = 1
} ScriptLanguage;

typedef struct
{
    const EmbeddedAsset *embedded_assets;
    const char *name;
    const char *version;
    const char *asset_folder;
    uint16 fps;
    float64 dt;

    uint16 window_universe_width;
    uint16 window_universe_height;
    uint16 window_width;
    uint16 window_height;

    bool window_resizable;
    bool window_fullscreen;
    bool window_maximised;
    bool window_change;
    bool window_occlusion_culled;

    float32 offset_x;
    float32 offset_y;

    bool enable_logs;
    LogDestination log_destination;

    bool force_universe_scale; // apply black stripes to the window

    ScriptLanguage script_language;

    // for draw render
    uint8 r, g, b;
} AppInfo;

typedef struct
{
    SDL_Window *window;
    Renderer renderer;
    SoundManager sound_manager;
    AssetManager asset_manager;
    InputManager input_manager;
    DaScriptManager *da_script;
    WrenManager *wren;
    AppInfo info;
} App;

void EngineStart(const AppInfo *app);
void EngineStop();

extern App g_app;

END_DECLARATIONS