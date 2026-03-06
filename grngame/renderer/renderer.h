#pragma once
#include <SDL3/SDL_render.h>

typedef struct
{
    SDL_Renderer* renderer;
} Renderer;

bool RendererTryCreate(SDL_Window* window, Renderer* renderer);

void RendererClear(const Renderer * renderer);
void RendererPresent(const Renderer * renderer);