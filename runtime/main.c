#include "grngame/core/app.h"
#include <stdio.h>

#ifndef GRNGAME_ANDROID

int32 main()
{
    EngineStart();
    return 0;
}

#else

int32 SDL_main(int argc, char **argv)
{
    EngineStart();
    return 0;
}

#endif
