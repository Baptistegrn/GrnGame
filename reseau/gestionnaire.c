#include "gestionnaire.h"

#include "../allouer/allouer.h"
#include "../moteur/logging/logging.h"

#include <stdio.h>

#define MAX_PEERS 32

GestionnaireReseau *reseau_creer(int estServeur, const char *hote, uint16_t port) {
    GestionnaireReseau *gestionnaire =
        (GestionnaireReseau *)malloc_gestion_echec_compteur(sizeof(GestionnaireReseau));
    gestionnaire->estServeur = estServeur;
    gestionnaire->estActif = 0;
    gestionnaire->estConnecte = 0;
    gestionnaire->hote = NULL;
    gestionnaire->serveurPeer = NULL;

    if (enet_initialize() != 0) {
        log_fmt(NiveauLogErreur, "Failed to initialize ENet");
        free_gestion_echec_compteur(gestionnaire);
        return NULL;
    }

    if (estServeur) {
        ENetAddress addresse;
        enet_address_build_any(&addresse, ENET_ADDRESS_TYPE_IPV4);
        addresse.port = port;
        gestionnaire->hote = enet_host_create(ENET_ADDRESS_TYPE_IPV4, &addresse, MAX_PEERS, 2, 0, 0);
    } else {
        gestionnaire->hote = enet_host_create(ENET_ADDRESS_TYPE_IPV4, NULL, 1, 2, 0, 0);
        enet_address_set_host(&gestionnaire->serveurAddresse, ENET_ADDRESS_TYPE_IPV4, hote);
        gestionnaire->serveurAddresse.port = port;
        gestionnaire->serveurPeer =
            enet_host_connect(gestionnaire->hote, &gestionnaire->serveurAddresse, 2, 0);
        if (!gestionnaire->serveurPeer) {
            log_fmt(NiveauLogErreur, "Failed to connect to ENet");
            free_gestion_echec_compteur(gestionnaire);
            return NULL;
        }
    }

    if (!gestionnaire->hote) {
        log_fmt(NiveauLogErreur, "Failed to connect to get an host");
        free_gestion_echec_compteur(gestionnaire);
        return NULL;
    }

    printf("creer\n");
    log_fmt(NiveauLogInfo,"Module réseau initialisé");
    return gestionnaire;
}

void reseau_detruire(GestionnaireReseau *gestionnaireReseau) {
    gestionnaireReseau->estActif = 0;
    if (gestionnaireReseau->hote) {
        enet_host_destroy(gestionnaireReseau->hote);
    }
    enet_deinitialize();
}

int reseau_update(GestionnaireReseau *gestionnaireReseau) {
    gestionnaireReseau->estActif = 1;
    ENetEvent event;
    int processed = 0;

    while (enet_host_service(gestionnaireReseau->hote, &event, 0) > 0) {
        processed++;

        switch (event.type) {
        case ENET_EVENT_TYPE_CONNECT:
            log_fmt(NiveauLogInfo, "New connection");
            if (!gestionnaireReseau->estServeur) {
                gestionnaireReseau->estConnecte = 1;
            }
            break;

        case ENET_EVENT_TYPE_RECEIVE:
            if (gestionnaireReseau->estServeur) {
                enet_host_broadcast(gestionnaireReseau->hote, 0,
                                    event.packet); // le serveur renvoie les packets
            } else {
                enet_packet_destroy(event.packet);
            }
            break;

        case ENET_EVENT_TYPE_DISCONNECT:
            printf("deconnection\n");
            log_fmt(NiveauLogInfo, "Disconnected from server");
            break;

        default:
            break;
        }
    }

    return processed;
}