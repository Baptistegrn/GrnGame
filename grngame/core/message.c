#include "SDL3/SDL.h"
#include "SDL3/SDL_messagebox.h"
#include "grngame/core/app.h"
#include "grngame/math/types.h"

int32 MessageBoxWindow(const char *text)
{
    return SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "GrnGame", text, g_app.window);
}

int32 MessageBox_(const char *text)
{
    return SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "GrnGame", text, NULL);
}
