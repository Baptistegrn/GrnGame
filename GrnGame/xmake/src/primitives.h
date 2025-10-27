#pragma once

#include <stdint.h>
#include "SDL_render.h"
#ifdef __cplusplus
extern "C" {
#endif

void dessiner_rectangle(SDL_Renderer* renderer, int x, int y, int largeur, int hauteur, uint8_t r, uint8_t g, uint8_t b,
                        uint8_t a);

void dessiner_cercle(SDL_Renderer* renderer, int x, int y, int rayon, uint8_t r, uint8_t g, uint8_t b, uint8_t a);

#ifdef __cplusplus
}
#endif
