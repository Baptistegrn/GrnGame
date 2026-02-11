/*
 * Point d'entree du moteur de jeu avec bindings Lua.
 * Initialise Lua et lance le script principal.
 */

#include "../bindings_lua/bindings_lua.h"

#if defined(_WIN32)
#include <windows.h>

/* Point d'entree Windows */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    if (__argc > 1) {
        initialiser_lua(__argv[1]);
    } else {
        initialiser_lua("./src/main.lua");
    }
    liberer_lua();
    return 0;
}

#else

/* Point d'entree Unix/Linux */
int main(int argc, char **argv) {
    if (argc > 1) {
        initialiser_lua(argv[1]);
    } else {
        initialiser_lua("./src/main.lua");
    }
    liberer_lua();
    return 0;
}
#endif