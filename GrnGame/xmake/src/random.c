
#include "main.h"
#include <stdbool.h>

 int random_jeu(int min, int max)
{
    return min + rand() % (max - min + 1);
}
