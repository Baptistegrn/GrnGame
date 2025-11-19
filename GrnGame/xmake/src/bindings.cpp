#include "logging.h"
#include "main.h"
#include <cstdio>
#include <functional>
#include <nanobind/nanobind.h>
#include <nanobind/stl/function.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/vector.h>
#include <stdexcept>
#include <nanobind/nanobind.h>
#include <nanobind/stl/vector.h>   // pour std::vector
#include <nanobind/stl/tuple.h>    // pour nb::make_tuple
#include <nanobind/stl/string.h>   // si tu manipules des strings

struct BlocWrapper {
    float x, y, w, h;
    BlocWrapper(const Bloc &b) : x(b.x), y(b.y), w(b.w), h(b.h) {}
    Bloc to_bloc() const { return Bloc{x,y,w,h}; }
};

namespace nb = nanobind;
using namespace nb::literals;

static nb::object gUpdateCallback;

static void update_trampoline(Gestionnaire *gestionnaire) {
    if (gUpdateCallback && !gUpdateCallback.is_none()) {
        gUpdateCallback(gestionnaire);
    }
}


// pour passer de la classe vector a un tableau c avec la taille en plus 
void platformer_2d_wrapper(float dt, float pos_x, float pos_y, float vitesse_y, bool en_air,
                           float larg_joueur, float haut_joueur,
                           const std::vector<Bloc> &blocs,
                           float gravite, float force_saut,
                           float vitesse_max_chute, float correction_mur,
                           bool activer_saut,
                           float &out_x, float &out_y, float &out_vy,
                           bool &out_en_air, bool &collision_gauche, bool &collision_droite)
{
    out_x = pos_x;
    out_y = pos_y;
    out_vy = vitesse_y;

    platformer_2d(dt, &out_x, &out_y, &out_vy, &en_air,
                  larg_joueur, haut_joueur,
                  blocs.data(), (int)blocs.size(),
                  gravite, force_saut, vitesse_max_chute, correction_mur,
                  activer_saut,
                  &collision_gauche, &collision_droite);

    out_en_air = en_air;
}
// pour renvoyer tuple python
nb::tuple platformer_2d_tuple_list_of_lists(
    float dt, float pos_x, float pos_y, float vitesse_y, bool en_air,
    float larg_joueur, float haut_joueur,
    nb::list blocs_py,  // liste de listes Python
    float gravite, float force_saut,
    float vitesse_max_chute, float correction_mur,
    bool activer_saut = false
) {
    std::vector<Bloc> blocs_cpp;
    for (auto item : blocs_py) {
        nb::sequence seq = nb::cast<nb::sequence>(item);

        blocs_cpp.push_back({
            nb::cast<float>(seq[0]),
            nb::cast<float>(seq[1]),
            nb::cast<float>(seq[2]),
            nb::cast<float>(seq[3])
        });
    }

    float out_x, out_y, out_vy;
    bool out_en_air;
    bool collision_gauche, collision_droite;

    platformer_2d_wrapper(
        dt, pos_x, pos_y, vitesse_y, en_air,
        larg_joueur, haut_joueur, blocs_cpp,
        gravite, force_saut, vitesse_max_chute, correction_mur,
        activer_saut,
        out_x, out_y, out_vy,
        out_en_air, collision_gauche, collision_droite
    );

    return nb::make_tuple(out_x, out_y, out_vy, out_en_air, collision_gauche, collision_droite);
}


