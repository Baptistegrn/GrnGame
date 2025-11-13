import ctypes
from ctypes import c_int, c_float, c_bool, c_void_p, POINTER, c_double, c_char_p

TAILLE_LIEN_GT = 256
TAILLE_CANAL = 32

class FondActualiser(ctypes.Structure):
    _fields_ = [
        ("r", c_int), ("g", c_int), ("b", c_int),
        ("dessiner", c_bool),
        ("bande_noir", c_bool)
    ]

class GestionnaireEntrees(ctypes.Structure):
    _fields_ = [
        ("mouse_x", c_int), ("mouse_y", c_int),
        ("mouse_right_pressed", c_bool),
        ("mouse_right_just_pressed", c_bool),      
        ("mouse_pressed", c_bool),
        ("mouse_just_pressed", c_bool),
        ("keys", c_bool * 512),
        ("keys_pressed", c_bool * 512),
        ("quit", c_bool)
    ]

class Gestionnaire(ctypes.Structure):
    _fields_ = [
        ("run", c_bool),
        ("dt", c_float), ("fps", c_float),
        ("largeur", c_int), ("hauteur", c_int),
        ("coeff_minimise", c_int),
        ("largeur_actuel", c_int), ("hauteur_actuel", c_int),
        ("decalage_x", c_int), ("decalage_y", c_int),
        ("plein_ecran", c_bool),
        ("temps_frame", c_int), 
        ("fenetre", c_void_p), ("rendu", c_void_p),
        ("fond", POINTER(FondActualiser)),
        ("image", c_void_p),
        ("entrees", POINTER(GestionnaireEntrees)),
        ("textures", c_void_p),
        ("sons", c_void_p)
    ]

UpdateCallbackType = ctypes.CFUNCTYPE(None, POINTER(Gestionnaire))