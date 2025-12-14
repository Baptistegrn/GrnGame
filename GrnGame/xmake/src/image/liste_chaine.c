#include "../main.h"
#include "SDL_render.h"
#include <math.h>
#include <stdlib.h>


Liste *creer_liste(SDL_Texture *valeur, int angle) {
  Liste *nouv = malloc(sizeof(Liste));
  if (!nouv)
    return NULL;
  nouv->texture = valeur;
  nouv->angle = angle;
  nouv->suivant = NULL;
  return nouv;
}

Liste *ajouter_en_tete(Liste *liste, SDL_Texture *valeur, int angle) {
  Liste *nouv = creer_liste(valeur, angle);
  if (!nouv)
    return liste;
  nouv->suivant = liste;
  return nouv;
}

SDL_Texture *recuperer_texture_angle_indice(Liste *liste, int angle) {
  Liste *courant = liste;
  while (courant != NULL) {
    if (courant->angle == angle) {
      return courant->texture;
    }
    courant = courant->suivant;
  }
  return NULL;
}

Liste *liberer_liste(Liste *liste) {
  Liste *courant = liste;
  while (courant != NULL) {
    Liste *temp = courant;
    courant = courant->suivant;
    if (temp->texture) {
      SDL_DestroyTexture(temp->texture);
    }
    free(temp);
  }
  return NULL;
}