/*
* Authors: Corentin GOETGHEBEUR, Chris ARRIDI
* Ce module contient la structure transaction et les fonctions correspondantes.
*/

#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <stdio.h>
#include "date.h"

typedef struct Transaction{
    Date date;  // date de la transaction
    float montant;    // montant de la transaction
    char *label; // description
    char *nom;   // nom du receveur d'argent
} Transaction;

Transaction creation_transaction(Date date, float montant, char label[30], char nom[50]);

int ajout_transaction(FILE *f, Transaction transaction);

int read_transaction(FILE *f, Transaction *t);
#endif