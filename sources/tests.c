/*
* Authors: Corentin GOETGHEBEUR, Chris ARRIDI
* Ce fichier contient les fonction de tests de toutes les méthodes du projet.
*/

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


/**
 * @brief Fonction de tests du module date.h, la fonction test l'initialisation d'une valeur et la fonction date.
 * 
 * @param details affiche les détails des tests si non 0.
 * @return int 
 */
int testDate(int details){
    Date d = {
        .jour = 2,
        .mois = 5,
        .annee = 2021
    };

    if (details) printf("Création directe ok !\n");

    if( d.jour != 2 || d.mois != 5 || d.annee != 2021){
        return 1;
    }
    
    if (details) printf("Validité des infos ok ! \n");
    Date d1;
    date(&d1);
    if (details) printf("Test fonction date: %i / %i / %i\n", d1.jour, d1.mois, d1.annee);

    if (date_comp(d, d1) != -1){
        printf("Problème date_comp.\n");
        return 2;
    }
    return 0;
}

/**
 * @brief Cette fonction teste le module compte.
 * 
 * @param details affiche les détails du test
 * @return int 
 */
int testCompte(int details){

    Compte compte = {
        .nom = "Corentin",
        .num_compte = 25
    };

    if (details) printf("Création directe ok !\n");
    if (strcmp(compte.nom, "Corentin") != 0 || compte.num_compte != 25) return 1;
    if (details) printf("Validité des infos ok ! \n");
    return 0;
}

/**
 * @brief Fonction de tests du module Transaction
 * 
 * @param details affiche les détails
 * @return int 
 */
int testTransaction(int details){
    // Création de la date associée à la transaction
    Date d = {
        .jour = 2,
        .mois = 5,
        .annee = 2021
    };

    // test creation directe
    Transaction t1 = {
        .date= d,
        .montant = 1500,
        .label="Test Label",
        .nom="Corentin"
    };

    if (details) printf("Création directe ok !\n");

    // test creation_transaction
    Transaction t2 = creation_transaction(d, 1500, "Test Label", "Corentin");
    if (date_comp(t2.date, t1.date) != 0 || t2.montant != t1.montant || strcmp(t1.label, t2.label) != 0 || strcmp(t1.nom, t2.nom) != 0){
        printf("Problème creation_transaction.\n");
        return 1;
    }
    if (details) printf("Fonction creation_transaction ok !\n");

    // test ajout_transaction
    FILE *f;
    ouvrir(&f, "Tests/tests_transactions");
    ajout_transaction(f, t2);
    fermer(f);
    if (details) printf("Fonction ajout_transaction ok !\n");

    // test read_transaction
    FILE *f2;
    ouvrir(&f2, "Tests/tests_transactions");
    Transaction t3;
    read_transaction(f2, &t3);
    fermer(f2);

    printf("test date : %i %i %i \n", t3.date.jour, t3.date.mois, t3.date.annee);
    printf("Test montant: %f \n", t3.montant);
    printf("Test label : %s \n", t3.label);
    printf("test nom : %s \n", t3.nom);

    if (date_comp(t2.date, t3.date) != 0 || t2.montant != t3.montant || strcmp(t3.label, t2.label) != 0 || strcmp(t3.nom, t2.nom) != 0){
        printf("Probleme ajout_transaction ou read_transaction.\n");
        return 1;
    }

    if (details) printf("Fonction read_transaction ok! \n");
    return 0;
}

/**
 * @brief Tests du module files_utils
 * 
 * @param details affichage des détails
 * @return int 
 */
int testFiles(int details){
    FILE *f;
    
    ouvrir(&f, "Tests/test_ouverture");
    if(details) printf("Fonction ouvrir ok !\n");

    fermer(f);
    if(details) printf("Fonction fermer ok !\n");

    return 0;
}

int main(int argc, char const *argv[])
{
    
    int details = 0;
    if(argc > 1){
        details = argv[1];
    }

    printf("=========================================\n");
    printf("Programme de tests du projet:\n");

    printf("-----------------------------------------\n");
    printf("Tests du module Date ... \n");
    if (testDate(details) == 0){
        printf("Module Date ok!\n");
    }else{
        printf("Problème module date. \n");
        return 1;
    }

    printf("-----------------------------------------\n");
    printf("Tests du module Compte ... \n");
    if (testCompte(details) == 0){
        printf("Module Compte ok!\n");
    }else{
        printf("Problème module Compte. \n");
        return 1;
    }

    printf("-----------------------------------------\n");
    printf("Tests du module Files_utils ... \n");
    if (testFiles(details) == 0){
        printf("Module files_utils ok!\n");
    }else{
        printf("Problème module files_utils. \n");
        return 1;
    }

    printf("-----------------------------------------\n");
    printf("Tests du module Transaction ... \n");
    if (testTransaction(details) == 0){
        printf("Module Transaction ok!\n");
    }else{
        printf("Problème module Transaction. \n");
        return 1;
    }

    
    return 0;
}
