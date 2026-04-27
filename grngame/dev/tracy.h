#pragma once

#if defined(TRACY_ENABLE)
#include <tracy/TracyC.h>

#define PROFILE_FRAME_MARK() TracyCFrameMark
#define PROFILE_ZONE_START(ctx, name) TracyCZoneN(ctx, name, true)
#define PROFILE_ZONE_END(ctx) TracyCZoneEnd(ctx)

#else

#define PROFILE_FRAME_MARK() ((void)0)
#define PROFILE_ZONE_START(ctx, name) ((void)0)
#define PROFILE_ZONE_END(ctx) ((void)0)

#endif