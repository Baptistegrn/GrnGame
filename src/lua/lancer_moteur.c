#include "../bindings_c/GrnGame.h"

#if defined(_WIN32)
#include <windows.h>
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    initializeLua("../src/main.lua");
    return 0;
}

#else
int main(void) {
    initializeLua("../src/main.lua");
    return 0;
}
#endif