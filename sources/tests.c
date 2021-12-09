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

/**
 * @brief Fonction de tests du module entete
 * 
 * @param details affiche les détails
 * @return int 
 */
int testEntete(int details){
    Date d;
    date(&d);

    // Test création
    Entete e1 = {
        .date = d,
        .solde = 150
    };

    if(details)printf("Création directe ok !\n");

    // Test creation_entete
    Entete e2 = creation_entete(d, 150);
    if(details)printf("Fonction creation_entete ok !\n");

    // Test validité
    if(e1.date.jour != e2.date.jour || e1.date.mois != e2.date.mois ||
        e1.date.annee != e2.date.annee || e1.solde != e2.solde)
        {
            printf("Problème creation_entete.\n");
            return 1;
        }
    
    if(details)printf("Cohérence entre les créations ok !\n");



    // Test creation_fichier

    FILE *f = creation_fichier(e1, "Tests/test_creation_fichier");
    fprintf(f, "%s", "fichier de test !\n");
    fclose(f);

    if(details)printf("Fonction creation_fichier + write_entete ok !\n");

    // Test read_entete
    FILE *f2;
    ouvrir(&f2, "Tests/test_creation_fichier");
    read_entete(f2, &e2);
    fermer(f2);

    if(e1.date.jour != e2.date.jour || e1.date.mois != e2.date.mois ||
        e1.date.annee != e2.date.annee || e1.solde != e2.solde)
        {
            printf("Problème fonction read_entete.\n");
            return 1;
        }
    

    if(details) printf("Fonction read_entete ok !\n");
    return 0;
}

/**
 * @brief Fonction de test du module menu.
 * 
 * @param details affiche les détails
 * @return int 
 */
int testMenu(int details){
    // Test check_num
    int res1, res2;
    res1 = check_num_compte(1234); // Le fichier Files/comptes/1234 existe arbitrairement pour ce test
    res2 = check_num_compte(1); // Le fichier Files/comptes/1 n'existe pas
    
    if((res1 != 0) || (res2 != 1)){
        printf("Problème dans la fonction check_num_compte (vérifier la présence des fichiers de test !)\n");
        
    }

    if(details)printf("Fonction check_num_compte ok !\n");
    
    // Test creer_utilisateur
    // Cette fonction crée un compte etc... elle ne peut pas être redirigée vers le dossier test.
    // Les fichiers créés par ce test doivent être supprimés !

    creer_utilisateur("Test_utilisateur");
    if(details)printf("Fonction creer_utilisateur ok!\n");
    printf("SUPPRIMER LES FICHIERS CREES PAR LE TEST !\n");

    //Test compte_de
    int res;
    res = compte_de("Corentin");
    if(res == -1){
        printf("Pas de compte trouvé pour le nom Corentin \n");
        printf("Problème fonction compte_de\n");
        return 1;
    }
    if (details) printf("Fonction compte_de ok !\n");

    // test nom_compte
    int num_compte = 150;
    char nom_c[50];
    nom_compte(num_compte, nom_c);
    printf("test : %s\n", nom_c);
    if (strcmp(nom_c, "Files/comptes/150") != 0){
        printf("Problème fonction nom_compte.\n");
        return 1;
    }
    if(details) printf("Fonction nom_compte ok !\n");

    // tests mise_a_jour_solde
    Date d_solde;
    date(&d_solde);
    d_solde.jour = d_solde.jour +1; // la transaction doit se passer après la date de l'entete
    num_compte = compte_de("test_maj");
    nom_compte(num_compte, nom_c);
    FILE *f;
    ouvrir(&f, nom_c);
    Transaction t;
    int montant = 150;
    t  = creation_transaction(d_solde, montant, "Test maj solde", "test_maj");
    ajout_transaction(f, t);
    fermer(f);
    ouvrir(&f, nom_c);
    mise_a_jour_solde("test_maj", d_solde);
    fermer(f);
    Entete e;
    FILE *f2;
    ouvrir(&f2, nom_c);
    read_entete(f2, &e);
    fermer(f2);
    if(e.solde != montant){
        printf("Problème mise_a_jour_solde sur le solde.\n");
        return 1;
    }
    date(&d_solde);

    if(date_comp(e.date, d_solde) != 0){
        printf("Problème mise_a_jour_solde sur la date.\n");
        return 1;
    }
    if(details)printf("Fonction mise_a_jour_solde ok !\n");

    // Test num2nom
    char nom[30];
    if(num2nom(1234, nom) != 0){
        printf("Problème fonction num2nom.\n");
        return 1;
    }
    if(strcmp(nom, "test_maj") != 0){
        printf("Problème fonction num2nom\n");
        printf("%s", nom);
        return 1;
    }
    if(details) printf("Fonction num2nom ok !\n");

    // Test virement de compte à compte
    Date d_virement;
    date(&d_virement);
    virement_de_compte_a_compte(1234, 12345, d_virement, 50);

    return 0;
}


int main(int argc, char const *argv[])
{
    
    int details = 0;
    if(argc > 1){
        details = atoi(argv[1]);
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

    printf("-----------------------------------------\n");
    printf("Tests du module Entete ... \n");
    if (testEntete(details) == 0){
        printf("Module Entete ok!\n");
    }else{
        printf("Problème module Entete. \n");
        return 1;
    }

    printf("-----------------------------------------\n");
    printf("Tests du module Menu ... \n");
    if (testMenu(details) == 0){
        printf("Module Menu ok!\n");
    }else{
        printf("Problème module Menu. \n");
        return 1;
    }

    printf("-----------------------------------------\n");
    printf("Tous les tests sont passés sans problèmes !\n");
    printf("=========================================\n");

    return 0;
}
