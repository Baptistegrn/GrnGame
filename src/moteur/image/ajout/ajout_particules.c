#include "../../../main.h"

/* ajoute un tableau de particules au tableau dimages */
void ajouter_particules_au_tableau(float *x, float *y, Uint16 *rotation, Uint8 *a, Uint8 *r,
                                   Uint8 *g, Uint8 *b, int taille) {
    if (!gs)
        goto gsvide;

    ObjectImage obj;
    memset(&obj, 0, sizeof(ObjectImage));

    obj.type = TYPE_PARTICULE;
    obj.particule.posx = x;
    obj.particule.posy = y;
    obj.particule.rotation = rotation;
    obj.particule.a = a;
    obj.particule.r = r;
    obj.particule.g = g;
    obj.particule.b = b;
    obj.particule.taille = taille;

    ajouter_image_au_jeu(obj);

    return;
gsvide:
    log_message(NiveauLogErreur, "manager empty in add particules to array");
}