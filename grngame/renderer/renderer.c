#include "renderer.h"

bool RendererTryCreate(SDL_Window *window, Renderer *renderer)
{
    renderer->renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer->renderer)
    {
        return false;
    }

    return true;
}

void RendererClear(const Renderer *renderer)
{
    SDL_RenderClear(renderer->renderer);
}

void RendererPresent(const Renderer *renderer)
{
    SDL_RenderPresent(renderer->renderer);
}