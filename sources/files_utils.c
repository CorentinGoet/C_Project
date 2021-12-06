
/*
* Authors: Corentin GOETGHEBEUR, Chris ARRIDI
* Ce module contient les fonctions d'aide à l'accès aux fichiers. 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../headers/files_utils.h"

void ouvrir(FILE **f, char nom[]){
    
    *f = fopen(nom, "r+");   // On ouvre le fichier en lecture écriture avec ajout (r+) et pas remplacement (w+)
    
    if(*f != NULL){
        printf("ouvrir termine\n");
    }else{
        printf("Erreur d'ouverture du fichier : %s\n", nom);
    }
};
void fermer(FILE *f){
    fclose(f);
};