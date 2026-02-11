/*
 * Enregistrement des bindings GrnGame dans la VM Lua.
 */

#include "sol/optional_implementation.hpp"
#include <sol/sol.hpp>

extern "C" {
#include "../../bindings_c/GrnGame.h"
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
}

/* Cache global pour la fonction de mise a jour Lua */
static sol::function g_update_func;

/*
 * Fonction trampoline appelee par le moteur C a chaque frame.
 * Appelle la fonction Lua de mise a jour stockee en cache.
 */
static void update_trampoline(void) {
    if (g_update_func.valid()) {
        g_update_func();
    }
}

/*
 * Reinitialise la fonction de mise a jour avant de fermer l'etat Lua.
 * Doit etre appele avant lua_close() pour eviter des acces invalides.
 */
extern "C" void reset_update_func(void) { g_update_func = sol::nil; }

/* parametre par default */
#define DEFAULT_HEIGHT 320
#define DEFAULT_WIDTH 180
#define DEFAULT_FPS 60.0f
#define DEFAULT_BLACK_BARS false
#define DEFAULT_WINDOW_TITLE "My Game"
#define DEFAULT_UPDATE_FUNC "update"

/* initialise le moteur et demarre la boucle principale */
void luaInitialize(sol::optional<int> height, sol::optional<int> width, sol::optional<float> fps,
                   sol::optional<bool> black_bars, sol::optional<std::string> window_title,
                   sol::optional<sol::function> update_func, sol::this_state ts) {
    sol::state_view lua(ts);

    int h = height.value_or(DEFAULT_HEIGHT);
    int w = width.value_or(DEFAULT_WIDTH);
    float f = fps.value_or(DEFAULT_FPS);
    bool b = black_bars.value_or(DEFAULT_BLACK_BARS);
    std::string wt = window_title.value_or(DEFAULT_WINDOW_TITLE);

    if (update_func) {
        g_update_func = *update_func;
    } else {
        g_update_func = lua[DEFAULT_UPDATE_FUNC];
    }

    initialize(h, w, f, b, wt.c_str(), update_trampoline);
}

/* enregistre un message de log */
void luaLogMessage(int level, const std::string &message) { logMessage(level, message.c_str()); }

/* enregistrement des bindings utilitaires */
void enregistrer_bindings_utils(sol::table &utils) {
    utils.set_function("initialize", &luaInitialize);
    utils.set_function("logMessage", &luaLogMessage);
    utils.set_function("stop", &stop);
    utils.set_function("fullscreen", &fullscreen);
    utils.set_function("windowedFullscreen", &windowedFullscreen);
    utils.set_function("windowed", &windowed);
    utils.set_function("windowedMinimised", &windowedMinimised);
    utils.set_function("cls", &clearScreen);
    utils.set_function("setLogLvl", &setLogLvl);
}