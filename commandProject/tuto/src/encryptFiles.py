#!/usr/bin/env python3
import os
from Cryptodome.Cipher import AES
from Cryptodome.Util.Padding import pad
import argparse

# arguments 
parser = argparse.ArgumentParser(
    description="Creation of encrypted files (AES-128 CBC)"
)


parser.add_argument("key", help="Encryption key in HEX (32 hex chars = 16 bytes)")
parser.add_argument(
    "path",
    nargs="?",
    default=".",
    help="File you want to encrypt"
)
parser.add_argument("iv", help="IV in HEX (32 hex chars = 16 bytes)")


# recuperer les arguments
args = parser.parse_args()


def chiffrer_ressource(chemin_source, cle_hex, iv_hex):
    try:
        cle = bytes.fromhex(cle_hex)
        iv = bytes.fromhex(iv_hex)
    except ValueError:
        print("Error: key and iv must be valid HEX.")
        return

    if len(cle) != 16:
        print(f"Error: key must be 16 bytes (32 hex characters). Current: {len(cle)} bytes")
        return

    if len(iv) != 16:
        print("Error: iv must be 16 bytes (32 hex characters).")
        return

    if not os.path.isfile(chemin_source):
        print(f"Error: file '{chemin_source}' does not exist.")
        return

    try:
        with open(chemin_source, 'rb') as f:
            donnees_brutes = f.read()

        cipher = AES.new(cle, AES.MODE_CBC, iv)
        
        donnees_preparees = pad(donnees_brutes, AES.block_size)
        donnees_chiffrees = cipher.encrypt(donnees_preparees)

        #extension .data (choix arbitraire)
        nom_base = os.path.splitext(chemin_source)[0]
        chemin_destination = nom_base + ".data"

        with open(chemin_destination, 'wb') as f:
            f.write(donnees_chiffrees)

        print(f"--- Encryption Success ---")
        print(f"Source: {chemin_source}")
        print(f"Target: {chemin_destination}")
        print(f"Original size: {len(donnees_brutes)} bytes")
        print(f"Final size (padded): {len(donnees_chiffrees)} bytes")
        print(f"Padding added: {len(donnees_chiffrees) - len(donnees_brutes)} bytes")

    except Exception as e:
        print(f"Encryption error: {e}")

if __name__ == "__main__":
    chiffrer_ressource(args.path, args.key, args.iv)