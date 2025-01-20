#include "../prot/jeu.h"

/* -- Ce fichier contient les fonctions nécessaires pour le bon déroulement du jeu -- */
/* S'il y a des règles à changer, c'est ici */


/* Initialise un jeu vide*/
void initJeu(Jeu* jeu){
    jeu->tourelles = NULL;
    jeu->etudiants = NULL;
    jeu->cagnotte = 0;
    jeu->tour = 0;
    jeu->lastTour = 0;
    jeu->lastLigne = 0;
}


/* Fonction prenant en paramètres le jeu, le tour, la ligne et le type d'un étudiant */
/* Ajoute un étudiant dans la file */
void addEtudiant(Jeu* jeu, const int _tour, const int _ligne, const char _type){

    Etudiant* etu = malloc(sizeof(Etudiant));

    if (etu == NULL) {
        printf("Impossible d'ajouter l'étudiant %d %d %c.\n", _tour, _ligne, _type);
        return;
    }

    etu->type = _type;
    etu->ligne = _ligne;
    etu->position = 0;
    etu->tour = _tour;
    etu->vitesse = 1;

/* Met à jour les caractéristiques d'un étudiant selon son type */
    if (_type == 'Z') {
        etu->pointsDeVie = 5;
        etu->vitesse = 1;
    }

    /* Valeurs par défaut*/
    else{
        etu->pointsDeVie = 1;
        etu->vitesse = 1;
    }

    etu->next = NULL;
    etu->next_line = NULL;
    etu->prev_line = NULL;

    /* Cas : premier étudiant */
    if (jeu->etudiants == NULL){
        jeu->etudiants = etu;
    }

    /* Cas : il y a déjà des étudiants, donc on met l'étudiant en fin de liste */
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
