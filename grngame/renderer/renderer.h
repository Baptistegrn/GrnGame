#pragma once
#include "../math/types.h"
#include "grngame/utils/attributes.h"
#include <SDL3/SDL_render.h>

typedef struct
{
    SDL_Renderer *renderer;
} Renderer;

bool RendererTryCreate(SDL_Window *window, Renderer *renderer);

void RendererClear(const Renderer *renderer);
void RendererPresent(const Renderer *renderer);
void RendererSetColor(uint8 r, uint8 g, uint8 b, uint8 a);
void RendererFillRect(const SDL_FRect *RESTRICT rect);
void RendererFillRects(const SDL_FRect *RESTRICT rects, int count);
void RendererRect(const SDL_FRect *RESTRICT rect);
void RendererTextureRotated(SDL_Texture *RESTRICT texture, const SDL_FRect *RESTRICT src, const SDL_FRect *RESTRICT dst,
                            float64 angle, const SDL_FPoint *RESTRICT center, SDL_FlipMode flip);
void RendererSetTextureAlpha(SDL_Texture *RESTRICT texture, uint8 a);
bool OffScreen(float32 x, float32 y, float32 w, float32 h);
void SetRenderColor(uint8 r, uint8 g, uint8 b);