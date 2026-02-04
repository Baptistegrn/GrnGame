/*
 * Rendu de texte via les feuilles de caracteres.
 */

#include "../../../main.h"

/* Ajoute un caractere individuel (texture) au tableau de rendu */
static Sint16 ajouter_char_dans_tableau(const char *lien_image, char lettre, float posx, float posy,
                                        unsigned char coeff, bool sens, Uint16 rotationP,
                                        Uint16 rotation, Uint8 a) {
    if (!gs)
        goto gsvide;
    char lien_image_lettre[TAILLE_LIEN];

    snprintf(lien_image_lettre, TAILLE_LIEN, "%s/%d.png", lien_image, (unsigned char)lettre);

    normaliser_chemin(lien_image_lettre);

    SDL_Texture *texture = recuperer_texture_par_lien(lien_image_lettre);

    if (!texture) {
        log_fmt(NiveauLogAvertissement, "Texture not found '%c' (%d)", lettre,
                (unsigned char)lettre);
        return 0;
    }

    int tex_w = 0, tex_h = 0;
    if (SDL_QueryTexture(texture, NULL, NULL, &tex_w, &tex_h) != 0) {
        return 0;
    }

    Sint16 largeur_finale = (Sint16)(tex_w * coeff);
    Sint16 hauteur_finale = (Sint16)(tex_h * coeff);

    ajouter_image_au_tableau(lien_image_lettre, posx, posy, largeur_finale, hauteur_finale, sens,
                             rotationP, rotation, a);

    return largeur_finale;

gsvide:
    log_message(NiveauLogDebug, "manager is empty in add char to table function");
    return 0;
}

/* Affiche un mot complet en assemblant les caracteres individuels */
void ajouter_mot_dans_tableau(const char *chemin, const char *mot, float posx, float posy,
                              unsigned char coeff, bool sens, Sint16 ecart, Uint16 rotationP,
                              Uint16 rotation, Uint8 a) {
    if (!gs)
        goto gsvide;
    int taille_chaine = strlen(mot);
    Sint16 position_courante = 0;

    for (int i = 0; i < taille_chaine; i++) {
        Sint16 largeur = ajouter_char_dans_tableau(chemin, mot[i], posx + (float)position_courante,
                                                   posy, coeff, sens, rotationP, rotation, a);

        position_courante += largeur + ecart;
    }

    return;

gsvide:
    log_message(NiveauLogDebug, "manager is empty in add word to table function");
}