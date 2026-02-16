#include "serialisation.h"

#include <string.h>

ENetPacket *reseau_creer_paquet(const char *contenu, size_t taille, ENetPacketFlag flags) {
    return enet_packet_create(contenu, taille, flags);
}

ENetPacket *reseau_creer_paquet_texte_fiable(const char *texte) {
    return reseau_creer_paquet(texte, strlen(texte), ENET_PACKET_FLAG_RELIABLE);
}