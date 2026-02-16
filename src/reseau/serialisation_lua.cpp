#include "serialisation_lua.hpp"
#include "../moteur/logging/logging.h"
#include "enet6/enet.h"
#include "paquets.h"
#include "sol/forward.hpp"
#include "sol/table.hpp"
#include <cstddef>
#include <string>
#include <yyjson.h>

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

static sol::table json_vers_table_lua(sol::state& lua, yyjson_val* obj) {
    sol::table table = lua.create_table();

    yyjson_val* key;
    yyjson_val* val;
    yyjson_obj_iter iter;
    yyjson_obj_iter_init(obj, &iter);

    while ((key = yyjson_obj_iter_next(&iter))) {
        val = yyjson_obj_iter_get_val(key);

        const char* keyStr = yyjson_get_str(key);
        if (!keyStr) continue;

        if (yyjson_is_obj(val)) {
            table[keyStr] = json_vers_table_lua(lua, val);
        } else if (yyjson_is_str(val)) {
            table[keyStr] = std::string(yyjson_get_str(val));
        } else if (yyjson_is_real(val)) {
            table[keyStr] = yyjson_get_real(val);
        } else if (yyjson_is_int(val)) {
            table[keyStr] = (int)yyjson_get_int(val);
        } else if (yyjson_is_bool(val)) {
            table[keyStr] = yyjson_get_bool(val);
        } else {
            log_fmt(NiveauLogErreur, "Unsupported JSON value type in json_vers_table_lua");
        }
    }

    return table;
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

sol::table reseau_lire_paquet_lua(sol::state &lua, const ENetPacket *paquet) {
    if (!paquet || !paquet->data || paquet->dataLength == 0) {
        log_fmt(NiveauLogErreur, "Invalid ENet packet");
        return lua.create_table();
    }

    const char* jsonStr = reinterpret_cast<const char*>(paquet->data);
    size_t len = paquet->dataLength;

    yyjson_doc* doc = yyjson_read(jsonStr, len, 0);
    if (!doc) {
        log_fmt(NiveauLogErreur, "Failed to parse JSON from ENet packet");
        return lua.create_table();
    }

    yyjson_val* root = yyjson_doc_get_root(doc);
    sol::table table = json_vers_table_lua(lua, root);

    yyjson_doc_free(doc);
    return table;
}