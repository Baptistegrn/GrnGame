#include "grngame/core/app.h"
#include "grngame/utils/types.h"
#include <stdio.h>

int main()
{
    AppInfo app;
    app.name = "Noob - The Factionless";
    app.version = "1.0.1";
    app.window_width = 800;
    app.window_height = 600;
    app.window_fullscreen = false;
    app.asset_folder = "assets";
    app.window_resizable = true;
    printf(" check float :%d,check double :%d", CheckFloat(), CheckDouble());
    EngineStart(app);
}