/*
 * Liberation des ressources du moteur.
 * La fonction principale liberer_gs() appelle les liberateurs de chaque module.
 */

#ifndef LIBERER_H
#define LIBERER_H

#ifdef __cplusplus
extern "C" {
#endif

/* Libere toutes les ressources du gestionnaire principal */
void liberer_gs(void);

#ifdef __cplusplus
}
#endif

#endif /* LIBERER_H */