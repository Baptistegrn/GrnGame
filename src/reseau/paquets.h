#pragma once
#include <enet6/enet.h>

#ifdef __cplusplus
extern "C" 
{
#endif

ENetPacket* reseau_creer_paquet(const char* contenu, size_t taille, ENetPacketFlag flags);
ENetPacket* reseau_creer_paquet_texte_fiable(const char* texte);

#ifdef __cplusplus
}
#endif