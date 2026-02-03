/*
 * Bindings C pour le module jeu.
 * Expose les fonctions de physique, collision et camera.
 */

#include "../liberer/liberer.h"
#include "../module_jeu/camera/camera.h"
#include "../module_jeu/carte/carte.h"
#include "../module_jeu/hitbox/hitbox.h"

/* Calcule la physique de plateforme 2D avec gestion des collisions */
EntityPlatformer *hitboxPlatformer(EntityPlatformer *entite, Blocks *blocs, float vitesse_max_chute,
                                   float correction_mur, int *ignored_type, int size) {
    return hitbox_platforme(entite, blocs, vitesse_max_chute, correction_mur, ignored_type, size);
}

/* Charge les blocs a partir d'un fichier de carte */
Blocks *getBlocksFromFile(const char *chemin, Uint8 pas_x, Uint8 pas_y, char separation,
                          int excludeElement) {
    return charger_carte(chemin, pas_x, pas_y, separation, excludeElement);
}

/* Calcule la physique vue de dessus avec gestion des collisions */
EntityTopdown *hitboxTopdown(EntityTopdown *entity, Blocks *blocks, int *ignored_type, int size) {
    return hitbox_topdown(entity, blocks, ignored_type, size);
}

/* Met a jour la position de la camera avec interpolation */
void cameraUpdate(Camera *cam, float target_x, float target_y, float dt) {
    camera_mise_a_jour(cam, target_x, target_y, dt);
}

/* Cree un conteneur de blocs vide */
Blocks *createBlocks(void) { return creer_blocks(); }

/* Retourne le nombre de blocs dans le conteneur */
int getBlocksSize(Blocks *blocks) { return taille_blocks(blocks); }

/* Ajoute un bloc au conteneur */
void addBlock(Blocks *blocks, Block *block) { ajouter_block(blocks, block); }

/* Libere tous les blocs du conteneur */
void freeBlocks(Blocks *blocks) { liberer_blocks(blocks); }