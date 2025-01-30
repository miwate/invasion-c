#include "../prot/jeu.h"

/* -- Ce fichier contient les fonctions nécessaires pour le bon déroulement du jeu -- */
/* S'il y a des règles à changer, c'est ici */

/* Initialise un jeu vide*/
void initJeu(Jeu* jeu){
    jeu->fin = 1; // = 1 si le jeu n'est pas fini
    jeu->tourelles = NULL;
    jeu->etudiants = NULL;
    jeu->cagnotte = 0;
    jeu->tour = 1;
    jeu->dernierTour = 0;
    jeu->derniereLigne = 0;
    jeu->score = 0;
    jeu->combo = 0;
    jeu->multiplicateurScore = 1.0;
}


/* Rafraichit l'état du jeu (faire tirer les tourelles, faire avancer les ennemis)*/
void rafraichirJeu(Jeu* jeu){

    /* Vague vide */
    if (jeu->etudiants == NULL){
        jeu->fin = 0;
        printf("Pas de vague.\n");
        return;
    }

    Etudiant* etu = jeu->etudiants;
    int finTour = 1;
    int nbEtudiantsTour = 0;

    /* Les étudiants sont-ils tous éliminés ? */
    while (etu != NULL){
        // printf("%d - %d\n", etu->tour, jeu->tour);

        if (etu->tour == jeu->tour){
            nbEtudiantsTour += 1;
            if (etu->pointsDeVie > 0){
                finTour = 0;
            }
        }
        etu = etu->next;
    }

    /* Incrémentation du tour si les ennemis du tour sont morts */
    if (nbEtudiantsTour == 0 && finTour){
        printf("Incrémentation du tour\n");
        jeu->tour += 1;
    }

    /* Faire avancer les étudiants */
    etu = jeu->etudiants;
    while (etu != NULL){
        //printf("etu->tour %d\n", etu->tour);
        if (etu->tour <= jeu->tour) {
            etu->position -= etu->vitesse;
        }
        /* FIN de la partie ? */
        if (etu->position < 0){
            jeu->fin = -1;
        }
        etu = etu->next;
    }

    /* Faire tirer les Barney (tourelles pour rappel)*/
    Tourelle* barney = jeu->tourelles;

    while (barney != NULL){
        
        etu = jeu->etudiants;
        Etudiant* etu_prec = NULL;

        while (etu != NULL){
            if (etu->ligne == barney->ligne && jeu->tour == etu->tour){

                /* Ajoute le score dégâts infligés */
                jeu->score += jeu->multiplicateurScore * abs(etu->pointsDeVie - barney->degats);

                /* INflige les dégâts */
                etu->pointsDeVie -= barney->degats;


                /* Cas : zombie mort (parce que ça me met mal à l'aise de dire étudiant mort) */
                if (etu->pointsDeVie <= 0){

                    /* Ajoute un peu d'argent dans la cagnotte */
                    jeu->cagnotte += 10 * jeu->combo;

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
                    if (etu != NULL){
                        free(q);
                    }
                    continue; 
                }

            }
            etu_prec = etu;

            etu = etu->next;
        }
        barney = barney->next;
    }
    jeu->combo ++;


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
    barney->next = NULL;

    // Gros changements apportés ici pour simplifier les cas
    /* Affectation des stats */
    switch (_type) {
        case 't': // Tourelle classique
            barney->pointsDeVie = 3;
            barney->degats = 1;
            barney->prix = 150;
            break;
        case 'b': // Bouclier
            barney->pointsDeVie = 6;
            barney->degats = 0;
            barney->prix = 200;
            break;
        case 'm': // Mine Explosive
            barney->pointsDeVie = 1;
            barney->degats = 5;
            barney->prix = 100;
            break;
        case 's': // Slow (Ralentissement)
            barney->pointsDeVie = 2;
            barney->degats = 1;
            barney->prix = 120;
            break;
        case 'x': // Explosive adjacente
            barney->pointsDeVie = 2;
            barney->degats = 4;
            barney->prix = 800;
            break;
        default:
            printf("Type de tourelle inconnu : %c\n", _type);
            free(barney);
            return;
    }

    if (barney->prix > jeu->cagnotte){
        printf("Cagnotte insuffisante !\n");
        free(barney);
        return;
    }
    jeu->cagnotte -= barney->prix;
    barney->next = NULL;

    
    /* Mise à jour du multiplicateur de score en fonction des Barney ajoutés (aussi en fonction des étudiants) */
    jeu->multiplicateurScore /= (1 + 0.5 * barney->degats + 0.25 * barney->pointsDeVie);
    
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
        etu->pointsDeVie = 7;
        etu->vitesse = 1;
        etu->degats = 2;
    }
    else if (_type == 'C'){ // C pour colosse
        etu->pointsDeVie = 9;
        etu->vitesse = 1;
        etu->degats = 3;
    }
    else if (_type == 'M'){ // M pour Medic
        etu->pointsDeVie = 8;
        etu->vitesse = 3;
        etu->degats = 1;
    }
    else if (_type == 'R'){ // R pour Roi
        etu->pointsDeVie = 9;
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
    jeu->multiplicateurScore *= (1 + 0.025 * etu->degats + 0.025 * etu->pointsDeVie + 0.025 * etu->vitesse);

    
    /* Cas : premier étudiant */
    if (jeu->etudiants == NULL){
        jeu->etudiants = etu;
    }

    /* Cas : il y a déjà des étudiants, donc on met l'étudiant en fin de liste */
    else {
        Etudiant* precedent = jeu->etudiants;

        while (precedent->next != NULL){
            if (precedent->ligne == etu->ligne - 1){
                precedent->next_line = etu;
                etu->prev_line = precedent;
            }
            precedent = precedent->next;
        }
        precedent->next = etu;

    }
}
