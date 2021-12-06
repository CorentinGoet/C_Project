
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

#include "../headers/compte.h"
#include "../headers/date.h"
#include "../headers/files_utils.h"
#include "../headers/transactions.h"
#include "../headers/entete.h"
#include "../headers/menu.h"



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
    printf("test strcat\n");
    char test_str1[] = "partie 1";
    char test_str2[] = "partie 2";
    char* test_str = strcat(test_str1, test_str2);
    printf("%s\n", test_str);

    ajout_transaction(f, t2);

    // Test de lecture de transaction
    ouvrir(&f, "Files/liste_transactions");
    read_transaction(f);


    // Test entete
    printf("Test entete \n");
    Entete e = {
        .date = d,
        .solde = 100
    };

    //printf("test creation_fichier\n");
    
    //FILE *f1 = creation_fichier(e, "test_entete");
    
    
    // tests menu

    // test check_num_compte
    printf("Test check_num_compte :\n");
    if (check_num_compte(2) == 1){
        printf("Compte disponible. \n");
    }else{
        printf("Numéro déjà pris. \n");
    }


    // test creer utilisateur
    creer_utilisateur("Corentin");



}

