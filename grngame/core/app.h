#pragma once
#include "../input/input_manager.h"
#include "../math/types.h"
#include "SDL3/SDL_pixels.h"
#include "grngame/assets/asset_manager.h"
#include "grngame/audio/sound_manager.h"
#include "grngame/bindings/wren/wren_bind.h"
#include "grngame/dev/logging.h"
#include "grngame/renderer/renderer.h"
#include "kvec.h"

BEGIN_DECLARATIONS

typedef struct
{
    const EmbeddedAsset *embedded_assets;
    const char *name;
    const char *version;
    const char *asset_folder;
    uint16 fps;
    float64 dt;
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

    float32 offset_x;
    float32 offset_y;

    bool enable_logs;
    LogDestination log_destination;

    bool force_universe_scale; // apply black stripes to the window

    int render_clear;

    kvec_t(SDL_Color) palette_elements;
    kvec_t(int) palette_alpha;

} AppInfo;

typedef struct
{
    SDL_Window *window;
    Renderer renderer;
    SoundManager sound_manager;
    AssetManager asset_manager;
    InputManager input_manager;
    WrenManager *wren;
    AppInfo info;

} App;

void EngineStart(const AppInfo *app);
void EngineStop();
COLD void ShutdownScripts(void);
extern App g_app;

END_DECLARATIONS