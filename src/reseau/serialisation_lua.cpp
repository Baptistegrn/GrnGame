#include "serialisation_lua.hpp"
#include "enet6/enet.h"
#include "serialisation.h"
#include "sol/forward.hpp"
#include "sol/table.hpp"
#include <cstddef>
#include <string>
#include <yyjson.h>
#include "../moteur/logging/logging.h"

static std::string cle_vers_string(const sol::object& cle) {
    if (cle.is<int>()) {
        return std::to_string(cle.as<int>());
    } else {
        return cle.as<std::string>();
    }
}

static yyjson_mut_val* table_lua_vers_json(const sol::table& table, yyjson_mut_doc* doc) {
    yyjson_mut_val* obj = yyjson_mut_obj(doc);

    for (const auto& [cle, valeur] : table) {
        std::string cleStr = cle_vers_string(cle);

        if (valeur.is<int>()) {
            yyjson_mut_obj_add_int(doc, obj, cleStr.c_str(), valeur.as<int>());
        } else if (valeur.is<double>()) {
            yyjson_mut_obj_add_double(doc, obj, cleStr.c_str(), valeur.as<double>());
        } else if (valeur.is<bool>()) {
            yyjson_mut_obj_add_bool(doc, obj, cleStr.c_str(), valeur.as<bool>());
        } else if (valeur.is<std::string>()) {
            yyjson_mut_obj_add_strcpy(doc, obj, cleStr.c_str(), valeur.as<std::string>().c_str());
        } else if (valeur.is<sol::table>()) {
            yyjson_mut_val* nestedObj = table_lua_vers_json(valeur.as<sol::table>(), doc);
            yyjson_mut_obj_add_val(doc, obj, cleStr.c_str(), nestedObj);
        } else {
            log_fmt(NiveauLogErreur, "Unsupported type in table_lua_vers_json");
        }
    }

    return obj;
}

ENetPacket* reseau_creer_paquet_lua_fiable(const sol::table& table) {
    yyjson_mut_doc* doc = yyjson_mut_doc_new(NULL);

    yyjson_mut_val* root = table_lua_vers_json(table, doc);
    yyjson_mut_doc_set_root(doc, root);

    size_t len;
    char* jsonString = yyjson_mut_write(doc, 0, &len);

    ENetPacket* packet = reseau_creer_paquet(jsonString, len, ENET_PACKET_FLAG_RELIABLE);

    free(jsonString);
    yyjson_mut_doc_free(doc);

    return packet;
}