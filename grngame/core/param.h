// Global engine parameters and tunables.

#pragma once

#include <math.h>

// Pixel coordinate mode:
// - 1: force integer-aligned drawing (useful for pixel-art rendering)
// - 0: allow floating-point positions
#define PIXEL_INT_ONLY 1

#if PIXEL_INT_ONLY == 1
#define PIXEL_ALIGN(x) floor(x)
#else
#define PIXEL_ALIGN(x) (x)
#endif

// Maximum allowed delta-time (seconds) for a single frame. Frames with a
// larger dt will be clamped to this value to avoid physics/logic instability.
#define FRAME_DT_MAX 0.25

// Fixed-update loop frequency (Hz). Use this for deterministic fixed updates
// (physics, fixed-step game logic).
#define FIXED_UPDATE_LOOP 60.0

// Fixed-update timestep in seconds (derived from FIXED_UPDATE_LOOP).
#define FIXED_DT (1.0 / FIXED_UPDATE_LOOP)

// RGB color used to clear the screen when a Wren script interpretation fails.
#define WREN_INTERPRET_FAILED 255, 0, 0

// Maximum number of simultaneously supported controllers/gamepads
#define MAX_CONTROLLERS 6

// Default dead zone applied to controller axes (range: 0.0 - 1.0).
#define DEFAULT_DEAD_ZONE 0.1f

// Number of seconds (in frames multiplier) between garbage collector runs.
// The GC is triggered every `fps * GARBAGE_COLLECTOR_TIME_TO_REFRESH` frames.
// Keep this reasonably high to avoid frequent pauses; 60 means roughly once
// per minute at 60 FPS.
#define GARBAGE_COLLECTOR_TIME_TO_REFRESH 60

// fps on web for rendering (because vsync doesnt work on web)
#define RENDER_UPDATE_WEB 60

// max filters can be use for sounds
#define MAX_FILTERS 4

// when you want to get a sound at specific position this is the tolerence
#define SOUND_POSITION_EPSILON 1.0f