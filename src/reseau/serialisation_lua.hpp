#pragma once
#include <enet6/enet.h>
#include <sol/sol.hpp>

ENetPacket* creer_paquet_lua_fiable(const sol::table& table);