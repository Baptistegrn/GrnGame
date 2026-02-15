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

/* Alloue les structures principales du gestionnaire */
Gestionnaire *allouer_structures(void);

/* Initialise le moteur avec les parametres de fenetre et rendu */
Gestionnaire *initialiser(int hauteur_univers, int largeur_univers, float fps, bool bande_noir,
                          const char *nom_fenetre);

/* Initialise la fenetre et le renderer SDL */
void initialiser_fenetre();

/* Configure la redirection des erreurs vers un fichier log */
void configurer_redirection_stderr(const char *chemin_log);

/* met les clefs de chiffrement a 0 */
void mettre_cle_vide();
#ifdef __cplusplus
}
#endif

#endif /* INITIALISER_H */
