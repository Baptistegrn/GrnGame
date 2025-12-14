#include "../../logging/logging.h"
#include "../../main.h"
#include <math.h>
#include <string.h>

void ajouter_image_au_tableau(Gestionnaire *gs, char *id, int x, int y, int w,
                              int h, int sens, int rotation, int transparence) {
  if (!gs || !gs->textures)
    return;

  if (rotation > 359 || rotation < 0) {
    rotation = 0;
    log_fmt(NiveauLogAvertissement,
            "Avertissement: Rotation invalide, remise à 0");
  }

  Object_image obj;
  memset(&obj, 0, sizeof(Object_image));

  obj.type = TYPE_IMAGE;
  obj.image.posx = x;
  obj.image.posy = y;
  obj.image.taillex = w;
  obj.image.tailley = h;
  obj.image.sens = sens;
  obj.image.rotation = rotation;
  obj.image.transparence = transparence;

  // Gestion du cache d'angle
  if (rotation == 0) {
    obj.image.texture = recuperer_texture_par_lien(gs->textures, id);
  } else {
    obj.image.texture = recuperer_texture_variante(gs->textures, id, rotation);

    if (!obj.image.texture) {
      SDL_Texture *texture_base = recuperer_texture_par_lien(gs->textures, id);
      if (texture_base) {
        SDL_RendererFlip flip =
            (sens == 0) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
        obj.image.texture = creer_texture_tournee_rapide(
            gs->rendu, texture_base, w, h, (double)rotation, flip);

        if (obj.image.texture) {
          ajouter_variante_cache(gs->textures, id, obj.image.texture, rotation);
        }
      }
    }
  }

  if (!obj.image.texture) {
    log_fmt(NiveauLogErreur, "Erreur: Texture introuvable pour '%s'", id);
    return;
  }

  ajouter_image_au_jeu(gs, obj);
}

void ajouter_image_au_tableau_batch(Gestionnaire *gs, char **id, int *x, int *y,
                                    int *w, int *h, int *sens, int *rotation,
                                    int *transparence, int taille) {
  if (!gs)
    return;
  if (taille <= 0)
    return;

  for (int i = 0; i < taille; i++) {
    ajouter_image_au_tableau(gs, id[i], x[i], y[i], w[i], h[i], sens[i],
                             rotation[i], transparence[i]);
  }
}