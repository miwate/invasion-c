#include "../prot/jeu.h"

/* -- Ce fichier contient les fonctions nécessaires pour le bon déroulement du jeu -- */
/* S'il y a des règles à changer, c'est ici */

/* Initialise un jeu vide*/
void initJeu(Jeu* jeu){
    jeu->tourelles = NULL;
    jeu->etudiants = NULL;
    jeu->cagnotte = 0;
    jeu->tour = 1;
    jeu->lastTour = 0;
    jeu->lastLigne = 0;
    jeu->score = 0;
    jeu->combo = 0;
    jeu->multiplicateurScore = 1.0;
}


/* Rafraichit l'état du jeu (faire tirer les tourelles, faire avancer les ennemis)*/
void rafraichirJeu(Jeu* jeu){

    /* Vague vide */
    if (jeu->etudiants == NULL){
        printf("Pas de vague.\n");
        return;
    }

    Etudiant* etu = jeu->etudiants;
    int finTour = 1;

    /* Les étudiants sont-ils tous éliminés ? */
    while (etu != NULL){
        if (etu->tour == jeu->tour && etu->pointsDeVie > 0){
            finTour = 0;
        }
        etu = etu->next;
    }

    /* Incrémentation du tour si les ennemis du tour sont morts */
    if (finTour != 0){
        jeu->tour += 1;
    }

    /* Faire avancer les étudiants */
    etu = jeu->etudiants;
    while (etu != NULL){
        //printf("d");
        etu->position -= etu->vitesse;
        etu = etu->next;
    }

    /* Faire tirer les Barney (tourelles pour rappel)*/
    Tourelle* barney = jeu->tourelles;

    while (barney != NULL){
        
        etu = jeu->etudiants;
        Etudiant* etu_prec = NULL;

        while (etu != NULL){
            if (etu->ligne == barney->ligne){

                /* Ajoute le score dégâts infligés */
                jeu->score += jeu->multiplicateurScore * abs(etu->pointsDeVie - barney->degats);

                /* INflige les dégâts */
                etu->pointsDeVie -= barney->degats;


                /* Cas : zombie mort (parce que ça me met mal à l'aise de dire étudiant mort) */
                if (etu->pointsDeVie <= 0){

                    /* ENleve l'étudiant de la liste */
                    if (etu_prec == NULL) {
                        jeu->etudiants = etu->next;
                    }
                    
                    else {
                        etu_prec->next = etu->next;
                    }

                    /* Libération de mémoire */
                    Etudiant* q = etu;
                    etu = etu->next;
                    free(q);

                    continue; 
                }

            }
            etu_prec = etu;

            etu = etu->next;
        }
        barney = barney->next;
    }


}


/* Ajoute une tourelle dans le jeu en fonction de la ligne et de la position */
void ajoutTourelle(Jeu* jeu, const int _ligne, const int _position, const char _type){

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
        barney->pointsDeVie = 2;     
    }

    barney->next = NULL;

    
    /* Mise à jour du multiplicateur de score en fonction des Barney ajoutés (aussi en fonction des étudiants) */
    jeu->multiplicateurScore /= (1 + 0.05 * barney->degats + 0.025 * barney->pointsDeVie);
    
    /* Multiplicateur min */
    if (jeu->multiplicateurScore < 0.1) jeu->multiplicateurScore = 0.1;


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
void ajoutEtudiant(Jeu* jeu, const int _tour, const int _ligne, const char _type){


    /* Initialisation de l'étudiant */
    Etudiant* etu = malloc(sizeof(Etudiant));

    if (etu == NULL) {
        printf("Impossible d'ajouter l'étudiant %d %d %c.\n", _tour, _ligne, _type);
        return;
    }

    etu->type = _type;
    etu->ligne = _ligne;
    etu->position = SPAWN_DISTANCE;
    etu->tour = _tour;
    etu->vitesse = 1;

    /* Met à jour les caractéristiques d'un étudiant selon son type */
    if (_type == 'Z') { // Z pour Zombie
        etu->pointsDeVie = 5;
        etu->vitesse = 1;
        etu->degats = 1;
    }
    else if (_type == 'A'){ // A pour alien
        etu->pointsDeVie = 1;
        etu->vitesse = 1;
        etu->degats = 2;
    }
    else if (_type == 'C'){ // C pour colosse
        etu->pointsDeVie = 8;
        etu->vitesse = 1;
        etu->degats = 3;
    }
    else if (_type == 'P'){ // P pour pacman
        etu->pointsDeVie = 1;
        etu->vitesse = 3;
        etu->degats = 1;
    }
    else if (_type == 'R'){ // R pour Roi
        etu->pointsDeVie = 3;
        etu->vitesse = 2;
        etu->degats = 4;
    }

    /* Valeurs par défaut*/
    else{
        etu->pointsDeVie = 1;
        etu->vitesse = 1;
        etu->degats = 1;
    }

    etu->next = NULL;
    etu->next_line = NULL;
    etu->prev_line = NULL;

    /* Mise à jour du multiplicateur de score en fonction des Barney ajoutés (aussi en fonction des étudiants) */
    jeu->multiplicateurScore *= (1 + 0.05 * etu->degats + 0.025 * etu->pointsDeVie + 0.025 * etu->vitesse);


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
