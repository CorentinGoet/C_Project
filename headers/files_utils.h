/*
* Authors: Corentin GOETGHEBEUR, Chris ARRIDI
* Ce module contient les fonctions d'accès aux fichiers.
*/

#ifndef FILES_UTILS_H
#define FILES_UTILS_H

#include <stdio.h>

void ouvrir(FILE **f, char* nom);
void fermer(FILE *f);

#endif