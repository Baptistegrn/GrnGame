#include "../../../main.h"

/*
 * Récupère les axes normalisés (sticks et gâchettes) de la manette.
 * Retourne un tableau de 6 flottants alloué dynamiquement.
 * Indices : 0-1 (Stick G), 2-3 (Stick D), 4-5 (Triggers).
 */
float *renvoie_joysticks(float dead_zone, unsigned char index) {
    if (!gs)
        goto gsvide;
    /* Allocation */
    float *valeurs = xmalloc(sizeof(float) * 6);

    /* Normalisation des valeurs entre -1.0 et 1.0 */
    const float normalisation = 32766.0f;

    GestionnaireEntrees *entrees = gs->entrees;

    /* Stick gauche */
    valeurs[0] = (float)entrees->Joy[index].g.x / normalisation;
    valeurs[1] = (float)entrees->Joy[index].g.y / normalisation;

    /* Stick droit */
    valeurs[2] = (float)entrees->Joy[index].d.x / normalisation;
    valeurs[3] = (float)entrees->Joy[index].d.y / normalisation;

    /* Triggers (Gâchettes) */
    valeurs[4] = (float)entrees->trigger[index].trigger_g / normalisation;
    valeurs[5] = (float)entrees->trigger[index].trigger_d / normalisation;
    /* Application de la zone morte (Deadzone) */
    for (int i = 0; i < 6; i++) {
        if (fabsf(valeurs[i]) < dead_zone) {
            valeurs[i] = 0.0f;
        }
    }

    return valeurs;

gsvide:
    log_message(NiveauLogDebug, "manager empty in return joysticks function");
    return NULL;
}

/*
 * Vérifie si un bouton de la manette vient d'être pressé (Front descendant).
 * Utilise le nom du bouton ("A", "B", "Start", etc.).
 */
bool touche_mannette_juste_presse(const char *touche, unsigned char index) {
    if (!gs)
        goto gsvide;
    if (!touche)
        return false;

    SDL_GameControllerButton bt = bouton_manette_depuis_nom(touche);

    return (bt != SDL_CONTROLLER_BUTTON_INVALID) && gs->entrees->manette_presse[bt][index];

gsvide:
    log_message(NiveauLogDebug, "manager empty in controller just pressed function");
    return false;
}

/*
 * Vérifie si un bouton de la manette est actuellement maintenu enfoncé.
 */
bool touche_mannette_enfoncee(const char *touche, unsigned char index) {
    if (!gs)
        goto gsvide;
    if (!touche)
        return false;

    SDL_GameControllerButton bt = bouton_manette_depuis_nom(touche);

    return (bt != SDL_CONTROLLER_BUTTON_INVALID) && gs->entrees->manette[bt][index];

gsvide:
    log_message(NiveauLogDebug, "manager empty in controller pressed function");
    return false;
}