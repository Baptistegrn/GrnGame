/* enregistre les bindings de fenetre dans la vm lua */

#include <sol/sol.hpp>

extern "C" {
#include "../../moteur/boucle/boucle.h"
#include "../../moteur/entrees/entrees.h"
#include "../../moteur/fenetre/fenetre.h"
#include "../../moteur/initialisation/initialisation.h"
#include "../../moteur/logging/logging.h"
#include "../../proprietes.h"
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
}

#include <sol/sol.hpp>

extern "C" {
#include "../../moteur/constante/constante.h"
}

/* enregistrement des bindings de fenetre */
void enregistrer_bindings_fenetre(sol::table &window) {
    window.set_function("fullscreen", &passer_plein_ecran);
    window.set_function("windowedFullscreen", &passer_fenetre_maximisee);
    window.set_function("windowed", &passer_fenetre_taille);
    window.set_function("windowedCoefficient", &passer_fenetre_coeff);
    window.set_function("setFps", definir_fps);
    window.set_function("setTitle", definir_titre);
    window.set_function("setBlackBars", definir_bandes_noires);
    window.set_function("setUniversSize", definir_taille);
}