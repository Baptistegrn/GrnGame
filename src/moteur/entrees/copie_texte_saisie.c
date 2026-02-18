/* copie le texte dans un buffer c qui se renitialise de maniere automatique */
#include "../../main.h"
#include "../../proprietes.h"
#include "../logging/logging.h"
#include "entrees.h"
#include <string.h>

/* copie le texte vers le buffer de entrees */
void copie_texte_saisie(char *source) {
    if (!gs)
        goto gsvide;

    char *dest = gs->entrees->texte_ecrit;
    int taille_actuelle = gs->entrees->taille_texte_ecrit;
    int taille_max = TAILLE_TEXTE_SAISIE;
    int taille_source = strlen(source);

    /* si le copier coller depuis levent sdl trop long */
    if (taille_source >= taille_max - 1) {
        log_fmt(NiveauLogErreur, "Input text too long for buffer (%d vs %d), ignoring.",
                taille_source, taille_max);
        return;
    }

    /* si on peut ajouter du texte */
    if (taille_source + taille_actuelle < taille_max - 1) {
        for (int i = 0; i < taille_source; i++) {
            dest[i + taille_actuelle] = source[i];
        }
        taille_actuelle += taille_source;
    }
    /*si buffer plein on divise par deux on garde la deuxieme moitier */
    else {
        int moitie = taille_max / 2;
        for (int i = 0; i < moitie; i++) {
            dest[i] = dest[i + moitie];
        }
        taille_actuelle = moitie;

        if (taille_actuelle + taille_source < taille_max - 1) {
            for (int i = 0; i < taille_source; i++) {
                dest[i + taille_actuelle] = source[i];
            }
            taille_actuelle += taille_source;
        } else {
            int espace_restant = (taille_max - 1) - taille_actuelle;
            for (int i = 0; i < espace_restant; i++) {
                dest[i + taille_actuelle] = source[i];
            }
            taille_actuelle += espace_restant;
        }
    }

    dest[taille_actuelle] = '\0';
    gs->entrees->taille_texte_ecrit = taille_actuelle;
    return;

gsvide:
    log_message(NiveauLogErreur, "manager empty in text input function");
}

/* lit le texte saisie */
const char *lire_texte_saisie() {
    if (!gs) {
        return NULL;
    }
    return gs->entrees->texte_ecrit;
}