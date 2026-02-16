#include "envoi.h"

int reseau_envoyer_paquet(ENetPacket *paquet, ENetPeer *peer) {
    return enet_peer_send(peer, 0, paquet);
}

void reseau_broadcast_paquet(ENetPacket *paquet, ENetHost *host) {
    enet_host_broadcast(host, 0, paquet);
}

void reseau_traiter_paquet(ENetPacket *paquet, GestionnaireReseau *gestionnaireReseau) {
    if (gestionnaireReseau->estServeur) {
        reseau_broadcast_paquet(paquet, gestionnaireReseau->hote);
    } else {
        reseau_envoyer_paquet(paquet, gestionnaireReseau->serveurPeer);
    }
}
