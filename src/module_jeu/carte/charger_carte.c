/*
 * Chargement de cartes depuis des fichiers texte.
 * Le format attendu est une grille de nombres separes par un caractere.
 */

#include "../../allouer/allouer.h"
#include "../../moteur/logging/logging.h"
#include "../../proprietes.h"
#include "carte.h"
#include <stdio.h>

/*
 * Charge une carte depuis un fichier texte.
 * Chaque nombre represente un type de bloc.
 * le caractere separation represente comment distinguer les blocs (ex: ' ' ou ',').
 * pas_x et pas_y definissent la taille de chaque bloc en pixels.
 * Les blocs avec le type element_vide sont ignores et pas inscrit comme blocks.
 */
Blocs *charger_carte(const char *chemin, Uint8 pas_x, Uint8 pas_y, char separation,
                     int element_vide) {
    FILE *fs = fopen(chemin, "r");
    if (!fs) {
        log_fmt(NiveauLogErreur, "error open file for load a map: %s\n", chemin);

        /* Retourne un bloc vide en cas d'erreur */
        Blocs *vide = malloc_gestion_echec_compteur(sizeof(Blocs));
        vide->capacity = 1;
        vide->size = 1;
        vide->tab = malloc_gestion_echec_compteur(sizeof(Bloc));
        vide->tab[0] = (Bloc){0, 0, pas_x, pas_y, element_vide};

        return vide;
    }

    char buffer[TAILLE_LIGNE];
    int i = 0;
    int x = 0;
    int y = 0;

    Blocs *ptr = malloc_gestion_echec_compteur(sizeof(Blocs));
    ptr->capacity = 50;
    ptr->size = 0;
    ptr->tab = malloc_gestion_echec_compteur(sizeof(Bloc) * ptr->capacity);

    int c;
    while ((c = fgetc(fs)) != EOF) {
        /* Separateur de blocs */
        if (c == separation) {
            buffer[i] = '\0';
            int type = atoi(buffer);

            if (type != element_vide) {
                Bloc actuel = (Bloc){x, y, pas_x, pas_y, type};

                if (ptr->size == ptr->capacity) {
                    ptr->capacity *= 2;
                    ptr->tab =
                        realloc_gestion_echec_compteur(ptr->tab, sizeof(Bloc) * ptr->capacity);
                }
                ptr->tab[ptr->size++] = actuel;
            }
            x += pas_x;
            i = 0;
        }
        /* Fin de ligne */
        else if (c == '\n') {
            buffer[i] = '\0';

            if (i > 0) {
                int type = atoi(buffer);
                if (type != element_vide) {
                    Bloc actuel = (Bloc){x, y, pas_x, pas_y, type};

                    if (ptr->size == ptr->capacity) {
                        ptr->capacity *= 2;
                        ptr->tab =
                            realloc_gestion_echec_compteur(ptr->tab, sizeof(Bloc) * ptr->capacity);
                    }
                    ptr->tab[ptr->size++] = actuel;
                }
            }

            x = 0;
            y += pas_y;
            i = 0;
        }
        /* Accumulation des caracteres */
        else {
            buffer[i++] = (char)c;
        }
    }

    /* Traitement du dernier bloc */
    if (i > 0) {
        buffer[i] = '\0';
        int type = atoi(buffer);

        if (type != element_vide) {
            Bloc actuel = (Bloc){x, y, pas_x, pas_y, type};

            if (ptr->size == ptr->capacity) {
                ptr->capacity *= 2;
                ptr->tab = realloc_gestion_echec_compteur(ptr->tab, sizeof(Bloc) * ptr->capacity);
            }
            ptr->tab[ptr->size++] = actuel;
        }
    }

    fclose(fs);
    return ptr;
}
