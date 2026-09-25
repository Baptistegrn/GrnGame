#pragma once
#include "app.h"
#include <soloud_c.h>
#include <stdbool.h>

typedef enum
{
    INIT_OK,
    INIT_SDL_FAILED,
    INIT_LOG_FAILED,
    INIT_SOUND_FAILED,
    INIT_CONFIG_FAILED,
    INIT_OPEN_GAME_DATA_FAILED,
    INIT_ALREADY, // already initialized
} InitResult;

InitResult InitAll();

InitResult InitAppConfig(void);
