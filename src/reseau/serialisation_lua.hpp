#pragma once
#include <enet6/enet.h>
#include <sol/sol.hpp>

ENetPacket* reseau_creer_paquet_lua_fiable(const sol::table& table);
sol::table reseau_lire_paquet_lua(sol::state& lua, const ENetPacket* paquet);