#include "grngame/bindings/wren/wren_get.h"
#include "grngame/core/app.h"
#include "grngame/core/init.h"
#include "grngame/dev/logging.h"
#include "wren.h"
#include <stdlib.h>

#if defined(GRN_EMBED_ASSETS) && __has_include("resources/embedded_assets.h")
#include "resources/embedded_assets.h"
#else
#define GRN_EMBED_ASSETS_MISSING
#endif

int main()
{
    if (!WrenEarlyInit())
        return 1;

    AppInfo app = {0};

    app.name = WrenGetString("config", "Config", "name");
    app.version = WrenGetString("config", "Config", "version");
    app.fps = WrenGetInt("config", "Config", "fps");
    app.window_width = WrenGetInt("config", "Config", "windowWidth");
    app.window_height = WrenGetInt("config", "Config", "windowHeight");
    app.window_universe_width = WrenGetInt("config", "Config", "universeWidth");
    app.window_universe_height = WrenGetInt("config", "Config", "universeHeight");
    app.window_resizable = WrenGetBool("config", "Config", "resizable");
    app.window_fullscreen = WrenGetBool("config", "Config", "fullscreen");
    app.window_maximised = WrenGetBool("config", "Config", "maximised");
    app.enable_logs = WrenGetBool("config", "Config", "enableLogs");
    app.log_destination = WrenGetInt("config", "Config", "logDestination");
    app.force_universe_scale = WrenGetBool("config", "Config", "forceUniverseScale");
    app.render_clear = WrenGetInt("config", "Config", "renderClear");
    app.asset_folder = "assets";

#ifndef GRN_EMBED_ASSETS_MISSING
    app.embedded_assets_data = g_embedded_assets;
#else
    app.embedded_assets_data = NULL;
#endif

    EngineStart(&app);

    return 0;
}