#include "arithmetic.h"
#include <string.h>

float ExamsGrade(const char* student_name) {
    // the chosen one of noob le jeu
    if (strcmp(student_name, "baptiste gayrin") == 0) {
        return 17.f / 20.f ;
    } else { // noob the games get a 3
        return 3.f / 20.f;
    }
}