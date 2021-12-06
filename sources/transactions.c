/*
* Authors: Corentin GOETGHEBEUR, Chris ARRIDI
* Ce module contient la structure transaction et les fonctions correspondantes.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    
    // Ecriture du fichier
    fprintf(f, "%i %i %i ", transaction.date.jour, transaction.date.mois, transaction.date.annee); //date
    fprintf(f, "%f ", transaction.montant);  // montant
    fprintf(f, "%li ", strlen(transaction.label)); // taille du label
    fprintf(f, "%s ", transaction.label);    // label
    fprintf(f, "%li ", strlen(transaction.nom)); // taille du nom
    fprintf(f, "%s \n", transaction.nom);    // nom
}


/*
* Cette fonction lit une transaction depuis le fichier fourni.
*/
int read_transaction(FILE *f, Transaction *t){
    int jour, mois, annee, taille;
    float montant;
    char *label = malloc(31 * sizeof(char));
    char *nom = malloc(50 * sizeof(char));

    fscanf(f, "%i %i %i ", &jour, &mois, &annee); // lecture de la date

    fscanf(f, "%f ", &montant); // récupérer montant
    fscanf(f, "%i ", &taille);  // récupérer la taille du label
    fgets(label, taille + 1, f);    // récupérer le label
    fscanf(f, "%i ", &taille);  // récupérer la taille du nom
    fgets(nom, taille + 1, f);  // récupérer le nom

    // Création de la transaction

    t->date.jour = jour;
    t->date.mois = mois;
    t->date.annee = annee;
    t->montant = montant;
    t->label = label;
    t->nom = nom;

    return 0;
}