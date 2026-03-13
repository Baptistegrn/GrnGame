
#include "grngame/core/app.h"
#include "grngame/dev/logging.h"

int main()
{
    AppInfo app;
    app.name = "Noob - The Factionless";
    app.version = "1.0.1";
    app.fps = 80.0;
    app.window_width = 200;
    app.window_height = 200;
    app.window_universe_height = 200;
    app.window_universe_width = 200;
    app.asset_folder = "assets";
    app.window_resizable = true;
    app.window_fullscreen = false;
    app.window_maximised = false;
    app.enable_logs = true;
    app.log_destination = LOG_TO_CONSOLE;
    app.force_universe_scale = false;
    EngineStart(&app);
    return 0;
}