import subprocess
from ..utilitaires.systeme import renvoie_systeme

def installer_xmake(systeme):
    try:
        # commande sur la doc xmake
        if systeme in ("linux", "mac"):
            try:
                subprocess.run(
                    ["bash", "-c", "curl -fsSL https://xmake.io/shget.text | bash"],
                    check=True
                )
            except subprocess.CalledProcessError:
                subprocess.run(
                    ["bash", "-c", "wget https://xmake.io/shget.text -O - | bash"],
                    check=True
                )

        elif systeme == "windows":
            subprocess.run(
                ["powershell", "-Command", "irm https://xmake.io/psget.text | iex"],
                check=True,
                shell=True
            )

        else:
            raise RuntimeError("Système non supporté pour installation auto de xmake.")

    except subprocess.CalledProcessError:
        raise RuntimeError("Échec de l'installation automatique de xmake.")


def installation_xmake():
    installer_xmake(renvoie_systeme())