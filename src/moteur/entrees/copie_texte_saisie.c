/* copie le texte dans un buffer c qui se renitialise de maniere automatique */
#include "../../main.h"
#include "../../proprietes.h"
#include "../logging/logging.h"
#include "entrees.h"
#include <string.h>

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

    /* si la source est trop grande pour le buffer entier */
    if (taille_source >= taille_max - 1) {
        log_fmt(NiveauLogErreur, "Input text too long for buffer (%d vs %d), ignoring.",
                taille_source, taille_max);
        return;
    }

    /* si le texte depasse la taille max on vide tout et on recommence */
    if (taille_actuelle + taille_source >= taille_max - 1) {
        taille_actuelle = 0;
        dest[0] = '\0';
    }

    /* on copie le texte a la suite */
    for (int i = 0; i < taille_source; i++) {
        dest[i + taille_actuelle] = source[i];
    }
    taille_actuelle += taille_source;

    dest[taille_actuelle] = '\0';
    gs->entrees->taille_texte_ecrit = taille_actuelle;
    return;

gsvide:
    log_message(NiveauLogErreur, "manager empty in text input function");
}

/* vide le buffer de saisie manuellement */
void vider_texte_saisie() {
    if (!gs)
        goto gsvide;

    gs->entrees->texte_ecrit[0] = '\0';
    gs->entrees->taille_texte_ecrit = 0;
    return;

gsvide:
    log_message(NiveauLogErreur, "manager empty in clear text function");
}

/* lit le texte saisie */
const char *lire_texte_saisie() {
    if (!gs)
        goto gsvide;

    return gs->entrees->texte_ecrit;

gsvide:
    log_message(NiveauLogErreur, "manager empty in read text function");
    return NULL;
}