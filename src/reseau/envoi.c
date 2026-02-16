#include "envoi.h"

int reseau_envoyer_paquet(ENetPacket *paquet, ENetPeer *peer) {
    return enet_peer_send(peer, 0, paquet);
}