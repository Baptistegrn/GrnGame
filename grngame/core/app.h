#pragma once
#include "../input/input_manager.h"
#include "../math/types.h"
#include "SDL3/SDL_pixels.h"
#include "grngame/assets/asset_manager.h"
#include "grngame/audio/sound_manager.h"
#include "grngame/bindings/wren/wren_bind.h"
#include "grngame/data/data.h"
#include "grngame/data/json.h"
#include "grngame/dev/hotreload.h"
#include "grngame/dev/logging.h"
#include "grngame/renderer/cielab.h"
#include "grngame/renderer/palette.h"
#include "grngame/renderer/renderer.h"
#include "khash.h"
#include "kvec.h"

BEGIN_DECLARATIONS

#if defined(GRNGAME_EMBED_ASSETS)
#define EMBEDDED_ASSETS_DATA_AVAILABLE
#endif

typedef struct AppInfo
{
    sqlite3 *asset_db;

    const char *name;
    const char *version;
    const char *asset_folder;

    uint16 fps;
    float32 dt;
    uint64 frame_count;

    uint16 window_universe_width;
    uint16 window_universe_height;
    uint16 window_width;
    uint16 window_height;

    bool window_resizable;
    bool window_fullscreen;
    bool window_maximised;
    bool window_change;
    bool window_occlusion_culled;
    bool bordered;

    float32 offset_x;
    float32 offset_y;

    bool enable_logs;
    LogDestination log_destination;

    bool force_universe_scale; // apply black stripes to the window

    int32 render_clear;

    string_vec_t palette;
} AppInfo;

typedef struct
{
    SDL_Window *window;
    Renderer renderer;

    SoundManager sound_manager;
    AssetManager asset_manager;
    InputManager input_manager;
    JsonManager json_manager;
    WrenManager *wren_manager;
    EmbeddedAssetManager embedded_asset_manager;
    PaletteManager palette_manager;

#if defined(GRNGAME_HOT_RELOAD_ENABLE)
    kvec_t(HotreloadQueueElement) queue;
#endif

    AppInfo info;

} App;

void EngineStart();
void EngineStop();
COLD void ShutdownScripts(void);
void ReloadConfig(void);

extern App g_app;

END_DECLARATIONS
