/*
 * Rotation de textures avec cache.
 */

#ifndef ROTATE_H
#define ROTATE_H

#include <SDL.h>

#ifdef __cplusplus
extern "C" {
#endif

SDL_Texture *creer_texture_angle(SDL_Texture *src, Uint16 w, Uint16 h, Uint16 angle,
                                 SDL_RendererFlip flip);

void ajuster_rect_texture_tournee(SDL_Texture *texture, SDL_Rect *dst, int x_ecran, int y_ecran,
                                  int w_ecran, int h_ecran, int coeff);

#ifdef __cplusplus
}
#endif

#endif