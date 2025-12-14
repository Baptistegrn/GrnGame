#include "../main.h"
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>


void ajouter_shaders(Gestionnaire *gt, int *posx, int *posy, int *r, int *g,
                     int *b, int *a, int taille) {
  if (!gt || !gt->rendu)
    return;
  if (gt->shader->init == false) {
    // On alloue la taille maximale possible (largeur * hauteur)
    size_t taille_max_octets = sizeof(int) * gt->largeur * gt->hauteur;

    gt->shader->posx = malloc(taille_max_octets);
    gt->shader->posy = malloc(taille_max_octets);
    gt->shader->r = malloc(taille_max_octets);
    gt->shader->g = malloc(taille_max_octets);
    gt->shader->b = malloc(taille_max_octets);
    gt->shader->a = malloc(taille_max_octets);
    memset(gt->shader->posx, 0, taille_max_octets);
    memset(gt->shader->posy, 0, taille_max_octets);
    memset(gt->shader->r, 0, taille_max_octets);
    memset(gt->shader->g, 0, taille_max_octets);
    memset(gt->shader->b, 0, taille_max_octets);
    memset(gt->shader->a, 0, taille_max_octets);
    gt->shader->init = true;
  }
  gt->shader->taille = taille;
  for (int i = 0; i < taille; i++) {
    gt->shader->posx[i] = posx[i];
    gt->shader->posy[i] = posy[i];
    gt->shader->r[i] = r[i];
    gt->shader->g[i] = g[i];
    gt->shader->b[i] = b[i];
    gt->shader->a[i] = a[i];
  }
}

void appliquer_shaders(Gestionnaire *gt) {
  if (!gt || !gt->rendu || !gt->shader_vitre)
    return;
  if (gt->shader->taille == 0 || !gt->shader->posx)
    return;

  void *pixels;
  int pitch;

  // Verrouillage de la texture
  if (SDL_LockTexture(gt->shader_vitre, NULL, &pixels, &pitch) != 0)
    return;

  memset(pixels, 0, gt->hauteur * pitch);

  Uint32 *buffer = (Uint32 *)pixels;
  int largeur_img = gt->largeur;
  int hauteur_img = gt->hauteur;

  int *posx = gt->shader->posx;
  int *posy = gt->shader->posy;
  int *r = gt->shader->r;
  int *g = gt->shader->g;
  int *b = gt->shader->b;
  int *a = gt->shader->a;
  int taille = gt->shader->taille;
  for (int i = 0; i < taille; i++) {
    int x = posx[i];
    int y = posy[i];

    if (x >= 0 && x < largeur_img && y >= 0 && y < hauteur_img && a[i] != 0) {
      int index = y * (pitch / 4) + x;
#if defined(_WIN32) || defined(__linux__) || defined(__APPLE__)
      Uint32 pixel_color = (r[i] << 24) | (g[i] << 16) | (b[i] << 8) | a[i];
#endif

      buffer[index] = pixel_color;
    }
  }

  SDL_UnlockTexture(gt->shader_vitre);

  int coeff = gt->largeur_actuel / gt->largeur;

  SDL_Rect dst;
  dst.x = gt->decalage_x;
  dst.y = gt->decalage_y;
  dst.w = gt->largeur * coeff;
  dst.h = gt->hauteur * coeff;

  SDL_RenderCopy(gt->rendu, gt->shader_vitre, NULL, &dst);

  gt->shader->taille = 0;
}