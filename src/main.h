/*
 * contient uniquement forward-declarations pour le Gestionnaire.
 */

#ifndef MAIN_H
#define MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Forward declarations */
typedef struct GestionnaireTiming GestionnaireTiming;
typedef struct GestionnaireFenetre GestionnaireFenetre;
typedef struct GestionnaireEntrees GestionnaireEntrees;
typedef struct GestionnaireTextures GestionnaireTextures;
typedef struct GestionnaireSon GestionnaireSon;
typedef struct GestionnaireFrameActuelle GestionnaireFrameActuelle;
typedef struct GestionnaireFichiers GestionnaireFichiers;
typedef struct Camera Camera;
typedef struct GestionnaireModuleJeu GestionnaireModuleJeu;

/* Structure principale du moteur */
typedef struct Gestionnaire {
    GestionnaireTiming *timing;
    GestionnaireFenetre *fenetre;
    GestionnaireEntrees *entrees;
    GestionnaireTextures *textures;
    GestionnaireSon *sons;
    GestionnaireFrameActuelle *frame;
    GestionnaireFichiers *fichiers;
    GestionnaireModuleJeu *module_jeu;
} Gestionnaire;

/* Instance globale du gestionnaire */
extern Gestionnaire *gs;

#ifdef __cplusplus
}
#endif

#endif /* MAIN_H */
