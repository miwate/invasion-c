#ifndef JEU_H
#define JEU_H

#include "etudiant.h"
#include "tourelle.h"

typedef struct jeu{
    Tourelle* tourelles;
    Etudiant* etudiants;
    int cagnotte;
    int tour;
    int lastTour;
    int lastLigne;
} Jeu;

void initJeu(Jeu* jeu);
void loadFichier(Jeu* jeu, const char* _niveau);
void previewVagues(Jeu* jeu);

#endif


