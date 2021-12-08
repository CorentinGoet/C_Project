/*
* Authors: Corentin GOETGHEBEUR, Chris ARRIDI
* Ce module contient la structure Entete et les fonctions correspondantes permettant de modéliser un en-tete de compte bancaire.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../headers/date.h"
#include "../headers/transactions.h"
#include "../headers/files_utils.h"

#include "../headers/entete.h"

/* Rappel structure Entete:
* ============================
*    typedef struct Entete{
*        Date date;
*        float solde;
*    }Entete;
*/


Entete creation_entete(Date date, float solde){
    Entete entete = {
        .date = date,
        .solde = solde
    };
    return entete;
}

FILE* creation_fichier(Entete entete, char* nom){
    
    FILE *f = fopen(nom, "w+");   // On place le fichier dans le dossier files/comptes, on l'ouvre avec w+ pour le créer
    write_entete(f, entete);
    return f;
}

int read_entete(FILE *f, Entete *e){
    int jour, mois, annee;
    float s;
    if(fscanf(f, "%i %i %i %f \n", &jour, &mois, &annee, &s) != 4) return 1;
    e->date.jour = jour;
    e->date.mois = mois;
    e->date.annee = annee;
    e->solde = s;
    return 0;
}

int write_entete(FILE *f, Entete e){
    if(fprintf(f, "%i %i %i %f \n", e.date.jour, e.date.mois, e.date.annee, e.solde) != 4){
        return 1;
    }
    return 0;
}



/*
void mise_a_jour_solde(FILE *f, Date d){

    // Récupération des données depuis le fichier des transactions
    FILE *transactions_file;
    ouvrir(&transactions_file, "Files/liste_transactions");
    
    Transaction t = read_transaction(transactions_file);


    
}
*/