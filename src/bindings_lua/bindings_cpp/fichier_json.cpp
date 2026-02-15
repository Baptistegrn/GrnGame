/*
 * Enregistrement des bindings GrnGame (JSON) dans la VM Lua.
 */

#include "sol/optional_implementation.hpp"
#include <sol/sol.hpp>

extern "C" {
#include "../../bindings_c/GrnGame.h"
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
void lua_charger_json_fichier(const std::string &filename) { loadJsonFile(filename.c_str()); }

/* sauvegarde un fichier json sur le disque */
void lua_sauvegarder_json_fichier(const std::string &filename) { saveJsonFile(filename.c_str()); }

/* supprime un fichier json du disque et de la memoire */
void lua_supprimer_json_fichier(const std::string &filename) { deleteJsonFile(filename.c_str()); }

/* ecrit une variable dans le json en detectant automatiquement le type */
void lua_ecrire_variable(const std::string &filename, const std::string &varName,
                         sol::object value) {
    if (value.is<bool>()) {
        writeIntToJson(filename.c_str(), varName.c_str(), value.as<bool>() ? 1 : 0);
    } else if (value.is<std::string>()) {
        writeStringToJson(filename.c_str(), varName.c_str(), value.as<std::string>().c_str());
    } else if (value.is<double>()) {
        double val = value.as<double>();
        if (val == (int)val) {
            writeIntToJson(filename.c_str(), varName.c_str(), (int)val);
        } else {
            writeDoubleToJson(filename.c_str(), varName.c_str(), val);
        }
    }
}

/* Recupere une variable depuis le json avec detection auto */
sol::object lua_lire_variable(const std::string &filename, const std::string &varName,
                              sol::this_state s) {
    sol::state_view lua(s);

    int type = getJsonVariableType(filename.c_str(), varName.c_str());

    switch (type) {
    case JSON_TYPE_NOMBRE: {
        double val = readDoubleFromJson(filename.c_str(), varName.c_str());
        if (val == (int)val) {
            return sol::make_object(lua, (int)val);
        }
        return sol::make_object(lua, val);
    }
    case JSON_TYPE_STRING: {
        char *val = readStringFromJson(filename.c_str(), varName.c_str());
        if (val)
            return sol::make_object(lua, std::string(val));
        break;
    }
    default:
        /* affiche lerreur si le chemin nest pas valide */
        double val = readDoubleFromJson(filename.c_str(), varName.c_str());
        /* essaye de renvoyer la valeur quand meme */
        return sol::make_object(lua, val);
        break;
    }
    return sol::make_object(lua, sol::nil);
}

/* supprime une variable du json */
void lua_supprimer_variable_depuis_json(const std::string &filename, const std::string &varName) {
    deleteVariableFromJson(filename.c_str(), varName.c_str());
}

/* definit une partie de la cle de chiffrement */
void lua_metre_json_cle(int index, int value) { setJsonEncryptionKey(index, value); }

/* definit une partie du vecteur d'initialisation */
void lua_mettre_json_iv(int index, int value) { setJsonEncryptionIV(index, value); }

/* verifie si un fichier existe ou pas */
bool lua_fichier_existe(const std::string &filename) { return FileExists(filename.c_str()); }

/* enregistrement des bindings json */
void enregistrer_bindings_json(sol::table &json) {
    json.set_function("load", &lua_charger_json_fichier);
    json.set_function("save", &lua_sauvegarder_json_fichier);
    json.set_function("delete", &lua_supprimer_json_fichier);
    json.set_function("writeVariable", &lua_ecrire_variable);
    json.set_function("readVariable", &lua_lire_variable);
    json.set_function("deleteVariable", &lua_supprimer_variable_depuis_json);
    json.set_function("setKey", &lua_metre_json_cle);
    json.set_function("setIV", &lua_mettre_json_iv);
    json.set_function("exists", &lua_fichier_existe);
}