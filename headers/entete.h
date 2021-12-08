/*
* Authors : Corentin GOETGHEBEUR, Chris ARRIDI
* Ce module contient la structure entete et les fonction associées permettant de modéliser un en-tete de compte bancaire.
*/

#ifndef ENTETE_H
#define ENTETE_H

#include "date.h"
#include <stdio.h>

typedef struct Entete{
    Date date;
    float solde;
}Entete;

Entete creation_entete(Date date, float solde);
FILE* creation_fichier(Entete entete, char* nom);
//void mise_a_jour_solde(FILE *f, Date d);

void read_entete(FILE *f, Date *d, float *solde);

#endif