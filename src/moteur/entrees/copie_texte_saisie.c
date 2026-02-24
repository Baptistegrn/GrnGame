/*
 * Gestion du texte saisi.
 */

#include "../../allouer/allouer.h"
#include "../../main.h"
#include "../../prediction_branche.h"
#include "../logging/logging.h"
#include "entrees.h"
#include <string.h>

/* Realloue automatiquement le buffer de texte si plein */
static void reallouer_si_plein(int nouvelle_taille) {
    if (UNLIKELY(!gs))
        goto gsvide;

    GestionnaireEntrees *e = gs->entrees;
    if (e->taille_texte_ecrit + nouvelle_taille >= e->capacite_texte_ecrit) {
        while (e->taille_texte_ecrit + nouvelle_taille >= e->capacite_texte_ecrit) {
            e->capacite_texte_ecrit *= 2;
        }
        e->texte_ecrit = realloc_gestion_echec_compteur(e->texte_ecrit, e->capacite_texte_ecrit);
    }
    return;

gsvide:
    log_message(NiveauLogErreur, "manager empty in text input reallocation");
    return;
}

/* Copie le texte vers le buffer d'entrees */
void copie_texte_saisie(char *source) {
    if (UNLIKELY(!gs))
        goto gsvide;

    GestionnaireEntrees *e = gs->entrees;
    int taille_source = (int)strlen(source);

    // Reallouer si necessaire
    reallouer_si_plein(taille_source + 1);

    memcpy(e->texte_ecrit + e->taille_texte_ecrit, source, taille_source);
    e->taille_texte_ecrit += taille_source;

    e->texte_ecrit[e->taille_texte_ecrit] = '\0';
    return;

gsvide:
    log_message(NiveauLogErreur, "manager empty in text input function");
    return;
}

/* vide le buffer de saisie (ne libere pas la memoire pour reutilisation) */
void vider_texte_saisie() {
    if (UNLIKELY(!gs))
        goto gsvide;

    if (gs->entrees && gs->entrees->texte_ecrit)
        gs->entrees->texte_ecrit[0] = '\0';
    if (gs->entrees)
        gs->entrees->taille_texte_ecrit = 0;
    return;

gsvide:
    log_message(NiveauLogErreur, "manager empty in clear text function");
}

/* lit le texte saisi (peut retourner NULL si le manager est vide) */
const char *lire_texte_saisie() {
    if (UNLIKELY(!gs))
        goto gsvide;

    return gs->entrees ? gs->entrees->texte_ecrit : NULL;

gsvide:
    log_message(NiveauLogErreur, "manager empty in read text function");
    return NULL;
}