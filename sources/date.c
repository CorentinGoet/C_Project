/*
* Authors: Corentin GOETGHEBEUR, Chris ARRIDI
* Ce module contient la structure de date et 
* les fonctions permettant de la manipuler.
*/

#include <stdlib.h>
#include <time.h>
#include "../headers/date.h"

/* Rappel structure date :
*typedef struct Date{
*    int jour;
*    int mois;
*    int annee;
*};
*/

void date(Date *d){
    time_t t = time(NULL);
    struct tm *tmp_time;
    tmp_time = localtime(&t);
    d->jour = tmp_time->tm_mday;
    d->mois = tmp_time->tm_mon + 1;
    d->annee = tmp_time->tm_year + 1900;
}

int date_comp(Date d1, Date d2){
    
    // convertir en nb de jours
    int nb_jour1, nb_jour2;
    nb_jour1 = 365 * d1.annee +  30*d1.mois + d1.jour;
    nb_jour2 = 365 * d2.annee +  30*d2.mois + d2.jour;

    if(nb_jour1 > nb_jour2) return 1;
    if(nb_jour1 == nb_jour2) return 0;
    if(nb_jour1 < nb_jour2) return -1;

    return -2;
}
