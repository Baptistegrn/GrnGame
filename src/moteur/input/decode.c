#include "../../main.h"

/*
 * Convertit un nom de touche (ex: "A", "SPACE", "ENTER") en SDL_Scancode.
 * Utilise des tableaux de recherche séparés par longueur pour optimiser la
 * vitesse.
 */
SDL_Scancode scancode_depuis_nom(const char *nom_non_normalise) {
    if (!nom_non_normalise)
        return SDL_SCANCODE_UNKNOWN;

    /* Normalisation du nom (doit être libéré après) */
    char *nom = normaliser_nom(nom_non_normalise);

    const ToucheNom *liste = NULL;
    size_t longueur = strlen(nom);

    /* Sélection du tableau de recherche selon la longueur du mot */
    if (longueur == 1) {
        liste = touches_1;
    } else if (longueur == 2) {
        liste = touches_2;
    } else if (longueur == 3) {
        liste = touches_3;
    } else {
        liste = touches_longues;
    }

    /* Recherche linéaire dans la liste sélectionnée */
    for (int i = 0; liste[i].nom; i++) {
        if (strcmp(nom, liste[i].nom) == 0) {
            SDL_Scancode code = liste[i].code;
            xfree(nom);
            return code;
        }
    }
    log_fmt(NiveauLogErreur, "name of key not found '%s'", nom);
    xfree(nom);

    return SDL_SCANCODE_UNKNOWN;
}

/*
 * Convertit un nom de bouton manette (ex: "A", "DPAD_UP") en
 * SDL_GameControllerButton.
 */
SDL_GameControllerButton bouton_manette_depuis_nom(const char *nom_non_normalise) {
    if (!nom_non_normalise)
        return SDL_CONTROLLER_BUTTON_INVALID;

    /* Normalisation du nom */
    char *nom = normaliser_nom(nom_non_normalise);

    /* Recherche linéaire dans la liste globale des boutons */
    for (int i = 0; boutons_manette[i].nom; i++) {
        if (strcmp(nom, boutons_manette[i].nom) == 0) {
            SDL_GameControllerButton bouton = boutons_manette[i].bouton;
            xfree(nom);
            return bouton;
        }
    }

    log_fmt(NiveauLogErreur, "name of controller button not found '%s'", nom);
    xfree(nom);

    return SDL_CONTROLLER_BUTTON_INVALID;
}
