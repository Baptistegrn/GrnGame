#include "../../main.h"

/* charge une carte */
Blocks *charger_carte(const char *chemin, Uint8 pas_x, Uint8 pas_y, char separation,
                      int element_vide) {
    FILE *fs = fopen(chemin, "r");
    if (!fs) {
        log_fmt(NiveauLogErreur, "erreur ouverture fichier pour charger une carte: %s\n", chemin);
        return NULL;
    }

    char buffer[TAILLE_LIGNE];
    int i = 0;

    int x = 0;
    int y = 0;

    Blocks *ptr = malloc(sizeof(Blocks));
    ptr->capacity = 50;
    ptr->size = 0;
    ptr->tab = malloc(sizeof(Block) * ptr->capacity);

    int c;
    while ((c = fgetc(fs)) != EOF) {

        /* separateur de blocs */
        if (c == separation) {
            buffer[i] = '\0';
            int type = atoi(buffer);
            /* on ajoute pas le block qui contient lelement a exclure */
            if (type != element_vide) {

                Block actuel = (Block){x, y, pas_x, pas_y, type};

                if (ptr->size == ptr->capacity) {
                    ptr->capacity *= 2;
                    ptr->tab = realloc(ptr->tab, sizeof(Block) * ptr->capacity);
                }

                ptr->tab[ptr->size] = actuel;
                ptr->size++;
            }
            x += pas_x;
            i = 0;
        }

        /* fin de ligne */
        else if (c == '\n') {
            buffer[i] = '\0';

            /* gere dernier nombre */
            if (i > 0) {
                int type = atoi(buffer);
                /* on ajoute pas le block qui contient lelement a exclure */
                if (type != element_vide) {
                    Block actuel = (Block){x, y, pas_x, pas_y, type};

                    if (ptr->size == ptr->capacity) {
                        ptr->capacity *= 2;
                        ptr->tab = realloc(ptr->tab, sizeof(Block) * ptr->capacity);
                    }

                    ptr->tab[ptr->size] = actuel;
                    ptr->size++;
                }
            }

            x = 0;
            y += pas_y;
            i = 0;
        }

        /* Accumulation des chiffres */
        else {
            buffer[i] = (char)c;
            i++;
        }
    }
    /*gestion derniere occurence*/
    if (i > 0) {
        buffer[i] = '\0';

        int type = atoi(buffer);
        /* on ajoute pas le block qui contient lelement a exclure */
        if (element_vide != type) {
            Block actuel = (Block){x, y, pas_x, pas_y, type};

            if (ptr->size == ptr->capacity) {
                ptr->capacity *= 2;
                ptr->tab = realloc(ptr->tab, sizeof(Block) * ptr->capacity);
            }

            ptr->tab[ptr->size] = actuel;
            ptr->size++;
        }
    }
    fclose(fs);
    return ptr;
}
