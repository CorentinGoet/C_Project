/*
* Authors: Corentin GOETGHEBEUR, Chris ARRIDI
* Ce module permet d'accéder aux dates.
*/

#ifndef DATE_H
#define DATE_H

typedef struct Date{
    int jour;
    int mois;
    int annee;
} Date;

void date(Date *d);

int date_comp(Date d1, Date d2);

#endif