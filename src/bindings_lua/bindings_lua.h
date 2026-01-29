#ifndef BINDINGS_LUA_H
#define BINDINGS_LUA_H

void initialiser_lua(const char *fichier_chemin_lua);
void recharger_lua(const char *chemin);
void actualiser_rechargement();

#endif /* BINDINGS_LUA_H */