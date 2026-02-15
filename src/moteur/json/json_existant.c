#include "../../main.h"
#include <stdio.h>

/* retourne si un fichier existe */
bool fichier_existant(const char *lien) {
    FILE *f = fopen(lien, "r");
    if (f) {
        fclose(f);
        return true;
    }
    return false;
}