#pragma once
#include "gestionnaire.h"
#include <enet6/enet.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Envoie un paquet au serveur
 */
int reseau_envoyer_paquet(ENetPacket *paquet, ENetPeer *peer);

/**
 * Envoie paquet a tous les clients connectées a l'hote
 */
void reseau_broadcast_paquet(ENetPacket *paquet, ENetHost *host);

/**
 * Si on est un serveur, envoie le paquets aux clients, sinon l'envoie au serveur
 */
void reseau_traiter_paquet(ENetPacket *paquet, GestionnaireReseau *gestionnaireReseau);

#ifdef __cplusplus
}
#endif