#include "../logging/logging.h"
#include "../main.h"
#include <stdio.h>
#include <string.h>

// Note: Conversion de caractères ASCII en images (bitmap)

int ajouter_char_dans_tableau(Gestionnaire *gs, char *lien_image, char lettre,
                              int posx, int posy, int coeff, int sens,
                              int rotation, int alpha) {
  if (!gs || !lien_image) {
    log_fmt(NiveauLogErreur, "Erreur: Paramètre NULL pour caractère\n");
    return 0;
  }

  char lien_image_lettre[TAILLE_LIEN_GT];
  snprintf(lien_image_lettre, TAILLE_LIEN_GT, "%s/%d.png", lien_image,
           (unsigned char)lettre);
  normaliser_chemin(lien_image_lettre);

  SDL_Texture *texture =
      recuperer_texture_par_lien(gs->textures, lien_image_lettre);
  if (!texture) {
    log_fmt(NiveauLogAvertissement, "Erreur: Texture introuvable '%c' (%d)\n",
            lettre, (unsigned char)lettre);
    return 0;
  }

  int texW = 0, texH = 0;
  if (SDL_QueryTexture(texture, NULL, NULL, &texW, &texH) != 0) {
    return 0;
  }

  int largeur_finale = texW * coeff;
  int hauteur_finale = texH * coeff;

  ajouter_image_au_tableau(gs, lien_image_lettre, posx, posy, largeur_finale,
                           hauteur_finale, sens, rotation, alpha);

  return largeur_finale;
}

void ajouter_mot_dans_tableau(Gestionnaire *gs, char *chemin, const char *mot,
                              int posx, int posy, int coeff, int sens,
                              int ecart, int rotation, int alpha) {
  if (!gs || !chemin || !mot) {
    log_fmt(NiveauLogErreur, "Erreur: Paramètre NULL pour mot\n");
    return;
  }

  int taille_chaine = strlen(mot);
  int position_courante = 0.0f;

  for (int i = 0; i < taille_chaine; i++) {
    int largeur =
        ajouter_char_dans_tableau(gs, chemin, mot[i], posx + position_courante,
                                  posy, coeff, sens, rotation, alpha);
    position_courante += largeur + ecart;
  }
}