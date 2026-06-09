// Global engine parameters and tunables.

#pragma once

#include <math.h>

// Pixel coordinate mode:
// - 1: force integer-aligned drawing (useful for pixel-art rendering)
// - 0: allow floating-point positions
#define PIXEL_INT_ONLY 1

#if PIXEL_INT_ONLY
#define PIXEL_ALIGN(x) floor(x)
#else
#define PIXEL_ALIGN(x) (x)
#endif

// Fixed-update loop frequency
#define FIXED_UPDATE_LOOP 60.0

#define FIXED_DT (1.0 / FIXED_UPDATE_LOOP)

// index color used to clear the screen when a Wren script interpretation fails.
#define WREN_INTERPRET_FAILED 7

// Maximum number of simultaneously supported controllers/gamepads
#define MAX_CONTROLLERS 6

// Default dead zone applied to controller axes (range: 0.0 - 1.0).
#define DEFAULT_DEAD_ZONE 0.1f

// Number of seconds (in frames multiplier) between garbage collector runs.
#define GARBAGE_COLLECTOR_TIME_TO_REFRESH 60

// fps on web for rendering (because vsync doesnt work on web)
#define RENDER_UPDATE_WEB 60

// max filters can be use for sounds
#define MAX_FILTERS 4

// when you want to get a sound at specific position this is the tolerence
#define SOUND_POSITION_EPSILON 1.0f

// name of the module of the programm
#define MODULE_WREN_NAME "Main"

// path of the main entry of the script
#define WREN_SCRIPT "scripts/main.wren"