#include "../main.h"
#include "../logging/logging.h"

#include <SDL_image.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *normaliser_nom(const char *src) {
  if (!src) {
    return NULL;
  }

  size_t taille = strlen(src);
  char *dst = malloc(sizeof(char) * (taille + 1));
  if (!dst) {
    log_fmt(NiveauLogErreur,
            "Erreur: Échec d'allocation mémoire pour la normalisation du nom");
    return NULL;
  }

  for (size_t i = 0; i < taille; i++) {
    dst[i] = tolower(src[i]);
  }
  dst[taille] = '\0';

  return dst;
}

SDL_Scancode scancode_depuis_nom(const char *nom_non_normalise) {
  char *nom = normaliser_nom(nom_non_normalise);
  if (!nom || strlen(nom) == 0) {
    free(nom);
    log_fmt(NiveauLogErreur, "Erreur: Nom de touche NULL ou vide");
    return SDL_SCANCODE_UNKNOWN;
  }

  const ToucheNom *liste = NULL;
  size_t longueur = strlen(nom);

  if (longueur == 1) {
    liste = touches_1;
  } else if (longueur == 2) {
    liste = touches_2;
  } else if (longueur == 3) {
    liste = touches_3;
  } else {
    liste = touches_longues;
  }

  for (int i = 0; liste[i].nom; i++) {
    if (strcmp(nom, liste[i].nom) == 0) {
      free(nom);
      return liste[i].code;
    }
  }

  log_fmt(NiveauLogErreur, "Erreur: Nom de touche inconnu '%s'", nom);
  free(nom);
  return SDL_SCANCODE_UNKNOWN;
}

SDL_GameControllerButton
bouton_manette_depuis_nom(const char *nom_non_normalise) {
  char *nom = normaliser_nom(nom_non_normalise);
  if (!nom || strlen(nom) == 0) {
    free(nom);
    log_fmt(NiveauLogErreur, "Erreur: Nom de bouton manette NULL ou vide");
    return SDL_CONTROLLER_BUTTON_INVALID;
  }

  for (int i = 0; boutons_manette[i].nom; i++) {
    if (strcmp(nom, boutons_manette[i].nom) == 0) {
      free(nom);
      return boutons_manette[i].bouton;
    }
  }

  log_fmt(NiveauLogErreur, "Erreur: Nom de bouton manette inconnu '%s'", nom);
  free(nom);
  return SDL_CONTROLLER_BUTTON_INVALID;
}

bool touche_juste_presse(Gestionnaire *jeu, const char *touche) {
  if (!jeu || !jeu->entrees || !touche) {
    return false;
  }

  SDL_Scancode sc = scancode_depuis_nom(touche);
  return (sc != SDL_SCANCODE_UNKNOWN) && jeu->entrees->keys_pressed[sc];
}

bool touche_enfoncee(Gestionnaire *jeu, const char *touche) {
  if (!jeu || !jeu->entrees || !touche) {
    return false;
  }

  SDL_Scancode sc = scancode_depuis_nom(touche);
  return (sc != SDL_SCANCODE_UNKNOWN) && jeu->entrees->keys[sc];
}

bool touche_mannette_juste_presse(Gestionnaire *jeu, const char *touche) {
  if (!jeu || !jeu->entrees || !touche) {
    return false;
  }

  SDL_GameControllerButton bt = bouton_manette_depuis_nom(touche);
  return (bt != SDL_CONTROLLER_BUTTON_INVALID) &&
         jeu->entrees->controller_pressed[bt];
}

bool touche_mannette_enfoncee(Gestionnaire *jeu, const char *touche) {
  if (!jeu || !jeu->entrees || !touche) {
    return false;
  }

  SDL_GameControllerButton bt = bouton_manette_depuis_nom(touche);
  return (bt != SDL_CONTROLLER_BUTTON_INVALID) && jeu->entrees->controller[bt];
}