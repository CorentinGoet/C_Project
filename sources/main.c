
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

struct date{
    int jour;
    int mois;
    int annee;
};

void date(struct date *d){
    time_t t = time(NULL);
    struct tm *tmp_time;
    tmp_time = localtime(&t);
    d->jour = tmp_time->tm_mday;
    d->mois = tmp_time->tm_mon + 1;
    d->annee = tmp_time->tm_year + 1900;
}

struct compte{
    char *nom;   // Nom du propriétaire du compte
    int num_compte;  // Numéro du compte
};

void ouvrir(FILE **f, char* nom){
    *f = fopen(nom, "r+");
}

void fermer(FILE *f){
    fclose(f);
}

int main(int argc, char const *argv[])
{
    struct date d;
    date(&d);
    printf("date : %i / %i / %i \n", d.jour, d.mois, d.annee);
    
    struct compte c;
    c.nom = "Corentin";
    c.num_compte = 1;
    printf("Le compte n°%i appartient à %s\n", c.num_compte, c.nom);

    FILE *f;
    ouvrir(&f, "testfile.txt");
    char test[4];
    fgets(test, 4, f);
    printf("%s\n", test);
    fermer(f);
    return 0;
}

