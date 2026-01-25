#include "../../../main.h"
#include "ajout.h"

/*
 * Ajoute une image au tableau de rendu.
 * Gère la rotation et le cache des textures pour l'optimisation.
 */
void ajouter_image_au_tableau(const char *id, float x, float y, Sint16 taillex, Sint16 tailley,
                              bool sens, Uint16 rotation, Uint8 a) {
    if (!gs)
        goto gsvide;
    /* Correction rotation */
    if (rotation > 359) {
        rotation = 0;

        log_fmt(NiveauLogAvertissement, "Rotation invalid for image %s correction to 0", id);
    }

    ObjectImage obj;
    memset(&obj, 0, sizeof(ObjectImage));

    /* Limitation des valeurs Alpha entre 0 et 255 */
    a = SDL_clamp(a, 0, 255);
    obj.type = TYPE_IMAGE;
    obj.image.posx = x;
    obj.image.posy = y;
    obj.image.taillex = taillex;
    obj.image.tailley = tailley;
    obj.image.sens = sens;
    obj.image.rotation = rotation;
    obj.image.a = a;

    /* Gestion du cache d'angle */
    if (rotation == 0) {
        obj.image.texture = recuperer_texture_par_lien(id);
    } else {
        obj.image.texture = recuperer_texture_variante(id, rotation);

        if (!obj.image.texture) {
            SDL_Texture *texture_base = recuperer_texture_par_lien(id);

            if (texture_base) {
                SDL_RendererFlip flip = (sens == 0) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

                obj.image.texture =
                    creer_texture_tournee_rapide(texture_base, taillex, tailley, rotation, flip);

                if (obj.image.texture) {
                    ajouter_variante_cache(

                        id, obj.image.texture, rotation);
                }
            }
        }
    }

    /* Vérification texture finale */
    if (!obj.image.texture) {
        log_fmt(NiveauLogErreur, "Texture not found for '%s'", id);
        return;
    }

    ajouter_image_au_jeu(obj);

    return;

gsvide:
    log_fmt(NiveauLogDebug, "manager is empty in final add of image to table");
}
