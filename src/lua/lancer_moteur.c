#include "../bindings_c/GrnGame.h"

#if defined(DEBUG_MODE)
int main(void) {
    initialisation_lua();
    return 0;
}

#elif defined(_WIN32)
#include <windows.h>
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    initialisation_lua();
    return 0;
}

#else
int main(void) {
    initialisation_lua();
    return 0;
}
#endif