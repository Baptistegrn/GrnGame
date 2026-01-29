#cle
key = 0x42
#obligatoire main.lua doit etre le seul fichier encrypter 
file_path = "main.lua"

signature = b"\x1BENC"  
with open(file_path, "rb") as f:
    data = f.read()

if data.startswith(signature):
    print(f"file already crypted : {file_path}.")
else:
    encrypted_data = signature + bytearray([b ^ key for b in data])
    with open(file_path, "wb") as f:
        f.write(encrypted_data)
    print(f"file is corretly crypted : {file_path}.")