/*
* Authors: Corentin GOETGHEBEUR, Chris ARRIDI
* Ce module contient la structure transaction et les fonctions correspondantes.
*/

#include <stdio.h>
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
    size_t code_retour;
    printf("Test ouvrir\n");
    char filename[] = "Files/liste_transactions";
    ouvrir(&f, filename);
    if(f == NULL){
        printf("Erreur ouverture du fichier\n");
    }else{
        printf("ouverture du fichier ok\n");
    }
    printf("Test fwrite\n");
    
    // Ecriture du fichier
    fprintf(f, "%i %i %i ", transaction.date.jour, transaction.date.mois, transaction.date.annee); //date
    fprintf(f, "%f ", transaction.montant);  // montant
    fprintf(f, "%li ", strlen(transaction.label)); // taille du label
    fprintf(f, "%s ", transaction.label);    // label
    fprintf(f, "%li ", strlen(transaction.nom)); // taille du nom
    fprintf(f, "%s \n", transaction.nom);    // nom


    printf("test fermer\n");
    fermer(f);
    return code_retour;
}


/*
* Cette fonction lit une transaction depuis le fichier fourni.
*/
Transaction read_transaction(FILE *f){
    int jour, mois, annee, taille;
    float montant;
    char label[31];
    char nom[50];

    fscanf(f, "%i %i %i ", &jour, &mois, &annee); // lecture de la date
    
    // Construction de la structure date
    Date d = {
        .jour = jour,
        .mois = mois, 
        .annee = annee
    };

    fscanf(f, "%f ", &montant); // récupérer montant
    fscanf(f, "%i ", &taille);  // récupérer la taille du label
    fgets(label, taille + 1, f);    // récupérer le label
    fscanf(f, "%i ", &taille);  // récupérer la taille du nom
    fgets(nom, taille + 1, f);  // récupérer le nom

    // Création de la transaction

    Transaction t = {
        .date = d,
        .montant = montant,
        .label = label,
        .nom = nom
    };

    return t;
}