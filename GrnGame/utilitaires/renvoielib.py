
from .systeme import renvoie_systeme
def renvoie_lib():
    systeme = renvoie_systeme()


    if systeme == "windows":
        lib_name = "LibGrnGame.pyd"
    elif systeme == "linux":
        lib_name = "LibGrnGame.so"
    elif systeme == "darwin":
        lib_name = "LibGrnGame.dylib"
    else:
        raise OSError(f"Système non pris en charge : {systeme}")

    return lib_name
