#pragma once
#include <enet6/enet.h>

ENetPacket* reseau_creer_paquet(const char* contenu, size_t taille, ENetPacketFlag flags);
ENetPacket* reseau_creer_paquet_texte_fiable(const char* texte);