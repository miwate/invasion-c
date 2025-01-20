#include "../prot/etudiant.h"
#include "../prot/jeu.h"

#include <stdio.h>
#include <stdlib.h>

/* Ajoute un étudiant dans la file*/
void addEtudiant(Jeu* jeu, const int _tour, const int _ligne, const char _type){

    Etudiant* etu = malloc(sizeof(Etudiant));

    if (etu == NULL){
        printf("Impossible d'ajouter l'étudiant %d %d %c.\n", _tour, _ligne, _type);
        return;
    }

    etu->type = _type;
    etu->ligne = _ligne;
    etu->position = 0;
    etu->tour = _tour;

    /* Si l'ennemi est de type Z*/
    if (_type == 'Z'){
        etu->pointsDeVie = 5;
        etu->vitesse = 1;
    }
    else {
        etu->pointsDeVie = 1;
        etu->vitesse = 1;
    }

    etu->next = NULL;
    etu->next_line = NULL;
    etu->prev_line = NULL;

    if (jeu->etudiants == NULL){
        jeu->etudiants = etu;
    }
    else {
        Etudiant* precedent = jeu->etudiants;
        while (precedent->next != NULL){
            precedent = precedent->next;
        }
        precedent->next = etu;

        if (precedent->ligne < etu->ligne){
            precedent->next_line = etu;
        }
    }
}


