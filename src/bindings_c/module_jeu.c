#include "../module_jeu/camera/camera.h"
#include "../module_jeu/carte/carte.h"
#include "../module_jeu/hitbox/hitbox.h"

/*
 * Calcule la physique de plateforme 2D avec gestion des collisions.
 */
EntityPlatformer *hitboxPlatformer(EntityPlatformer *entite, Blocks *blocs, float vitesse_max_chute,
                                   float correction_mur, int *ignored_type, int size) {
    return hitbox_platforme(entite, blocs, vitesse_max_chute, correction_mur, ignored_type, size);
}
/*recupere les blocs a partir d'un fichier */
Blocks *getBlocksFromFile(const char *chemin, Uint8 pas_x, Uint8 pas_y, char separation,
                          int excludeElement) {
    return charger_carte(chemin, pas_x, pas_y, separation, excludeElement);
}

/*calcule la physique facon top_down view et gere les collisions*/
EntityTopdown *hitboxTopdown(EntityTopdown *entity, Blocks *blocks, int *ignored_type, int size) {
    return hitbox_topdown(entity, blocks, ignored_type, size);
}
/*Camera */

void cameraUpdate(Camera *cam, float target_x, float target_y, float dt) {
    camera_mise_a_jour(cam, target_x, target_y, dt);
}