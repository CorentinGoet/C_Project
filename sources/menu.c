/*
* Authors: Corentin GOETGHEBEUR, Chris ARRIDI
* Ce module contient les différentes fonctions nécessaires au menu.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

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
        srand(time(0));
        num_compte = rand();
    } while (check_num_compte(num_compte) == 0);
    
    // Création d'un en-tête pour le compte
    Date d;
    date(&d);

    Entete entete = creation_entete(d, 0);


    // Création du fichier correspondant au compte
    
    char path[50];
    nom_compte(num_compte, path);
    FILE *f = creation_fichier(entete, path);
    fermer(f);

    // ajout au registre de la banque
    ouvrir(&f, "Files/registre");
    fseek(f, 0, SEEK_END);
    fprintf(f, "%i %li %s \n", num_compte, strlen(nom), nom); // Format du registre: num_compte taille_nom nom
    fermer(f);

    return 0;
}

// Renvoie le numéro de compte associé à ce nom.
int compte_de(char *nom){
    FILE *f;
    ouvrir(&f, "Files/registre");
    int taille, num_compte, res;
    char nom_compte[50];
    do{
        res = fscanf(f, "%i %i ", &num_compte, &taille); // Récupérer le num de compte et le nb de char du nom
        fgets(nom_compte, taille + 2, f);   // Récupérer le nom
        nom_compte[taille+0] = '\0'; // On retire le \n et l'espace à la fin du nom
        if (strcmp(nom, nom_compte) == 0){
            return num_compte;
        }
    }while(res > 0);
    return -1; // pas de compte trouvé
}

// Met à jour le compte associé à ce nom pour la date correspondante.
int mise_a_jour_solde(char* nom, Date d){
    //principe:
    // parcourir les transactions du fichier et ajouter au solde les transactions

    // ouvrir le fichier du compte
    int num_compte;
    num_compte = compte_de(nom);
    if (num_compte == -1){
        return 1; // Pas de compte à ce nom
    }

}

// Vire le montant indiqué du compte 1 au compte 2 à la date d.
int virement_de_compte_a_compte(int num_compte1, int num_compte2, Date d, float montant);

// Vire le montant indiqué du compte asssocié au nom 1, vers le compte associé au nom 2 à la date d.
int virement_de_a(char *nom1, char *nom2, Date d, float montant);

// Affiche le relevé du compte associé au nom pour le mois fourni.
int imprimer_releve(char *nom, int mois);

// Vérifie qu'un numéro de compte est disponible
int check_num_compte(int num_compte){
    char filename [35];
    nom_compte(num_compte, filename);
    FILE *file;
    if((file = fopen(filename,"r"))!=NULL)
        {
            
            fclose(file);
            return 0;
        }
    else
        {
            return 1;
        }
    
}

// Donne le path/nom d'un fichier à partir du numéro de compte
int nom_compte(int num_compte, char* nom){
    char filename [35]= "Files/comptes/";
    char str_num_compte[15];
    sprintf(str_num_compte, "%i", num_compte); // convertir le num de compte en chaine de caractères
    strcat(filename, str_num_compte);   // concaténer le path

    strcpy(nom, filename);
    return 0;
}