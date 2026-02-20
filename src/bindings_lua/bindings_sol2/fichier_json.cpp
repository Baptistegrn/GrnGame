/*
 * Enregistrement des bindings GrnGame (JSON) dans la VM Lua.
 */
#include "sol/forward.hpp"
#include "sol/table.hpp"
#include <sol/sol.hpp>
#include <string>

extern "C" {
#include "../../moteur/json/json.h"
#include "../../moteur/logging/logging.h"
#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>

/* Codes de retour pour les types */
#define JSON_TYPE_INCONNU 0
#define JSON_TYPE_NOMBRE 1
#define JSON_TYPE_STRING 2
#define JSON_TYPE_BOOL 3
}

/* charge un fichier json en memoire */
void lua_charger_json_fichier(const std::string &filename) {
    charger_fichier_json(filename.c_str());
}

/* sauvegarde un fichier json sur le disque */
void lua_sauvegarder_json_fichier(const std::string &filename) {
    sauvegarder_fichier_json(filename.c_str());
}

/* supprime un fichier json du disque et de la memoire */
void lua_supprimer_json_fichier(const std::string &filename) {
    supprimer_fichier_json(filename.c_str());
}
/* ecrit recursivement du json */
void ecrire_recursif_json(const std::string &filename, sol::object data,
                          std::string chemin_actuel) {
    /* si la donnee est une table on boucle a linterieur */
    if (data.is<sol::table>()) {
        sol::table tbl = data.as<sol::table>();
        for (auto &elem : tbl) {
            std::string cle;

            /* securite : on convertit la cle en string (pour eviter les crashs avec les tableaux
             * lua) */
            if (elem.first.is<std::string>()) {
                cle = elem.first.as<std::string>();
            } else if (elem.first.is<int>()) {
                cle = std::to_string(elem.first.as<int>());
            } else {
                continue; /* on ignore les cles non supportees */
            }

            sol::object valeur = elem.second;
            std::string nouveau_chemin;

            /* on construit le chemin en fonction de sil est vide ou pas */
            if (chemin_actuel.empty()) {
                nouveau_chemin = cle;
            } else {
                nouveau_chemin = chemin_actuel + "." + cle;
            }

            /* appel recursif avec le nouveau chemin */
            ecrire_recursif_json(filename, valeur, nouveau_chemin);
        }
    }
    /* sinon cest une valeur simple on letablit au bout du chemin */
    else {
        /* on ne peut pas ecrire une valeur simple a la racine sans chemin */
        if (chemin_actuel.empty())
            return;

        if (data.is<bool>()) {
            ecrire_int_json(filename.c_str(), chemin_actuel.c_str(), data.as<bool>() ? 1 : 0);
        } else if (data.is<std::string>()) {
            ecrire_string_json(filename.c_str(), chemin_actuel.c_str(),
                               data.as<std::string>().c_str());
        } else if (data.is<double>()) {
            double val = data.as<double>();
            if (val == (int)val) {
                ecrire_int_json(filename.c_str(), chemin_actuel.c_str(), (int)val);
            } else {
                ecrire_double_json(filename.c_str(), chemin_actuel.c_str(), val);
            }
        }
    }
}

