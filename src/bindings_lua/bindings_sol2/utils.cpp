/*
 * Enregistrement des bindings GrnGame dans la VM Lua.
 */

#include "sol/optional_implementation.hpp"
#include <sol/sol.hpp>

extern "C" {
#include "../../moteur/boucle/boucle.h"
#include "../../moteur/entrees/entrees.h"
#include "../../moteur/initialisation/initialisation.h"
#include "../../moteur/logging/logging.h"
#include "../../proprietes.h"
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
}

/* Cache global pour la fonction de mise a jour Lua */
static sol::protected_function g_update_func;

/*
 * Fonction trampoline appelee par le moteur C a chaque frame.
 * Appelle la fonction Lua de mise a jour stockee en cache.
 * Gere les erreurs Lua et affiche un traceback complet.
 */
static void update_trampoline(void) {
    if (!g_update_func.valid())
        return;

    sol::protected_function_result resultat = g_update_func();
    if (!resultat.valid()) {
        sol::error err = resultat;
        /* si erreur dans la callback on quitte */
        log_message(NiveauLogErreur, "Lua Runtime Error in update callback:");
        log_message(NiveauLogErreur, err.what());
        /* stop le jeu*/
        arreter_gestionnaire();
    }
}

/*
 * Reinitialise la fonction de mise a jour avant de fermer l'etat Lua.
 * Doit etre appele avant lua_close() pour eviter des acces invalides.
 */
extern "C" void fermer_callback(void) { g_update_func = sol::nil; }

/*
   Binding Lua : enregistre la callback `update` Lua
   - NE réalise PAS l'initialisation du moteur (SDL/structures)
   - stocke la fonction Lua protégée dans `g_update_func`
   - enregistre le trampoline C via `definir_rappel_mise_a_jour` */
void lua_set_update_callback(sol::optional<sol::function> update_func, sol::this_state ts) {
    sol::state_view lua(ts);

    /* récupère le traceback Lua pour protéger la fonction */
    sol::function traceback = lua["debug"]["traceback"];

    if (update_func && update_func->valid()) {
        g_update_func = sol::protected_function(*update_func, traceback);
    } else {
        /* si aucune fonction passée, cherche une globale `update` dans l'état */
        sol::object cand = lua[DEFAULT_UPDATE_FUNC];
        if (cand.is<sol::function>()) {
            g_update_func = sol::protected_function(cand.as<sol::function>(), traceback);
        } else {
            g_update_func = sol::nil; /* pas de callback */
        }
    }
    definir_rappel_mise_a_jour(update_trampoline);
    boucle_principale();
}

/* enregistre un message de log */
void lua_logMessage(int level, const std::string &message) {
    log_message((NiveauLog)level, message.c_str());
}

/* changer le niveau de logs */
void lua_changer_log_niveau(int lvl) { changer_niveau_log((NiveauLog)lvl); }

/* enregistrement des bindings utilitaires */
void enregistrer_bindings_utils(sol::table &utils) {
    utils.set_function("setUpdateCallback", &lua_set_update_callback);
    utils.set_function("logMessage", &lua_logMessage);
    utils.set_function("stopEngine", &arreter_gestionnaire);
    utils.set_function("setLogLvl", &lua_changer_log_niveau);
    utils.set_function("getInputText", &lire_texte_saisie);
    utils.set_function("deleteInputText", &vider_texte_saisie);
    utils.set_function("setWindowSize", definir_taille);
}