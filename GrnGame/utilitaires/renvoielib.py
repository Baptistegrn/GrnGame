
from .systeme import renvoie_systeme
def renvoie_lib():
    systeme = renvoie_systeme()


    if systeme == "windows":
        lib_name = "libjeu.dll"
    elif systeme == "linux":
        lib_name = "libjeu.so"
    elif systeme == "darwin":
        lib_name = "libjeu.dylib"
    else:
        raise OSError(f"Système non pris en charge : {systeme}")

    return lib_name
