/*
 * Initialisation du moteur et gestion des ressources.
 */

#ifndef INITIALISER_H
#define INITIALISER_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Gestionnaire Gestionnaire;

Gestionnaire *allouer_structures(void);

Gestionnaire *initialiser(int hauteur_univers, int largeur_univers, float fps, bool bande_noir,
                          const char *nom_fenetre);

void initialiser_fenetre();

void configurer_redirection_stderr(const char *chemin_log);

#ifdef __cplusplus
}
#endif

#endif
