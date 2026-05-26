#include "grngame/core/app.h"
#include <SDL3/SDL.h>

void RemoveTaskBarIconColor()
{
    SDL_SetWindowProgressState(g_app.window, SDL_PROGRESS_STATE_NONE);
}

void SetTaskBarIconProgress(float32 progress)
{
    SDL_SetWindowProgressState(g_app.window, SDL_PROGRESS_STATE_NORMAL);
    SDL_SetWindowProgressValue(g_app.window, progress);
}

void SetTaskBarIconPausedProgress(float32 progress)
{
    SDL_SetWindowProgressState(g_app.window, SDL_PROGRESS_STATE_PAUSED);
    SDL_SetWindowProgressValue(g_app.window, progress);
}

void SetTaskBarIconErrorProgress(float32 progress)
{
    SDL_SetWindowProgressState(g_app.window, SDL_PROGRESS_STATE_ERROR);
    SDL_SetWindowProgressValue(g_app.window, progress);
}

void SetTaskBarIconIndeterminate()
{
    SDL_SetWindowProgressState(g_app.window, SDL_PROGRESS_STATE_INDETERMINATE);
}

SDL_ProgressState GetTaskBarIconState()
{
    return SDL_GetWindowProgressState(g_app.window);
}

float32 GetTaskBarIconProgress()
{
    return SDL_GetWindowProgressValue(g_app.window);
}