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

    // récupérer le n° de compte
    int num_compte;
    num_compte = compte_de(nom);
    if (num_compte == -1){
        return 1; // Pas de compte à ce nom
    }

    // récupérer le nom du fichier de compte
    char filename[50];
    if (nom_compte(num_compte, filename) != 0){
        return 1; // Problème dans la fonction nom_compte
    }

    // ouvrir le fichier du compte
    FILE *f_compte;
    ouvrir(&f_compte, filename);

    // lire l'en-tête
    Entete entete;
    if(read_entete(f_compte, &entete) != 0){
        printf("En-tête de compte invalide pour %s, compte n°%i\n", nom, num_compte);
        return 1;
    }

    

    // lecture des transactions tant que la date est supérieure à la date 
    Transaction t;
    int res = read_transaction(f_compte, &t);
    while(res == 0) {
        
        if(date_comp(entete.date, t.date) <= 0){ // si la transition est plus vieille que la dernière màj de l'en-tête, on ne la considère pas
        
            if(strcmp(nom, t.nom) == 0){    // Si le propriétaire du compte est le receveur de la transaction
                entete.solde = entete.solde + t.montant;
            }else{
                entete.solde = entete.solde - t.montant;
            }
        }
        //lecture de la transaction
        res = read_transaction(f_compte, &t);
    }

    // Mettre à jour la date de l'en-tete
    date(&(entete.date));
    printf("date maj entete %i %i %i\n", entete.date.jour, entete.date.mois, entete.date.annee);
    // Ecrire le nouvel en-tête
    fseek(f_compte, 0, 0);
    printf("Nouveau solde : %f\n", entete.solde);
    if(write_entete(f_compte, entete) != 0){
        printf("Ecriture impossible de l'en-tête.\n");
        return 1;
    }

    return 0;
}

// Vire le montant indiqué du compte 1 au compte 2 à la date d.
int virement_de_compte_a_compte(int num_compte1, int num_compte2, Date d, float montant){
    
    char nom_receveur[50], nom_emetteur[50], label_rec[31], label_em[31], filename[30]; // Nom du receveur et de l'emetteur + labels
    Transaction transaction_rec, transaction_em;   // transactions du receveur et de l'emetteur

    // Initialisation des noms et des labels
    num2nom(num_compte2, nom_receveur);
    num2nom(num_compte1, nom_emetteur);

    sprintf(label_em, "virement a %i", num_compte2);
    sprintf(label_rec, "virement de %i", num_compte1);

    // création des transactions
    transaction_em = creation_transaction(d, montant, label_em, nom_receveur);
    transaction_rec = creation_transaction(d, montant, label_rec, nom_receveur);

    // Ecriture dans le compte de l'emetteur
    FILE *f;
    nom_compte(num_compte1, filename); 
    ouvrir(&f, filename);
    ajout_transaction(f, transaction_em);
    fermer(f);

    // Ecriture sur le compte du receveur
    nom_compte(num_compte2, filename);
    ouvrir(&f, filename);
    ajout_transaction(f, transaction_rec);
    fermer(f);

    return 0;
}

// Vire le montant indiqué du compte asssocié au nom 1, vers le compte associé au nom 2 à la date d.
int virement_de_a(char *nom1, char *nom2, Date d, float montant){
    // recup num de comptes de 1 et 2
    // appeler vir compte a compte

    int num_compte1, num_compte2;
    num_compte1 = compte_de(nom1);
    num_compte2 = compte_de(nom2);
    
    return virement_de_compte_a_compte(num_compte1, num_compte2, d, montant);
}

// Affiche le relevé du compte associé au nom pour le mois fourni.
int imprimer_releve(char *nom, int mois){
    // Parcourir les transactions
    // on garde celles du mois donné et l'entete
}

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

// Donne le nom du propriétaire a partir du num de compte (inv. de compte_de)
int num2nom(int num_compte, char nom[50]){
    FILE *f;
    ouvrir(&f, "Files/registre");
    int taille, num, res;
    char nom_compte[50];
    do{
        res = fscanf(f, "%i %i ", &num, &taille); // Récupérer le num de compte et le nb de char du nom
        fgets(nom_compte, taille + 2, f);   // Récupérer le nom
        nom_compte[taille+0] = '\0'; // On retire le \n et l'espace à la fin du nom
        if (num == num_compte){
            strcpy(nom, nom_compte);
            return 0;
        }
    }while(res > 0);
    return -1; // pas de compte trouvé
}