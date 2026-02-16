#pragma once
#include <enet6/enet.h>

typedef struct {
    int estServeur;
    int estActif;
    int estConnecte;
    ENetHost *hote;
    ENetPeer *serveurPeer;
    ENetAddress serveurAddresse;
} GestionnaireReseau;

/**
 * Crée le gestionnaire réseau
 * @param estServeur Si oui ou non nous sommes le serveur
 * @param hote L'addresse en format IPV4/IPV6 ou se trouve le serveur / on doit heberger
 * @param port Le port sur lequel on veut se connecter / heberger
 * @param maxPeers Le nombre max de paires
 * @param addressType ENET_ADDRESS_TYPE_IPV4 ou ENET_ADDRESS_TYPE_IPV6
 * @return Un GestionnaireReseau si tout c'est bien passé, sinon NULL
 */
GestionnaireReseau *reseau_creer(int estServeur, const char *hote, uint16_t port, uint32_t maxPeers,
                                 ENetAddressType addressType);

/**
 * Réalise un avortement post-natal sur un GestionnaireReseau afin de liberer les ressources
 * pour les distribuer aux autres processus #LFI
 */
void reseau_detruire(GestionnaireReseau *gestionnaireReseau);

/**
 * Traite les requets en attente, en appellant callback pour chaque paquet recu
 * @param callback Le callback a appeler avec le paquet
 * @param gestionnaireReseau Le gestionnaire réseau
 * @return Le nombre de paquets traitées
 */
int reseau_update(GestionnaireReseau *gestionnaireReseau, void(*callback)(const ENetPacket*));