#pragma once
#include "app.h"
#include <stdbool.h>

// later this function will grow when we will need to initialize more (stuff related to logging, app icon...),
// so the arguments are given in a struct and the result is as an enum

typedef enum
{
    INIT_OK,
    INIT_SDL_FAILED,
    INIT_ALREADY, // already initialized
} InitResult;

/**
 * @brief Initializes every subsystem GrnGame needs initialized before it can start the engine,
 * (sdl, logging, ...)
 *
 * @param app_info Optional extra info about your app
 * @return InitResult INIT_OK if successfully initialized, else where the error happened
 */
InitResult InitAll(const AppInfo *app_info);