/*
 * Rendu de texte via les feuilles de caracteres.
 */

#include "../../../allouer/allouer.h"
#include "../../../chemin/chemin.h"
#include "../../../main.h"
#include "../../../prediction_branche.h"
#include "../../logging/logging.h"
#include "../ajout/ajout.h"
#include "../chargement/chargement_image.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

/* Ajoute un caractere individuel (texture) au tableau de rendu */
static float ajouter_char_dans_tableau(const char *lien_image, char lettre, float posx, float posy,
                                       unsigned char coeff, bool sens, Uint16 rotationP,
                                       Uint16 rotation, Uint8 a) {
    if (UNLIKELY(!gs))
        goto gsvide;
    char lien_image_lettre[TAILLE_LIEN];

    snprintf(lien_image_lettre, TAILLE_LIEN, "%s/%d.png", lien_image, (unsigned char)lettre);

    normaliser_chemin(lien_image_lettre);

    SDL_Texture *texture = recuperer_texture_par_lien(lien_image_lettre);

    if (!texture) {
        log_fmt(NiveauLogAvertissement, "Texture not found '%c' (%d)", lettre,
                (unsigned char)lettre);
        return 0.0f;
    }

    int tex_w = 0, tex_h = 0;
    if (SDL_QueryTexture(texture, NULL, NULL, &tex_w, &tex_h) != 0) {
        return 0.0f;
    }

    float largeur_finale = (float)tex_w * (float)coeff;

    ajouter_image_au_tableau(lien_image_lettre, posx, posy, coeff, sens, rotationP, rotation, a);

    return largeur_finale;

gsvide:
    log_message(NiveauLogDebug, "manager is empty in add char to table function");
    return 0.0f;
}

/* Affiche un mot complet en assemblant les caracteres individuels */
float ajouter_mot_dans_tableau(const char *chemin, const char *mot, float posx, float posy,
                               unsigned char coeff, bool sens, Sint16 ecart, Uint16 rotationP,
                               Uint16 rotation, Uint8 a) {
    if (UNLIKELY(!gs))
        goto gsvide;
    int taille_chaine = strlen(mot);
    float position_courante = 0.0f;

    for (int i = 0; i < taille_chaine; i++) {
        float largeur = ajouter_char_dans_tableau(chemin, mot[i], posx + position_courante, posy,
                                                  coeff, sens, rotationP, rotation, a);

        position_courante += largeur + (float)ecart;
    }
    return position_courante;

gsvide:
    log_message(NiveauLogDebug, "manager is empty in add word to table function");
}