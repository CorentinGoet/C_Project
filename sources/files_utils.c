
/*
* Authors: Corentin GOETGHEBEUR, Chris ARRIDI
* Ce module contient les fonctions d'aide à l'accès aux fichiers. 
*/

#include <stdio.h>
#include <stdlib.h>
#include "../headers/files_utils.h"

void ouvrir(FILE **f, char* nom){
    *f = fopen(nom, "r+");   // On ouvre le fichier en lecture écriture avec ajout (r+) et pas remplacement (w+)
};
void fermer(FILE *f){
    fclose(f);
};