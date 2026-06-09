#pragma once
#include "app.h"
#include <soloud_c.h>
#include <stdbool.h>

// later this function will grow when we will need to initialize more (stuff related to logging, app icon...),
// so the arguments are given in a struct and the result is as an enum

typedef enum
{
    INIT_OK,
    INIT_SDL_FAILED,
    INIT_LOG_FAILED,
    INIT_ALREADY, // already initialized
} InitResult;

InitResult InitAll(const AppInfo *app_info);
void InitializeAppState(const AppInfo *app_info);
void InitializeManagers();
void InitializeAssets(const AppInfo *app_info);
void InitializeScripts(void);
void CreateHashFromEmbeddedAssets(const AppInfo *app_info);