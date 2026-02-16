#pragma once
#include <enet6/enet.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Envoie un paquet au paire
 */
int reseau_envoyer_paquet(ENetPacket *paquet, ENetPeer *peer);

#ifdef __cplusplus
}
#endif