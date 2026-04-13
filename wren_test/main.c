#include "grngame/core/app.h"
#include "grngame/dev/logging.h"
#include <stdio.h>

#if defined(GRN_EMBED_ASSETS) && __has_include("../build/generated/embedded_assets.h")
#include "../build/generated/embedded_assets.h"
#else
#define GRN_EMBED_ASSETS_MISSING
#endif

int main()
{
    AppInfo app;
    app.name = "Tests";
    app.version = "1.0.1";
    app.fps = 500;
    app.window_width = 200;
    app.window_height = 200;
    app.window_universe_height = 200;
    app.window_universe_width = 200;
    app.asset_folder =
        "test_game/assets"; // need to load at the origin for embedding use "assets" for not embedding option
#ifndef GRN_EMBED_ASSETS_MISSING
    app.embedded_assets = g_embedded_assets;
#else
    app.embedded_assets = NULL;
#endif
    app.window_resizable = true;
    app.window_fullscreen = false;
    app.window_maximised = false;
    app.enable_logs = true;
    app.log_destination = LOG_TO_CONSOLE;
    app.force_universe_scale = true;
    app.script_language = SCRIPT_LANGUAGE_WREN;
    EngineStart(&app);
    return 0;
}