/* expose a lua : ecrit une variable ou une table dans le json */
void lua_ecrire_variable(const std::string &filename, sol::object arg1,
                         sol::optional<sol::object> arg2) {

    /* cas 1 : on donne un chemin en argument 1 et une valeur en argument 2 (peut etre une table) */
    if (arg1.is<std::string>() && arg2.has_value()) {
        std::string chemin = arg1.as<std::string>();
        sol::object valeur = arg2.value();
        ecrire_recursif_json(filename, valeur, chemin);
    }
    /* cas 2 : argument 1 est une table lua donc on ne donne pas dargument 2 */
    else if (arg1.is<sol::table>()) {
        ecrire_recursif_json(filename, arg1, "");
    }
    /* cas derreur : mauvais arguments depuis lua */
    else {
        log_fmt(NiveauLogErreur, "you give wrong arguments in json.writeVariable");
    }
}
/* Recupere une variable depuis le json avec detection auto */
sol::object lua_lire_variable(const std::string &filename, const std::string &varName,
                              sol::this_state s) {
    sol::state_view lua(s);
    int type = recuperer_type_json(filename.c_str(), varName.c_str());
    switch (type) {
    case JSON_TYPE_NOMBRE: {
        double val = recuperer_double_json(filename.c_str(), varName.c_str());
        if (val == (int)val) {
            return sol::make_object(lua, (int)val);
        }
        return sol::make_object(lua, val);
    }
    case JSON_TYPE_STRING: {
        char *val = recuperer_string_json(filename.c_str(), varName.c_str());
        if (val) {
            sol::object obj = sol::make_object(lua, std::string(val));
            return obj;
        }
        break;
    }
    case JSON_TYPE_OBJECT: {
        /* recupere les chemins a partir du chemin de base */
        cJSON *tab = lister_tous_les_chemins(filename.c_str(), varName.c_str());
        /* table lua */
        sol::table table_resultat = lua.create_table();
        if (tab != NULL && cJSON_IsArray(tab)) {
            cJSON *element = NULL;
            /* parcours tableau cJSON */
            cJSON_ArrayForEach(element, tab) {
                if (cJSON_IsString(element) && element->valuestring) {
                    std::string sous_chemin = element->valuestring;
                    /* chemin complet ou la on peut recuperer la variable */
                    std::string chemin_complet =
                        varName.empty() ? sous_chemin : varName + "." + sous_chemin;

                    sol::object valeur_finale = lua_lire_variable(filename, chemin_complet, s);
                    /* gestion de la table lua */
                    std::stringstream ss(sous_chemin);
                    std::string token;
                    std::vector<std::string> cles;
                    while (std::getline(ss, token, '.')) {
                        cles.push_back(token);
                    }
                    sol::table curseur = table_resultat;
                    for (size_t i = 0; i < cles.size(); ++i) {
                        if (i == cles.size() - 1) {
                            curseur[cles[i]] = valeur_finale;
                        } else {
                            sol::object enfant = curseur[cles[i]];
                            if (!enfant.is<sol::table>()) {
                                curseur[cles[i]] = lua.create_table();
                            }
                            curseur = curseur[cles[i]];
                        }
                    }
                }
            }
            /* liberation du tableau cJSON obligatoire */
            cJSON_Delete(tab);
            return table_resultat;
        } else {
            log_fmt(NiveauLogErreur, "Path error '%s' invalid in %s", varName.c_str(),
                    filename.c_str());
            if (tab)
                cJSON_Delete(tab);
        }
        break;
    }
    default: {
        /* affiche l'erreur si le chemin n'est pas valide, essaye double par defaut */
        double val = recuperer_double_json(filename.c_str(), varName.c_str());
        return sol::make_object(lua, val);
    }
    }

    return sol::make_object(lua, sol::nil);
}

/* supprime une variable du json */
void lua_supprimer_variable_depuis_json(const std::string &filename, const std::string &varName) {
    supprimer_variable_json(filename.c_str(), varName.c_str());
}

/* verifie si un fichier existe ou pas */
bool lua_fichier_existe(const std::string &filename) { return fichier_existant(filename.c_str()); }

/* enregistrement des bindings json */
void enregistrer_bindings_json(sol::table &json) {
    json.set_function("load", &lua_charger_json_fichier);
    json.set_function("save", &lua_sauvegarder_json_fichier);
    json.set_function("delete", &lua_supprimer_json_fichier);
    json.set_function("writeVariable", &lua_ecrire_variable);
    json.set_function("readVariable", &lua_lire_variable);
    json.set_function("deleteVariable", &lua_supprimer_variable_depuis_json);
    json.set_function("setKey",
                      [](int index, uint8_t valeur) { mettre_fichiers_cle(index, valeur); });
    json.set_function("setIv",
                      [](int index, uint8_t valeur) { mettre_fichiers_iv(index, valeur); });
    json.set_function("exists", &lua_fichier_existe);
}