NB_MODULE(LibGrnGame, m) {

#ifdef NDEBUG
    nb::set_leak_warnings(false);
#endif
    // logging
    nb::enum_<NiveauLog>(m, "NiveauLog")
        .value("Debug", NiveauLogDebug)
        .value("Info", NiveauLogInfo)
        .value("Avertissement", NiveauLogAvertissement)
        .value("Erreur", NiveauLogErreur);


    m.def("log_message", &log_message);

    //init
    m.def("initialisation",
          [](int hauteur, int largeur, float fps, int coeff,
             std::string &lien_image, std::string &lien_son, bool bande_noir,
             std::string &nom_fenetre, std::string &chemin_log) {
              return initialisation(hauteur, largeur, fps, coeff,
                                    lien_image.data(),
                                    lien_son.data(),
                                    bande_noir,
                                    nom_fenetre.data(),
                                    chemin_log.data());
          });

    m.def("boucle_principale", &boucle_principale);

    m.def("set_update_callback", [](nb::object fn) {
        gUpdateCallback = fn;
        set_update_callback([](Gestionnaire *jeu) {
            if (gUpdateCallback && !gUpdateCallback.is_none()) {
                gUpdateCallback(jeu);
            } else {
                throw std::runtime_error("A donner None en tant que update callback");
            }
        });
    });

    m.def("redimensionner_fenetre", &redimensionner_fenetre);
    m.def("colorier", &colorier);
    m.def("ecrire_console",&ecrire_dans_console);
    m.def("Seticone",[](Gestionnaire* gs,std::string &lien){return Seticone(gs, lien.data());});
    // clav
    m.def("touche_juste_presse", [](Gestionnaire* g, const std::string &touche_nom) {
        return touche_juste_presse(g, touche_nom.data());
    });
    m.def("touche_enfoncee", [](Gestionnaire* g, const std::string &touche_nom) {
        return touche_enfoncee(g, touche_nom.data());
    });

    // mannette
    m.def("touche_mannette_enfoncee", [](Gestionnaire* g, const std::string &touche_nom) {
        return touche_mannette_enfoncee(g, touche_nom.data());
    });
    m.def("touche_mannette_juste_presse", [](Gestionnaire* g, const std::string &touche_nom) {
        return touche_mannette_juste_presse(g, touche_nom.data());
    });

    m.def("init_controller_joysticks",&init_controller_joysticks);

    m.def("renvoie_joysticks", [](Gestionnaire* g, float dead_zone = 0.1f) {
        float* ptr = renvoie_joysticks(g->entrees, dead_zone);
        return std::vector<float>(ptr, ptr + 6);
    });

    m.def("fermer_controller_joystick", [](Gestionnaire* g) {
        fermer_controller(g);
        fermer_joystick(g);
    });
    m.def("ajouter_forme", &ajouter_forme_au_tableau);

    m.def("ajouter_forme_batch", [](Gestionnaire* g,
                                    std::vector<float> xs, std::vector<float> ys,
                                    std::vector<float> ws, std::vector<float> hs,
                                    std::vector<int> sens, std::vector<int> rotations,
                                    std::vector<int> transparences, std::vector<int> rs,
                                    std::vector<int> gs , std::vector<int> bs ) {
        size_t taille = xs.size();
        ajouter_forme_au_tableau_batch(g,
                                       xs.data(), ys.data(), ws.data(), hs.data(),
                                       sens.data(), rotations.data(), transparences.data(),
                                       rs.data(), gs.data(), bs.data(),
                                       static_cast<int>(taille));
    });

    //image
    m.def("ajouter_image", [](Gestionnaire* g, const std::string &lien, float x, float y, float w, float h,
                              int sens = 0, int rotation = 0, int transparence = 255) {
        ajouter_image_au_tableau(g, const_cast<char*>(lien.data()), x, y, w, h, sens, rotation, transparence);
    });

    m.def("ajouter_image_batch", [](Gestionnaire* g,
                                    std::vector<std::string> ids,
                                    std::vector<float> xs, std::vector<float> ys,
                                    std::vector<float> ws, std::vector<float> hs,
                                    std::vector<int> sens , std::vector<int> rotations ,
                                    std::vector<int> transparences ) {
        size_t taille = ids.size();
        std::vector<char*> ids_c(taille);
        for (size_t i = 0; i < taille; ++i)
            ids_c[i] = const_cast<char*>(ids[i].data());
        ajouter_image_au_tableau_batch(g,
                                       ids_c.data(), xs.data(), ys.data(), ws.data(), hs.data(),
                                       sens.data(), rotations.data(), transparences.data(),
                                       static_cast<int>(taille));
    });

    //texte
    m.def("ajouter_mot", [](Gestionnaire* g, const std::string &lien, const std::string &mot,
                             float x, float y, float coeff, int sens = 0, float ecart = 0,
                             int rotation = 0, int alpha = 255) {
        ajouter_mot_dans_tableau(g,
                                 const_cast<char*>(lien.data()),
                                 const_cast<char*>(mot.data()),
                                 x, y, coeff, sens, ecart, rotation, alpha);
    });

    //son
    m.def("jouer_son", [](Gestionnaire* g, const std::string &lien, int boucle = 0, int canal = -1, int volume = 64) {
        jouer_son(g, const_cast<char*>(lien.data()), boucle, canal, volume);
    });
    m.def("arreter_son", [](Gestionnaire* g, const std::string &lien) {
        arreter_son(g, const_cast<char*>(lien.data()));
    });

    m.def("pause_son", [](Gestionnaire* g, const std::string &lien) {
        pause_son(g, const_cast<char*>(lien.data()));
    });

    m.def("reprendre_son", [](Gestionnaire* g, const std::string &lien) {
        reprendre_son(g, const_cast<char*>(lien.data()));
    });

    m.def("arreter_canal", &arreter_canal);
    m.def("pause_canal", &pause_canal);
    m.def("reprendre_canal", &reprendre_canal);


    m.def("charger_tous_les_sons", [](Gestionnaire* g, const std::string &dossier) {
        charger_tous_les_sons(g->sons, dossier.data());
    });

    m.def("charger_toutes_les_textures", [](Gestionnaire* g, const std::string &dossier) {
        charger_toutes_les_textures(g->textures, dossier.data());
    });

    m.def("liberer_gestionnaire_son", [](Gestionnaire* g) {
        liberer_gestionnaire_son(g->sons);
    });

    m.def("liberer_gestionnaire_image", [](Gestionnaire* g) {
        liberer_gestionnaire_image(g->textures);
    });

    m.def("platformer_2d", &platformer_2d_tuple_list_of_lists);

    // atribut pour acceder a ces parametres (constante) depuis python
    auto entrees_class = nb::class_<GestionnaireEntrees>(m, "GestionnaireEntrees")
        .def_ro("mouse_x", &GestionnaireEntrees::mouse_x)
        .def_ro("mouse_y", &GestionnaireEntrees::mouse_y)
        .def_ro("mouse_pressed", &GestionnaireEntrees::mouse_pressed)
        .def_ro("mouse_just_pressed", &GestionnaireEntrees::mouse_just_pressed)
        .def_ro("mouse_right_pressed", &GestionnaireEntrees::mouse_right_pressed)
        .def_ro("mouse_right_just_pressed", &GestionnaireEntrees::mouse_right_just_pressed);

    auto gestionnaire_class = nb::class_<Gestionnaire>(m, "Gestionnaire")
        .def_ro("run", &Gestionnaire::run)
        .def_ro("dt", &Gestionnaire::dt)
        .def_ro("fps", &Gestionnaire::fps)
        .def_ro("largeur", &Gestionnaire::largeur)
        .def_ro("hauteur", &Gestionnaire::hauteur)
        .def_ro("coeff_minimise", &Gestionnaire::coeff_minimise)
        .def_ro("largeur_actuel", &Gestionnaire::largeur_actuel)
        .def_ro("hauteur_actuel", &Gestionnaire::hauteur_actuel)
        .def_ro("decalage_x", &Gestionnaire::decalage_x)
        .def_ro("decalage_y", &Gestionnaire::decalage_y)
        .def_ro("plein_ecran", &Gestionnaire::plein_ecran)
        .def_ro("temps_frame", &Gestionnaire::temps_frame)
        .def_ro("entrees", &Gestionnaire::entrees);

}


