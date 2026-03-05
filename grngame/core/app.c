#include "app.h"
#include "SDL3/SDL_video.h"
#include "grngame/core/init.h"
#include "grngame/core/window.h"

void StartEngine(AppInfo app_info) {
    InitAll(&app_info);
    g_app.window = WindowCreate(&app_info);
    g_app.info = app_info;
}

App g_app;