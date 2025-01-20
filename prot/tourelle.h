#ifndef TOURELLE_H
#define TOURELLE_H

#include "jeu.h"
#include "etudiant.h"

typedef struct tourelle {
    int pointsDeVie;
    int ligne;
    int position;
    int prix;
    Tourelle* next;
    char type;
} Tourelle;


Tourelle* creerTourelle(int ligne, int position, char type, int prix);
void ajouterTourelle(Tourelle** listeTourelles, Tourelle* nouvelleTourelle);
void afficherTourelles(Tourelle* listeTourelles);
void detruireTourelles(Tourelle** listeTourelles);

#endif