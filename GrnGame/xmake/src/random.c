
#include <stdbool.h>
#include "main.h"

int random_jeu(int min, int max)
{
    return min + rand() % (max - min + 1);
}
