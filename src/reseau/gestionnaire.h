#pragma once
#include <enet6/enet.h>

typedef struct {
    int estServeur;
    int estActif;
    int estConnecte;
    ENetHost* hote;
    ENetPeer* serveurPeer;
    ENetAddress serveurAddresse;
} GestionnaireReseau;

GestionnaireReseau* reseau_creer(int estServeur, const char* hote, uint16_t port);
void reseau_detruire(GestionnaireReseau* gestionnaireReseau);
int reseau_update(GestionnaireReseau* gestionnaireReseau);