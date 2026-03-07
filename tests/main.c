#include "grngame/core/app.h"

int main() {
    AppInfo app;
    app.name = "Noob - The Factionless";
    app.version = "1.0.1";
    app.window_width = 800;
    app.window_height = 600;

    app.asset_folder = "assets";
    app.window_resizable = true;
    app.enable_logs = true;
    app.log_destination = LOG_TO_CONSOLE;

    EngineStart(app);
}