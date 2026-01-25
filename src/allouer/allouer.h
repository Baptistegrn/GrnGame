#ifndef MALLOC_H
#define MALLOC_H

void *xmalloc(size_t taille);
void *xrealloc(void *bloc, size_t taille);
void xfree(void *ptr);
#endif