/*
* Authors: Corentin GOETGHEBEUR, Chris ARRIDI
* Ce module contient les différentes fonctions nécessaires au menu.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>

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
    int num_compte, res = 0;
    char filename[50];
    Entete entete;
    Transaction t;
    // trouver le numéro du compte
    num_compte = compte_de(nom);
    // trouver le nom du fichier du compte
    nom_compte(num_compte, filename);
    // Affichage du titre etc...
    printf("====================\n");
    printf("Relevé de compte : \n");
    printf("Nom : %s N° de compte: %i\n", nom, num_compte);


    FILE *f;
    ouvrir(&f, filename);
    read_entete(f, &entete);
    // Affichage de l'en-tête
    printf("En-tête : \n");
    printf("Date: %i / %i / %i Solde : %f \n", entete.date.jour, entete.date.mois, entete.date.annee, entete.solde);
    printf(" - - - - - - - - - - - - - - -\n");
    printf("Transactions: \n");
    while(res == 0){
        res = read_transaction(f, &t);
        if(t.date.mois == mois && res == 0){
            // Affichage de la transaction
            printf("Date : %i / %i / %i, Montant: %f, Bénéficiaire : %s, Label : %s \n",
            t.date.jour, t.date.mois, t.date.annee, t.montant, t.nom, t.label);
        }

    }
    fermer(f);
    printf("====================\n");

    return 0;
}

// Vérifie qu'un numéro de compte est disponible parmi les fichiers de comptes
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

// Vérifie l'existence d'un nom dans le registre (0 si non 1 si oui)
int nom_dans_registre(char *nom){
    FILE *f;
    ouvrir(&f, "Files/registre");
    int taille, num, res;
    char nom_compte[50];

    // On parcourt le registre (similaire à compte_de, nom_compte ...)
    do{
        res = fscanf(f, "%i %i ", &num, &taille); // Récupérer le num de compte et le nb de char du nom
        fgets(nom_compte, taille + 2, f);   // Récupérer le nom
        nom_compte[taille+0] = '\0'; // On retire le \n et l'espace à la fin du nom
        if (strcmp(nom, nom_compte) == 0){
            return 1; // compte trouvé
        }
    }while(res > 0);

    return 0; // pas de compte trouvé à ce nom
}

// Vérifie l'existence d'un numéro de compte dans le registre
int numero_dans_registre(int num_compte){
    FILE *f;
    ouvrir(&f, "Files/registre");
    int taille, num, res;
    char nom_compte[50];
    do{
        res = fscanf(f, "%i %i ", &num, &taille); // Récupérer le num de compte et le nb de char du nom
        fgets(nom_compte, taille + 2, f);   // Récupérer le nom
        nom_compte[taille+0] = '\0'; // On retire le \n et l'espace à la fin du nom
        if (num == num_compte){
            return 1; // compte trouvé a ce num
        }
    }while(res > 0);
    return 0; // pas de compte trouvé pour ce num
}

// Interface de menu de la banque
void menu(){

    char choix;

    

    do{
        printf("============================================\n");
        printf("Bienvenue dans la banque !\n");
        printf("------------------------\n");
        printf("Que voulez-vous faire ?\n");
        printf("\tAjouter un nouveau client........................A\n");
        printf("\tLister les comptes de tous les clients...........L\n");
        printf("\tRelevé d'un compte client........................R\n");
        printf("\tVirement depuis un compte client.................V\n");
        printf("\tMise à jour du solde d'un client.................M\n");
        printf("\tQuitter..........................................Q\n");

        printf("Votre choix : \n");
        rewind(stdin);
        scanf("%s", &choix);

        switch(choix){
            case 'A':
            case 'a':
                menu_ajout_client();
                break;
            
            case 'L':
            case 'l':
                menu_liste_clients();
                break;
            
            case 'R':
            case 'r':
                menu_releve_client();
                break;

            case 'V':
            case 'v':
                menu_virements();
                break;
            
            case 'M':
            case 'm':
                menu_maj_solde();
                break;
            
            case 'Q':
            case 'q':
                printf("Au revoir.\n");
                break;

            default:
                printf("Veuillez choisir parmi les options proposées.\n");
        }

    }while(choix != 'q' && choix != 'Q');
}

int menu_ajout_client(){

    int ok = 1;
    int num_compte;
    char nom[50];

    printf("============================================\n");
    printf("Ajout d'un nouveau client :\n");
    
    while(ok){
        char choix;
        printf("Entrez le nom du client:\n");
        rewind(stdin);
        scanf("%s", nom);
        printf("Le nom sélectionné est : %s.\n", nom);

        // Vérifier que le nom est disponible
        if(nom_dans_registre(nom)){
            printf("Ce nom existe déjà dans le registre.\n");
            continue; // on repasse dans la boucle
        }
        
        printf("Que voulez-vous faire ?\n");
        printf("\tModifier le nom .... M\n");
        printf("\tCréer le compte .... C\n");
        printf("votre choix :\n");
        
        rewind(stdin);
        scanf("%s", &choix);

        switch(choix){
            case 'm':
            case 'M':
                ok = 1;
                break;
            
            case 'c':
            case 'C':
                ok = 0;
                break;

            default :
                printf("Veuillez choisir une option parmi celles proposées.\n");
                ok = 1;  
        }
    }
    
    creer_utilisateur(nom); // création du compte
    num_compte = compte_de(nom);    // récupération du num de compte
    printf("Un compte a bien été créé pour %s avec le n° de compte %i.\n", nom, num_compte);

    return 0;
}

int menu_liste_clients(){
    int num, taille, res;
    char nom[50];
    FILE *f;

    printf("============================================\n");
    printf("Liste des clients :\n");

    ouvrir(&f, "Files/registre");

    do{
        res = fscanf(f, "%i %i ", &num, &taille); // Récupérer le num de compte et le nb de char du nom
        fgets(nom, taille + 2, f);   // Récupérer le nom
        nom[taille+0] = '\0'; // On retire le \n et l'espace à la fin du nom
        
        if(res > 0) printf("Compte n°%i, nom: %s\n", num, nom); // on évite d'imprimer en double la derniere ligne
    }while(res > 0);
    fermer(f);
    return 0;
}

int menu_releve_client(){
    
    char nom[50], choix;
    int res = 0, mois = 0;


    printf("============================================\n");
    printf("Vous souhaitez accéder au relevé de compte d'un client.\n");

    while(res == 0){ // Tant que le nom entré ne correspond pas à un client
        printf("Entrez le nom du client :\n");
        scanf("%s", nom);
        printf("Nom sélectionné : %s.\n", nom);

        // Recherche du nom dans le registre
        if((res = nom_dans_registre(nom))){
            printf("Compte trouvé pour %s.\n", nom);
        }else{
            printf("Pas de compte pour le nom: %s.\n", nom);
        }
    }

    printf("Que voulez-faire ?\n");
    printf("Afficher le relevé de compte .........A\n");
    printf("Retourner au menu principal ..........M\n");

    scanf("%s", &choix);
    switch(choix){
        case 'a':
        case 'A':
            while(mois < 1 || mois > 12){
                printf("Quel mois souhaitez-vous afficher ? (janvier - 1 => décembre - 12)\n");
                rewind(stdin);
                scanf("%i", &mois);
                if (mois < 1 || mois > 12){
                    printf("%i n'est pas un numéro de mois valide.\n", mois);
                }else{
                    printf("Vous avez sélectionné le mois %i.\n", mois);
                }
            }

            imprimer_releve(nom, mois);
            break;
        
        case 'm':
        case 'M':
            return 0;
            break;
        
        default:
            printf("Choisissez une option parmi celles proposées.\n");
    }

    return 0;
}

int menu_virements(){
    Date d;
    char choix, nom1[50], nom2[50];
    int res = 0, montant;

    printf("============================================\n");
    printf("Vous avez choisi de faire un virement.\n");

    // Entrer le nom du compte à débiter
    while(res == 0){ // Tant que le nom entré ne correspond pas à un client
        printf("Entrez le nom du client à débiter:\n");
        scanf("%s", nom1);
        printf("Nom sélectionné : %s.\n", nom1);

        // Recherche du nom dans le registre
        if((res = nom_dans_registre(nom1))){
            printf("Compte trouvé pour %s.\n", nom1);
        }else{
            printf("Pas de compte pour le nom: %s.\n", nom1);
        }
    }
    res = 0;
    // Entrer le nom du compte à créditer
    while(res == 0){ // Tant que le nom entré ne correspond pas à un client
        printf("Vous avez choisi de faire un virement depuis le compte de %s.\n", nom1);
        printf("Entrez le nom du client à créditer:\n");
        scanf("%s", nom2);
        printf("Nom sélectionné : %s.\n", nom2);

        // Recherche du nom dans le registre
        if((res = nom_dans_registre(nom2))){
            printf("Compte trouvé pour %s.\n", nom2);
        }else{
            printf("Pas de compte pour le nom: %s.\n", nom2);
        }
    }
    res = 0;

    // Entrer le montant
    while(res == 0){
        printf("Vous avez choisi de faire un virement depuis %s vers %s.\n", nom1, nom2);
        printf("Entrez un montant:\n");
        rewind(stdin);
        res = scanf("%i", &montant);
        if(res != 1){
            printf("Entrez un montant valide.\n");
            res = 0;
            continue;
        }
        

    }
    res = 0;

    // Validation
    while(res == 0){
        int montant_transaction = montant; // Pour une raison inconnue, la variable montant change de valeur après le scanf qui ne la concerne pas
        printf("Vous avez choisi de faire un virement de %i € depuis le compte de %s vers le compte de %s.\n", montant, nom1, nom2);
        printf("\tValider ..... V\n");
        printf("\tAnnuler ..... A\n");
        scanf("%s", &choix);
        switch(choix){
            case 'v':
            case 'V':
                // Récupération de la date
                date(&d);
                // Virement
                virement_de_a(nom1, nom2, d, montant_transaction);
                printf("Le virement de %i € depuis le compte de %s vers le compte de %s a bien été effectué.\n", montant_transaction, nom1, nom2);
                res = 1;
                break;

            case 'a':
            case 'A':
                return 0;

            default:
                printf("Veuillez choisir parmi les options proposées.\n");
        }
    }

    return 0;
}

int menu_maj_solde(){

    int res = 0;
    char nom[50], choix;
    Date d;

    printf("============================================\n");
    printf("Vous avez choisi de mettre à jour un solde.\n");
    while(res == 0){ // Tant que le nom entré ne correspond pas à un client
        printf("Entrez le nom du client:\n");
        scanf("%s", nom);
        printf("Nom sélectionné : %s.\n", nom);

        // Recherche du nom dans le registre
        if((res = nom_dans_registre(nom))){
            printf("Compte trouvé pour %s.\n", nom);
        }else{
            printf("Pas de compte pour le nom: %s.\n", nom);
        }
    }
    res = 0;
    // Validation
    while(res == 0){
        printf("Vous avez choisi de mettre à jour le solde de %s.\n", nom);
        printf("\tValider ..... V\n");
        printf("\tAnnuler ..... A\n");
        scanf("%s", &choix);
        switch(choix){
            case 'v':
            case 'V':
                // Récupération de la date
                date(&d);
                // mise a jour
                mise_a_jour_solde(nom, d);
                printf("La mise à jour du solde de %s a bien été effectuée.\n", nom);
                res = 1;
                break;

            case 'a':
            case 'A':
                return 0;

            default:
                printf("Veuillez choisir parmi les options proposées.\n");
        }
    }

    return 0;
}