
#include "../moteur/json/json.h"

/* charger un fichier sur le disque */
void loadJsonFile(const char *filename) { charger_fichier_json(filename); }

/* sauvegarder un fichier sur le disque */
void saveJsonFile(const char *filename) { sauvegarder_fichier_json(filename); }

/* supprimer un fichier du disque et de la memoire */
void deleteJsonFile(const char *filename) { supprimer_fichier_json(filename); }

/* ecrire un entier dans un fichier */
void writeIntToJson(const char *filename, const char *varName, int value) {
    ecrire_int_json(filename, varName, value);
}

/* ecrire un double dans un fichier */
void writeDoubleToJson(const char *filename, const char *varName, double value) {
    ecrire_double_json(filename, varName, value);
}

/* ecrire une chaine de caractere dans un fichier */
void writeStringToJson(const char *filename, const char *varName, const char *value) {
    ecrire_string_json(filename, varName, value);
}

/* recuperer un entier d'un fichier */
int readIntFromJson(const char *filename, const char *varName) {
    return recuperer_int_json(filename, varName);
}

/* recuperer un double d'un fichier */
double readDoubleFromJson(const char *filename, const char *varName) {
    return recuperer_double_json(filename, varName);
}

/* recuperer une chaine de caractere d'un fichier */
char *readStringFromJson(const char *filename, const char *varName) {
    return recuperer_string_json(filename, varName);
}

/* supprimer une variable d'un fichier */
void deleteVariableFromJson(const char *filename, const char *varName) {
    supprimer_variable_json(filename, varName);
}

/* mettre une valeur dans la cle de chiffrement */
void setJsonEncryptionKey(int index, int value) { mettre_fichiers_cle(index, value); }

/* mettre une valeur dans le vecteur d'initialisation */
void setJsonEncryptionIV(int index, int value) { mettre_fichiers_iv(index, value); }

/* recuperer le type d'une variable dans un fichier 0=INCONNU, 1=NOMBRE, 2=STRING */
int getJsonVariableType(const char *filename, const char *varName) {
    return recuperer_type_json(filename, varName);
}

/* teste si un fichier existe */
bool FileExists(const char *filename) { return fichier_existant(filename); }