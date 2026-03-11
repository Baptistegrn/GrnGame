#pragma once
#include "../math/types.h"
#include <SDL3/SDL_render.h>

typedef struct
{
    SDL_Renderer *renderer;
} Renderer;

bool RendererTryCreate(SDL_Window *window, Renderer *renderer);

void RendererClear(const Renderer *renderer);
void RendererPresent(const Renderer *renderer);
void RendererSetColor(uint8 r, uint8 g, uint8 b, uint8 a);
void RendererFillRect(const SDL_FRect *rect);
void RendererFillRects(const SDL_FRect *rects, int count);
void RendererRect(const SDL_FRect *rect);
void RendererTextureRotated(SDL_Texture *texture, const SDL_FRect *src, const SDL_FRect *dst, float64 angle,
                            const SDL_FPoint *center, SDL_FlipMode flip);
void RendererSetTextureAlpha(SDL_Texture *texture, uint8 a);
bool OffScreen(float32 x, float32 y, float32 w, float32 h);