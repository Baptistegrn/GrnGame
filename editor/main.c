#include "grngame/core/app.h"
#include "grngame/dev/logging.h"
#include "grngame/utils/clear.h"

int main()
{
    AppInfo app;
    CLEAR(app, 0);
    app.name = "GrnGame editor";
    app.version = "1.0.0";
    app.fps = 40.0;
    app.window_universe_height = 320;
    app.window_universe_width = 180;
    app.window_height = 180 * 2;
    app.window_width = 320 * 2;
    app.asset_folder = "assets";
    app.window_resizable = true;
    app.window_fullscreen = false;
    app.window_maximised = true;
    app.enable_logs = true;
    app.log_destination = LOG_TO_CONSOLE;
    app.force_universe_scale = false;
    app.asset_db = NULL;
    EngineStart(&app);
    return 0;
}