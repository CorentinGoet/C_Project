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
