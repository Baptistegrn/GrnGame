#include "grngame/core/app.h"
#include "grngame/core/init.h"
#include "grngame/dev/logging.h"

#if defined(GRN_EMBED_ASSETS) && __has_include("resources/embedded_assets.h")
#include "resources/embedded_assets.h"
#else
#define GRN_EMBED_ASSETS_MISSING
#endif

int main()
{
    AppInfo app;
    app.name = "Tests";
    app.version = "1.0.1";
    app.fps = 60;
    app.window_width = 640;
    app.window_height = 360;
    app.window_universe_height = 180;
    app.window_universe_width = 320;
    app.asset_folder = "assets"; // need to load at the origin for embedding use "assets" for not embedding option
    app.render_clear = 1;
#ifndef GRN_EMBED_ASSETS_MISSING
    app.embedded_assets_data = g_embedded_assets;
#else
    app.embedded_assets_data = NULL;
#endif
    app.window_resizable = true;
    app.window_fullscreen = false;
    app.window_maximised = false;
    app.enable_logs = true;
    app.log_destination = LOG_TO_CONSOLE;
    app.force_universe_scale = false;
    EngineStart(&app);

    return 0;
}