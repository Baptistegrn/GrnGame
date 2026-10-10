#include "grngame/core/app.h"
#include <stdio.h>

#if defined(GRNGAME_ANDROID)

int32 SDL_main(int argc, char **argv)
{
    EngineStart();
    return 0;
}

#elif defined(GRNGAME_IOS)

#include <SDL3/SDL_main.h>

int main(int argc, char *argv[])
{
    EngineStart();
    return 0;
}

#else

int32 main()
{
    EngineStart();
    return 0;
}

#endif