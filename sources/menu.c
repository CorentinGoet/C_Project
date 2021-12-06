/*
* Authors: Corentin GOETGHEBEUR, Chris ARRIDI
* Ce module contient les différentes fonctions nécessaires au menu.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../headers/date.h"
#include "../headers/compte.h"
#include "../headers/files_utils.h"
#include "../headers/entete.h"
#include "../headers/transactions.h"

#include "../headers/menu.h"

//Crée un fichier de compte associé a un numéro aléatoire, met à jour le répertoire de la banque.
int creer_utilisateur(char *nom){

    // Création d'un numéro de compte aléatoire
    int num_compte;
    do{
        printf("creer num aléatoire\n");
        srand(time(0));
        num_compte = rand();
        printf("num aleatoire ok \n");
    } while (check_num_compte(num_compte) == 0);
    printf("num aleatoire ok \n");

    // Création d'un en-tête pour le compte
    printf("creation date\n");
    Date d;
    date(&d);

    printf("creation date ok \n ");

    printf("Création Entete \n");
    Entete entete = {
        .date = d,
        .solde = 0
    };

    printf("Creation fichier\n");

    // Création du fichier correspondant au compte
    creation_fichier(entete, nom);
    printf("Creation fichier ok\n");
}

// Renvoie le numéro de compte associé à ce nom.
int compte_de(char *nom);

// Met à jour le compte associé à ce nom pour la date correspondante.
int mise_a_jour_solde(char* nom, Date d);

// Vire le montant indiqué du compte 1 au compte 2 à la date d.
int virement_de_compte_a_compte(int num_compte1, int num_compte2, Date d, float montant);

// Vire le montant indiqué du compte asssocié au nom 1, vers le compte associé au nom 2 à la date d.
int virement_de_a(char *nom1, char *nom2, Date d, float montant);

// Affiche le relevé du compte associé au nom pour le mois fourni.
int imprimer_releve(char *nom, int mois);

// Vérifie qu'un numéro de compte est disponible
int check_num_compte(int num_compte){
    char filename [35]= "Files/comptes/";
    char str_num_compte[15];
    sprintf(str_num_compte, "%i ", num_compte); // convertir le num de compte en chaine de caractères
    strcat(filename, str_num_compte);   // concaténer le path
    
    if(access(filename, F_OK) == 0){
        // un fichier contenant ce numéro a été trouvé, ce numéro n'est pas disponible
        return 0;
    } else {
        // pas de fichier avec ce num, il est disponible
        return 1;
    }
}