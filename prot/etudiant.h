#ifndef ETUDIANT_H
#define ETUDIANT_H

struct jeu;

typedef struct etudiant {
    int pointsDeVie;
    int ligne;
    int position;
    int vitesse;
    int tour;
    Etudiant* next;
    Etudiant* next_line;
    Etudiant* prev_line;
    char type;
} Etudiant;

void addEtudiant(Jeu* jeu, const int _tour, const int _ligne, const char _type);

#endif