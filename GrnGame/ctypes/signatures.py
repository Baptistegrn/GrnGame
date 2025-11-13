from ctypes import c_int, c_float, c_bool, c_void_p, POINTER, c_double, c_char_p
from .structures import Gestionnaire, GestionnaireEntrees,  UpdateCallbackType

def configurer_signatures(jeu):
    #icone

    jeu.Seticone.argtypes = [POINTER(Gestionnaire),c_char_p]
    jeu.Seticone.restype = None

    # Initialisation
    jeu.initialisation.argtypes = [c_int, c_int, c_float, c_int, c_char_p, c_char_p, 
                                    c_bool, c_char_p,c_char_p]
    jeu.initialisation.restype = POINTER(Gestionnaire)
    
    # Colorier
    jeu.colorier.argtypes = [POINTER(Gestionnaire), c_int, c_int, c_int]
    jeu.colorier.restype = None
    
    # Boucle et update
    jeu.update.argtypes = [POINTER(Gestionnaire)]
    jeu.update.restype = None
    
    jeu.boucle_principale.argtypes = [POINTER(Gestionnaire)]
    jeu.boucle_principale.restype = None
    
    jeu.liberer_jeu.argtypes = [POINTER(Gestionnaire)]
    jeu.liberer_jeu.restype = None
    
    # Images
    jeu.ajouter_image_au_tableau.argtypes = [POINTER(Gestionnaire), c_char_p, 
                                              c_float, c_float, c_float, c_float, 
                                              c_int, c_int,c_int]
    jeu.ajouter_image_au_tableau.restype = None
    
    # Batch
    jeu.ajouter_image_au_tableau_batch.argtypes = [POINTER(Gestionnaire), 
                                                     POINTER(c_char_p), POINTER(c_float),
                                                     POINTER(c_float), POINTER(c_float),
                                                     POINTER(c_float), POINTER(c_int),
                                                     POINTER(c_int),POINTER(c_int), c_int]
    jeu.ajouter_image_au_tableau_batch.restype = None
    #forme image
    jeu.ajouter_forme_au_tableau.argtypes = [POINTER(Gestionnaire), c_float, c_float, c_float, c_float, c_int, c_int, c_int, c_int, c_int, c_int]
    jeu.ajouter_forme_au_tableau_batch.argtypes = [POINTER(Gestionnaire), POINTER(c_float), POINTER(c_float), POINTER(c_float), POINTER(c_float), POINTER(c_int), POINTER(c_int), POINTER(c_int), POINTER(c_int), POINTER(c_int), POINTER(c_int), c_int]
    jeu.ajouter_forme_au_tableau.restype = None
    jeu.ajouter_forme_au_tableau_batch.restype = None
    # Sons
    jeu.jouer_son.argtypes = [POINTER(Gestionnaire), c_char_p, c_int, c_int,c_int]
    jeu.jouer_son.restype = None
    
    jeu.arreter_son.argtypes = [POINTER(Gestionnaire), c_char_p]
    jeu.arreter_son.restype = None
    
    jeu.arreter_canal.argtypes = [c_int]
    jeu.arreter_canal.restype = None
    
    jeu.pause_canal.argtypes = [c_int]
    jeu.pause_canal.restype = None
    
    jeu.pause_son.argtypes = [POINTER(Gestionnaire), c_char_p]
    jeu.pause_son.restype = None
    
    jeu.reprendre_canal.argtypes = [c_int]
    jeu.reprendre_canal.restype = None
    
    jeu.reprendre_son.argtypes = [POINTER(Gestionnaire), c_char_p]
    jeu.reprendre_son.restype = None
    
    # Touches
    jeu.touche_juste_presse.argtypes = [POINTER(Gestionnaire), c_char_p]
    jeu.touche_juste_presse.restype = c_bool
    
    jeu.touche_enfoncee.argtypes = [POINTER(Gestionnaire), c_char_p]
    jeu.touche_enfoncee.restype = c_bool
    
    jeu.touche_mannette_enfoncee.argtypes = [POINTER(Gestionnaire), c_char_p]
    jeu.touche_mannette_enfoncee.restype = c_bool
    
    jeu.touche_mannette_juste_presse.argtypes = [POINTER(Gestionnaire), c_char_p]
    jeu.touche_mannette_juste_presse.restype = c_bool
    
    # Controller/Joystick
    jeu.init_controller_joysticks.argtypes = [POINTER(Gestionnaire), c_int]
    jeu.init_controller_joysticks.restype = c_bool
    
    jeu.fermer_controller.argtypes = [POINTER(Gestionnaire)]
    jeu.fermer_controller.restype = None
    
    jeu.renvoie_joysticks.argtypes = [POINTER(GestionnaireEntrees), c_float]
    jeu.renvoie_joysticks.restype = POINTER(c_float)
    
    jeu.fermer_joystick.argtypes = [POINTER(Gestionnaire)]
    jeu.fermer_joystick.restype = None
    
    # Maths
    jeu.random_jeu.argtypes = [c_int, c_int]
    jeu.random_jeu.restype = c_int
    
    jeu.abs_val.argtypes = [c_double]
    jeu.abs_val.restype = c_double
    
    jeu.clamp.argtypes = [c_double, c_double, c_double]
    jeu.clamp.restype = c_double
    
    jeu.pow_custom.argtypes = [c_double, c_double]
    jeu.pow_custom.restype = c_double
    
    jeu.sqrt_custom.argtypes = [c_double]
    jeu.sqrt_custom.restype = c_double
    
    jeu.cbrt_custom.argtypes = [c_double]
    jeu.cbrt_custom.restype = c_double
    
    jeu.exp_custom.argtypes = [c_double]
    jeu.exp_custom.restype = c_double
    
    jeu.log_custom.argtypes = [c_double]
    jeu.log_custom.restype = c_double
    
    jeu.log10_custom.argtypes = [c_double]
    jeu.log10_custom.restype = c_double
    
    jeu.log2_custom.argtypes = [c_double]
    jeu.log2_custom.restype = c_double
    
    jeu.sin_custom.argtypes = [c_double]
    jeu.sin_custom.restype = c_double
    
    jeu.cos_custom.argtypes = [c_double]
    jeu.cos_custom.restype = c_double
    
    jeu.tan_custom.argtypes = [c_double]
    jeu.tan_custom.restype = c_double
    
    jeu.asin_custom.argtypes = [c_double]
    jeu.asin_custom.restype = c_double
    
    jeu.acos_custom.argtypes = [c_double]
    jeu.acos_custom.restype = c_double
    
    jeu.atan_custom.argtypes = [c_double]
    jeu.atan_custom.restype = c_double
    
    jeu.atan2_custom.argtypes = [c_double, c_double]
    jeu.atan2_custom.restype = c_double
    
    jeu.sinh_custom.argtypes = [c_double]
    jeu.sinh_custom.restype = c_double
    
    jeu.cosh_custom.argtypes = [c_double]
    jeu.cosh_custom.restype = c_double
    
    jeu.tanh_custom.argtypes = [c_double]
    jeu.tanh_custom.restype = c_double
    
    jeu.asinh_custom.argtypes = [c_double]
    jeu.asinh_custom.restype = c_double
    
    jeu.acosh_custom.argtypes = [c_double]
    jeu.acosh_custom.restype = c_double
    
    jeu.atanh_custom.argtypes = [c_double]
    jeu.atanh_custom.restype = c_double
    
    jeu.floor_custom.argtypes = [c_double]
    jeu.floor_custom.restype = c_double
    
    jeu.ceil_custom.argtypes = [c_double]
    jeu.ceil_custom.restype = c_double
    
    jeu.round_custom.argtypes = [c_double]
    jeu.round_custom.restype = c_double
    
    jeu.trunc_custom.argtypes = [c_double]
    jeu.trunc_custom.restype = c_double
    
    jeu.fmod_custom.argtypes = [c_double, c_double]
    jeu.fmod_custom.restype = c_double
    
    jeu.hypot_custom.argtypes = [c_double, c_double]
    jeu.hypot_custom.restype = c_double
    
    # Fenêtre et affichage
    jeu.redimensionner_fenetre.argtypes = [POINTER(Gestionnaire)]
    jeu.redimensionner_fenetre.restype = None
    
    jeu.ecrire_dans_console.argtypes = [c_char_p]
    jeu.ecrire_dans_console.restype = None
    
    jeu.ajouter_mot_dans_tableau.argtypes = [POINTER(Gestionnaire), c_char_p, c_char_p,
                                              c_float, c_float, c_float, c_int, 
                                              c_float, c_int,c_int]
    jeu.ajouter_mot_dans_tableau.restype = None
    
    # Callback
    jeu.set_update_callback.argtypes = [UpdateCallbackType]
    jeu.set_update_callback.restype = None