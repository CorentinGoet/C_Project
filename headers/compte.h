/*
* Authors: Corentin GOETGHEBEUR, Chris ARRIDI
* Ce module définit une structure de compte bancaire.
*
*/

#ifndef COMPTE_H
#define COMPTE_H

typedef struct Compte{
    char nom[50];   // Nom du propriétaire du compte
    int num_compte;  // Numéro du compte
} Compte;

#endif