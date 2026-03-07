#include "grngame/core/app.h"
#include "grngame/dev/logging.h"

int main()
{
    AppInfo app;
    app.name = "Noob - The Factionless";
    app.version = "1.0.1";
    app.window_width = 300;
    app.window_height = 400;
    app.window_universe_height = 400;
    app.window_universe_width = 300;
    app.asset_folder = "assets";
    app.window_resizable = true;
    app.window_fullscreen = false;
    app.window_maximised = false;
    app.enable_logs = true;
    app.log_destination = LOG_TO_FILE;

    EngineStart(app);
}