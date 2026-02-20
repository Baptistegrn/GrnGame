#ifndef JSON_H
#define JSON_H

#include "../../proprietes.h"
#include "cJSON.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Structure representant un fichier chargé en memoire */
typedef struct FichierJSON {
    char nom[TAILLE_LIEN];
    void *contenu;
    char *buffer_ecriture;
} FichierJSON;

/* Structure principale du gestionnaire de fichiers */
typedef struct GestionnaireFichiers {
    uint8_t cle[16]; /* cle de chiffrement AES */
    uint8_t iv[16];  /* vecteur d'initialisation AES */

    FichierJSON *liste; /* Tableau dynamique de fichiers */
    int nb_fichiers;    /* Nombre actuel de fichiers chargés */
    int capacite;       /* Capacité actuelle du tableau (pour realloc) */
} GestionnaireFichiers;

/* Trouve un fichier deja charge dans la liste */
FichierJSON *trouver_fichier_dans_liste(const char *nom);

/* renvoie le bon object cJSON selon le chemin */
cJSON *naviguer_vers_cible(cJSON *root, const char *chemin, char **ptr, bool creer_parent);

/* Alloue et initialise le gestionnaire */
void initialiser_gestionnaire_fichiers(void);

/* Libere tout (les fichiers, les contenus et le gestionnaire) */
void liberer_gestionnaire_fichiers(void);

/* Charge un fichier depuis le disque, le decrypte et l'ajoute au tableau */
void charger_fichier_json(const char *nom_fichier);

/* Sauvegarde un fichier specifique de la memoire vers le disque (avec cryptage) */
void sauvegarder_fichier_json(const char *nom_fichier);

/* Supprime le fichier du disque et de la memoire */
void supprimer_fichier_json(const char *nom_fichier);

/* ecrit un entier */
void ecrire_int_json(const char *nom_fichier, const char *nom_variable, int valeur);

/* ecrit un double */
void ecrire_double_json(const char *nom_fichier, const char *nom_variable, double valeur);

/* ecrit une chaine de caractere */
void ecrire_string_json(const char *nom_fichier, const char *nom_variable, const char *valeur);

/* recupere un entier */
int recuperer_int_json(const char *nom_fichier, const char *nom_variable);

/* recupere un double */
double recuperer_double_json(const char *nom_fichier, const char *nom_variable);

/* recupere une chaine de caractere */
char *recuperer_string_json(const char *nom_fichier, const char *nom_variable);

/* supprime une variable */
void supprimer_variable_json(const char *nom_fichier, const char *nom_variable);

/* cle */
void mettre_fichiers_cle(int index, int valeur);
void mettre_fichiers_iv(int index, int valeur);

/* retourne si un fichier existe */
bool fichier_existant(const char *lien);

/* Codes de retour pour les types */
#define JSON_TYPE_INCONNU 0
#define JSON_TYPE_NOMBRE 1
#define JSON_TYPE_STRING 2
#define JSON_TYPE_OBJECT 3

/* Recupere le type d'une variable */
int recuperer_type_json(const char *fichier_nom, const char *nom_variable);

/* liste tout les chemins a partir dun chemin de base et dun nom de fichier */
cJSON *lister_tous_les_chemins(const char *fichier_nom, const char *chemin);

#endif /* JSON_H */