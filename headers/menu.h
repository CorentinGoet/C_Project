/*
* Authors: Corentin GOETGHEBEUR, Chris ARRIDI
* Ce module contient les différentes fonctions nécessaires au menu.
*/

#ifndef MENU_H
#define MENU_H

#include "date.h"

//Crée un fichier de compte associé a un numéro aléatoire, met à jour le répertoire de la banque.
int creer_utilisateur(char *nom);

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
int check_num_compte(int num_compte);

// Donne le nom d'un fichier de compte à partir du n° de compte
int nom_compte(int num_compte, char *nom);

#endif