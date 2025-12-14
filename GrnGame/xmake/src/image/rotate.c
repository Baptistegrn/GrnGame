#include "../main.h"
#include <math.h>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void ajuster_rect_texture_tournee(SDL_Texture *texture, SDL_Rect *dst,
                                  int x_ecran, int y_ecran, int w_ecran,
                                  int h_ecran, int coeff) {
  int final_w, final_h;
  SDL_QueryTexture(texture, NULL, NULL, &final_w, &final_h);
  dst->w = final_w * coeff;
  dst->h = final_h * coeff;
  dst->x = x_ecran - (dst->w - w_ecran) / 2;
  dst->y = y_ecran - (dst->h - h_ecran) / 2;
}

SDL_Texture *creer_texture_tournee_pixelart(SDL_Renderer *rendu,
                                            SDL_Texture *src, int w, int h,
                                            double angle_deg,
                                            SDL_RendererFlip flip) {
  if (!src || !rendu)
    return NULL;

  double angle_rad = angle_deg * M_PI / 180.0;
  double cos_a = cos(angle_rad);
  double sin_a = sin(angle_rad);

  int new_w = (int)ceil(fabs(w * cos_a) + fabs(h * sin_a));
  int new_h = (int)ceil(fabs(w * sin_a) + fabs(h * cos_a));

  SDL_Texture *temp_target = SDL_CreateTexture(rendu, SDL_PIXELFORMAT_RGBA8888,
                                               SDL_TEXTUREACCESS_TARGET, w, h);
  if (!temp_target)
    return NULL;

  SDL_SetRenderTarget(rendu, temp_target);
  SDL_RenderCopy(rendu, src, NULL, NULL);
  SDL_SetRenderTarget(rendu, NULL);

  Uint32 *pixels_src = (Uint32 *)malloc(w * h * sizeof(Uint32));
  if (!pixels_src) {
    SDL_DestroyTexture(temp_target);
    return NULL;
  }

  SDL_SetRenderTarget(rendu, temp_target);
  if (SDL_RenderReadPixels(rendu, NULL, SDL_PIXELFORMAT_RGBA8888, pixels_src,
                           w * sizeof(Uint32)) != 0) {
    free(pixels_src);
    SDL_DestroyTexture(temp_target);
    SDL_SetRenderTarget(rendu, NULL);
    return NULL;
  }
  SDL_SetRenderTarget(rendu, NULL);
  SDL_DestroyTexture(temp_target);

  Uint32 *pixels_dst = (Uint32 *)calloc(new_w * new_h, sizeof(Uint32));
  if (!pixels_dst) {
    free(pixels_src);
    return NULL;
  }

  double cx_src = w / 2.0;
  double cy_src = h / 2.0;
  double cx_dst = new_w / 2.0;
  double cy_dst = new_h / 2.0;

  for (int y_dst = 0; y_dst < new_h; y_dst++) {
    for (int x_dst = 0; x_dst < new_w; x_dst++) {
      double dx = x_dst - cx_dst;
      double dy = y_dst - cy_dst;

      double x_src = dx * cos_a + dy * sin_a + cx_src;
      double y_src = -dx * sin_a + dy * cos_a + cy_src;

      if (flip == SDL_FLIP_HORIZONTAL) {
        x_src = w - 1 - x_src;
      }

      int ix = (int)round(x_src);
      int iy = (int)round(y_src);

      if (ix >= 0 && ix < w && iy >= 0 && iy < h) {
        pixels_dst[y_dst * new_w + x_dst] = pixels_src[iy * w + ix];
      }
    }
  }
  SDL_Texture *texture_tournee = SDL_CreateTexture(
      rendu, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, new_w, new_h);
  if (texture_tournee) {
    SDL_SetTextureBlendMode(texture_tournee, SDL_BLENDMODE_BLEND);
    SDL_SetTextureScaleMode(texture_tournee, SDL_ScaleModeNearest);
    SDL_UpdateTexture(texture_tournee, NULL, pixels_dst,
                      new_w * sizeof(Uint32));
  }

  free(pixels_src);
  free(pixels_dst);

  return texture_tournee;
}

SDL_Texture *creer_texture_tournee_rapide(SDL_Renderer *rendu, SDL_Texture *src,
                                          int w, int h, double angle_deg,
                                          SDL_RendererFlip flip) {
  int angle_norm = ((int)round(angle_deg) % 360 + 360) % 360;

  if (angle_norm % 90 == 0) {
    int rotation_90 = angle_norm / 90;

    SDL_Texture *temp_target = SDL_CreateTexture(
        rendu, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);
    if (!temp_target)
      return NULL;

    SDL_SetRenderTarget(rendu, temp_target);
    SDL_RenderCopy(rendu, src, NULL, NULL);
    SDL_SetRenderTarget(rendu, NULL);

    Uint32 *pixels_src = (Uint32 *)malloc(w * h * sizeof(Uint32));
    if (!pixels_src) {
      SDL_DestroyTexture(temp_target);
      return NULL;
    }

    SDL_SetRenderTarget(rendu, temp_target);
    SDL_RenderReadPixels(rendu, NULL, SDL_PIXELFORMAT_RGBA8888, pixels_src,
                         w * sizeof(Uint32));
    SDL_SetRenderTarget(rendu, NULL);
    SDL_DestroyTexture(temp_target);

    int new_w = (rotation_90 % 2 == 0) ? w : h;
    int new_h = (rotation_90 % 2 == 0) ? h : w;

    Uint32 *pixels_dst = (Uint32 *)malloc(new_w * new_h * sizeof(Uint32));
    if (!pixels_dst) {
      free(pixels_src);
      return NULL;
    }

    for (int y = 0; y < h; y++) {
      for (int x = 0; x < w; x++) {
        int src_idx = y * w + x;
        int dst_x, dst_y;
        int fx = (flip == SDL_FLIP_HORIZONTAL) ? (w - 1 - x) : x;

        switch (rotation_90) {
        case 0:
          dst_x = fx;
          dst_y = y;
          break;
        case 1:
          dst_x = h - 1 - y;
          dst_y = fx;
          break;
        case 2:
          dst_x = w - 1 - fx;
          dst_y = h - 1 - y;
          break;
        case 3:
          dst_x = y;
          dst_y = w - 1 - fx;
          break;
        }
        pixels_dst[dst_y * new_w + dst_x] = pixels_src[src_idx];
      }
    }

    SDL_Texture *texture_tournee =
        SDL_CreateTexture(rendu, SDL_PIXELFORMAT_RGBA8888,
                          SDL_TEXTUREACCESS_STATIC, new_w, new_h);
    if (texture_tournee) {
      SDL_SetTextureBlendMode(texture_tournee, SDL_BLENDMODE_BLEND);
      SDL_SetTextureScaleMode(texture_tournee, SDL_ScaleModeNearest);
      SDL_UpdateTexture(texture_tournee, NULL, pixels_dst,
                        new_w * sizeof(Uint32));
    }

    free(pixels_src);
    free(pixels_dst);
    return texture_tournee;
  }
  return creer_texture_tournee_pixelart(rendu, src, w, h, angle_deg, flip);
}