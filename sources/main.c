
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

#include "../headers/compte.h"
#include "../headers/date.h"
#include "../headers/files_utils.h"
#include "../headers/transactions.h"



int main(int argc, char const *argv[])
{
    // Test date
    Date d;
    date(&d);
    printf("Test date: %i / %i / %i \n", d.jour, d.mois, d.annee);

    // Test compte
    Compte c = {
        .nom = "Corentin",
        .num_compte = 1
    };
    printf("Test compte: \n Le compte n°%d appartient a %s.\n", c.num_compte, c.nom);

    // Test transaction (inclut le test pour files_utils)
    printf("Test transaction: \n");
    Transaction t = {
        .date= d,
        .montant = 1500,
        .label="Test_Label",
        .nom="Corentin"
    };
    printf("La transaction a eu lieu le %i / %i / %i. \n", t.date.jour, t.date.mois, t.date.annee);
    printf("%s a recu %f euros.\n", t.nom, t.montant);
    printf("label : %s\n", t.label);

    // Test creation_transaction
    Transaction t2 = creation_transaction(d, 1500, "Test transaction", "Corentin");
    printf("La transaction a eu lieu le %i / %i / %i. \n", t2.date.jour, t2.date.mois, t2.date.annee);
    printf("%s a recu %f euros.\n", t2.nom, t2.montant);
    printf("label : %s\n", t2.label);

    // Test ajout_transaction + ouvrir + fermer
    FILE *f;
    ajout_transaction(f, t2);

}

