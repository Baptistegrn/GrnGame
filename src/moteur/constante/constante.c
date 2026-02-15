/*
 * Accesseurs pour les constantes globales du moteur.
 * Renvoie les valeurs depuis les structures du gestionnaire.
 */

#include "../../main.h"

/* Renvoie la position X de la souris */
int souris_x() {
    if (gs)
        return gs->entrees->souris_x;
    else {
        return 0;
    }
}

/* Renvoie la position Y de la souris */
int souris_y() {
    if (gs)
        return gs->entrees->souris_y;
    else {
        return 0;
    }
}

/* Renvoie true si le clic gauche vient juste d'etre presse */
bool souris_gauche_juste_presse() {
    if (gs)
        return gs->entrees->souris_juste_presse;
    else {
        return false;
    }
}

/* Renvoie true si le clic droit vient juste d'etre presse */
bool souris_droite_juste_presse() {
    if (gs)
        return gs->entrees->souris_droite_juste_presse;
    else {
        return false;
    }
}

/* Renvoie le scroll vertical de la molette */
int souris_scroll_vertical() {
    if (gs)
        return (int)gs->entrees->souris_scroll_y;
    else {
        return 0;
    }
}

/* Renvoie le scroll horizontal de la molette */
int souris_scroll_horizontal() {
    if (gs)
        return (int)gs->entrees->souris_scroll_x;
    else {
        return 0;
    }
}

/* Renvoie true si le clic gauche est maintenu enfonce */
bool souris_gauche_presse() {
    if (gs)
        return gs->entrees->souris_presse;
    else {
        return false;
    }
}

/* Renvoie true si le clic droit est maintenu enfonce */
bool souris_droite_presse() {
    if (gs)
        return gs->entrees->souris_droite_presse;
    else {
        return false;
    }
}

/* Renvoie le delta time entre les frames */
float dt() {
    if (gs)
        return gs->timing->dt;
    else {
        /* eviter division par 0 */
        return 0.0001;
    }
}

/* Renvoie le nombre d'images par seconde cible */
float nb_images() {
    if (gs)
        return gs->timing->fps;
    else {
        /*eviter divison par 0 */
        return 0.0001;
    }
}

/* Renvoie le compteur de frames depuis le debut */
Uint32 compteur_frame() {
    if (gs)
        return gs->timing->compteur_frames;
    else {
        return 0;
    }
}

/* Renvoie le decalage X pour les bandes noires */
int decalage_x() {
    if (gs)
        return lround((double)gs->fenetre->decalage_x / (double)gs->fenetre->coeff);
    else {
        return 0;
    }
}

/* Renvoie le decalage Y pour les bandes noires */
int decalage_y() {
    if (gs)
        return lround((double)gs->fenetre->decalage_y / (double)gs->fenetre->coeff);
    else {
        return 0;
    }
}

/* Renvoie la largeur actuelle de la fenetre */
int largeur_actuelle() {
    if (gs)
        return gs->fenetre->largeur_actuelle;
    else {
        return 0;
    }
}

/* Renvoie la hauteur actuelle de la fenetre */
int hauteur_actuelle() {
    if (gs)
        return gs->fenetre->hauteur_actuelle;
    else {
        return 0;
    }
}

/* Renvoie la largeur de l'univers de jeu */
int largeur_univers() {
    if (gs)
        return gs->fenetre->largeur_univers;
    else {
        return 0;
    }
}

/* Renvoie la hauteur de l'univers de jeu */
int hauteur_univers() {
    if (gs)
        return gs->fenetre->hauteur_univers;
    else {
        return 0;
    }
}

/* Renvoie le coefficient de mise a l'echelle */
int coeff() {
    if (gs)
        return gs->fenetre->coeff;
    else {
        return 0;
    }
}

/* renvoie si la fenetre est minimise */
bool fenetre_est_minimise() {
    if (gs)
        return gs->timing->en_pause;
    else {
        return false;
    }
}