/*
 * Rendu de texte via les feuilles de caracteres.
 */

#ifndef CARACTERE_H
#define CARACTERE_H
#include <SDL.h>
#include <stdbool.h>
#ifdef __cplusplus
extern "C" {
#endif

float ajouter_mot_dans_tableau(const char *chemin, const char *mot, float posx, float posy,
                               unsigned char coeff, bool sens, Sint16 ecart, Uint16 rotationP,
                               Uint16 rotation, Uint8 a);
#ifdef __cplusplus
}
#endif

#endif /* CARACTERE_H */