#include "../../main.h"

/* charge une carte */
// TODO : remplacer le pas_x et pas_y par des fonctions arithmetiques
Blocks *charger_carte(const char *chemin, Uint8 pas_x, Uint8 pas_y, char separation,
                      int element_vide) {
    FILE *fs = fopen(chemin, "r");
    if (!fs) {
        log_fmt(NiveauLogErreur, "error open file for load a map: %s\n", chemin);

        // retourner un seul bloc vide
        Blocks *vide = xmalloc(sizeof(Blocks));
        vide->capacity = 1;
        vide->size = 1;
        vide->tab = xmalloc(sizeof(Block));

        vide->tab[0] = (Block){
            0, // x
            0, // y
            pas_x, pas_y,
            element_vide // type vide
        };

        return vide;
    }

    char buffer[TAILLE_LIGNE];
    int i = 0;

    int x = 0;
    int y = 0;

    Blocks *ptr = xmalloc(sizeof(Blocks));
    ptr->capacity = 50;
    ptr->size = 0;
    ptr->tab = xmalloc(sizeof(Block) * ptr->capacity);

    int c;
    while ((c = fgetc(fs)) != EOF) {

        /* separateur de blocs */
        if (c == separation) {
            buffer[i] = '\0';
            int type = atoi(buffer);

            if (type != element_vide) {
                Block actuel = (Block){x, y, pas_x, pas_y, type};

                if (ptr->size == ptr->capacity) {
                    ptr->capacity *= 2;
                    ptr->tab = xrealloc(ptr->tab, sizeof(Block) * ptr->capacity);
                }

                ptr->tab[ptr->size++] = actuel;
            }
            x += pas_x;
            i = 0;
        }

        /* fin de ligne */
        else if (c == '\n') {
            buffer[i] = '\0';

            if (i > 0) {
                int type = atoi(buffer);
                if (type != element_vide) {
                    Block actuel = (Block){x, y, pas_x, pas_y, type};

                    if (ptr->size == ptr->capacity) {
                        ptr->capacity *= 2;
                        ptr->tab = xrealloc(ptr->tab, sizeof(Block) * ptr->capacity);
                    }

                    ptr->tab[ptr->size++] = actuel;
                }
            }

            x = 0;
            y += pas_y;
            i = 0;
        }

        /* accumulation des chiffres */
        else {
            buffer[i++] = (char)c;
        }
    }

    /* derniere occurrence */
    if (i > 0) {
        buffer[i] = '\0';
        int type = atoi(buffer);

        if (type != element_vide) {
            Block actuel = (Block){x, y, pas_x, pas_y, type};

            if (ptr->size == ptr->capacity) {
                ptr->capacity *= 2;
                ptr->tab = xrealloc(ptr->tab, sizeof(Block) * ptr->capacity);
            }

            ptr->tab[ptr->size++] = actuel;
        }
    }

    fclose(fs);
    return ptr;
}
