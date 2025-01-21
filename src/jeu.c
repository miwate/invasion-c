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

void refreshJeu(Jeu* jeu){

    Etudiant* etu = malloc(sizeof(Etudiant));

    if (etu == NULL) {
        printf("Erreur d'allocation mémoire.\n");
        return;
    }

    etu = jeu->etudiants;

    while(etu != NULL){
        etu->position -= etu->vitesse;
    }

/* UNFINISHED IL FAUT AJOUTER LES TOURELLES D'abord */


}


/* Ajoute une tourelle dans le jeu en fonction de la ligne et de la position */
void addTourelle(Jeu* jeu, const int _ligne, const int _position, const char _type){

    /* Barney pour le nom générique des tourelles en référence à Half life (côté sympa des tourelles)*/
    Tourelle* barney = malloc(sizeof(Tourelle));

    if (barney == NULL){
        printf("Impossible d'ajouter la tourelle %d %c.\n", _ligne, _type);
        return;
    }

    barney->type = _type;
    barney->ligne = _ligne;
    barney->position = _position;

    /* Différents types de tourelles */
    if (_type == 'T'){ // T pour Tourelle
        barney->degats = 2;
        barney->pointsDeVie = 2;
    }
    else if (_type == 'D'){ // B pour Defective
        barney->degats = 1;
        barney->pointsDeVie = 1;
    }
    else {
        barney->degats = 1;
        barney->pointsDeVie =2;     
    }

    barney->next = NULL;


    /* Cas : première tourelle */
    if (jeu->tourelles == NULL){
        jeu->tourelles = barney;
    }

    /* Cas : il y a déjà des tourelles, donc on met la tourelle en fin de liste */
    else {
        Tourelle* precedent = jeu->tourelles;

        while (precedent->next != NULL){
            precedent = precedent->next;
        }
        precedent->next = barney;

    }

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
    etu->position = SPAWN_AREA;
    etu->tour = _tour;
    etu->vitesse = 1;

/* Met à jour les caractéristiques d'un étudiant selon son type */
    if (_type == 'Z') { // Z pour Zombie
        etu->pointsDeVie = 5;
        etu->vitesse = 1;
    }
    else if (_type == 'H'){ // C pour Headcrab
        etu->pointsDeVie = 1;
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
