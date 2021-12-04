/*
* Authors: Corentin GOETGHEBEUR, Chris ARRIDI
* Ce module contient la structure transaction et les fonctions correspondantes.
*/

#include <stdio.h>
#include "../headers/date.h"
#include "../headers/transactions.h"
#include "../headers/files_utils.h"

/* Rappel structure Transaction:
*  =============================
*    typedef struct Transaction{
*        Date date;  // date de la transaction
*        int montant;    // montant de la transaction
*        char label[30]; // description
*        char nom[50];   // nom du receveur d'argent
*    } Transaction;
*/


Transaction creation_transaction(Date date, float montant, char* label, char* nom){
    Transaction transaction = {
        .date = date, 
        .montant = montant,
        .label = label, 
        .nom = nom
        };
    return transaction;
}

int ajout_transaction(FILE *f, Transaction transaction){
    size_t code_retour;
    printf("Test ouvrir\n");
    ouvrir(&f, "Files/test");
    printf("Test fwrite\n");
    code_retour = fwrite(&transaction, 1, sizeof(transaction), f);
    printf("test fermer\n");
    fermer(f);
    return code_retour;
}