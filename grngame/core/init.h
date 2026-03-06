#pragma once
#include "app.h"
#include <stdbool.h>
#include <soloud_c.h>

// later this function will grow when we will need to initialize more (stuff related to logging, app icon...),
// so the arguments are given in a struct and the result is as an enum

typedef enum
{
    INIT_OK,
    INIT_SDL_FAILED,
    INIT_ALREADY, // already initialized
} InitResult;

InitResult InitAll(const AppInfo *app_info);