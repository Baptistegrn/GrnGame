#include <SDL.h>
#include <SDL_image.h>
#include <math.h>
#include "main.h"


void normaliser_chemin(char* chemin)
{
    if (!chemin)
        return;

    for (char* p = chemin; *p; p++)
    {
        if (*p == '\\')
            *p = '/';
    }
}
