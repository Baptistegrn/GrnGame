/*
 * Libération des ressources et structures du moteur.
 */

#ifndef LIBERER_H
#define LIBERER_H

#ifdef __cplusplus
extern "C" {
#endif

/* Fonction principale de libération */
void liberer_gs(void);

/* Fonctions de libération des sous-structures */
void liberer_timing(void);
void liberer_fenetre(void);
void liberer_entrees(void);
void liberer_fond(void);
void liberer_tableau_image(void);
void liberer_textures(void);
void liberer_sons(void);
void liberer_controllers(void);

/* Fonctions de libération internes */
void liberer_gestionnaire_son(void);
void liberer_gestionnaire_image(void);
void liberer_shaders(void);
void free_tab_images(void);

#ifdef __cplusplus
}
#endif

#endif /*LIBERER_H*/