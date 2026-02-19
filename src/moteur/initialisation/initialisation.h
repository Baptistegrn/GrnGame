/*
 * Initialisation du moteur et gestion des ressources.
 */

#ifndef INITIALISATION_H
#define INITIALISATION_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Gestionnaire Gestionnaire;

/* initialise le gestionnaire */
void initialiser_gestionnaire(void);

/* initialise toute les structures */
void initialisation(void);

/* mettre des parametres */
void definir_taille(int largeur, int hauteur);
void definir_fps(float fps);
void definir_titre(const char *titre);
void definir_bandes_noires(bool bande_noir);

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
