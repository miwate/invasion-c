#include "../prot/jeu.h"
#include "../prot/video.h"

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
    jeu->premierSang = 'n';
}

/* Rafraichit l'état du jeu (faire tirer les tourelles, faire avancer les ennemis)*/
void rafraichirJeu(Jeu* jeu){

    /* Vague vide */
    if (jeu->etudiants == NULL){
        jeu->fin = 0;
        printf("Pas de vague.\n");
        return;
    }
    /* Tour suivant */
    jeu->tour += 1;


    /* Faire avancer les étudiants */
    Etudiant* etu = jeu->etudiants;

    Tourelle* barney = jeu->tourelles;
    Tourelle* barney_avant = NULL;

    while (etu != NULL){

        /* Ça sert pour afficher le sang s'il est touché */
        etu->touche = 'n';

        if (etu->tour <= jeu->tour) {
            etu->position -= etu->vitesse;
            barney = jeu->tourelles;

            /* Actions selon l'étu */
            switch (etu->type){

                /* MEDIC */
                case 'M':
                    /* Heal le suivant de 1pv et lui-même mais les max pv est 9 */
                    if (etu->pointsDeVie <= 7 && etu->pointsDeVie >= 1) {

                        if (etu->next && etu->next->pointsDeVie > 2 && etu->next->pointsDeVie < 9) etu->next->pointsDeVie += 1;
                        etu->pointsDeVie += 1;
                        etu->type = 'N';
                    }

                /* ALIEN */
                case 'A':
                    /* Il devient invisible sauf ses mains et ses antennes quand PV <= 3*/
                    if (etu->pointsDeVie <= 6) etu->type = 'H';

                /* Colosse */
                case 'C':
                    /* Si on casse son mur adoré, il s'énerve beaucoup !! */
                    if (etu->pointsDeVie <= 5){
                        etu->type = 'D';
                        etu->degats = 3;
                        etu->vitesse = 2;
                    }
                
                /* Roi */
                case 'R':
                    /*LE roi est presque mort vive le nouveau presque roi */
                    if (etu->pointsDeVie <= 3) {
                        etu->type = 'O';
                        etu->degats = 0;
                        etu->vitesse = 0;
                        etu->pointsDeVie = 5;
                    }
            }

            /* Barney notre héros national bloque les étudiants */
            while (barney != NULL){

                /* Même ligne */
                if (barney->ligne == etu->ligne){

                    if (etu->position <= barney->position){

                        etu->position  = barney->position;

                        /* Eviter overlap des étu ou dépassement */
                        Etudiant* etuTemp = jeu->etudiants;
                        while (etuTemp != NULL){
                            if (etuTemp!=etu && etuTemp->ligne==etu->ligne && etu->tour>etuTemp->tour) {
                                if (etuTemp->position >= etu->position) {
                                    etu->position = etuTemp->position+1;
                                }
                            }
                            etuTemp = etuTemp->next;
                        }

                        barney->pointsDeVie = barney->pointsDeVie - etu->degats;

                        /* MIne? La mine crée un mini incendie qui fait un peu mal */
                        if (barney->type == 'u'){
                            etu->pointsDeVie -= 5;
                            barney->type = 'k';
                            barney->pointsDeVie = 1;
                            barney->degats = 1;
                            /* Premier sang donc dialogue */
                            if (jeu->premierSang == 'n') jeu->premierSang = 'o';
                            break;
                        }

                        /* Explosion ?*/
                        else if (barney->type == 'k'){
                            barney->type = 'l';
                            barney->pointsDeVie = 2;
                            barney->degats = 0;
                        }

                        if (barney->pointsDeVie <= 0){

                            /* On casse le combo */
                            if (barney->type == 't' || barney->type == 's' || barney->type == 'x' ||barney->type == 'b'){
                                jeu->combo = 0;
                            } 

                            /* SUpprimer barney*/
                            if (barney_avant == NULL){
                                jeu->tourelles = barney->next;
                            }
                            else {
                                barney_avant->next = barney->next;
                            }

                            if (barney) free(barney);
                            /* Premier sang donc dialogue */
                            if (jeu->premierSang == 'n') jeu->premierSang = 'o';
                            
                            break;
                        }
                    }
                }
                barney_avant = barney;
                barney = barney->next;
            }

            /* Eviter overlap des étu ou dépassement */
            Etudiant* etuTemp = jeu->etudiants;
            while (etuTemp != NULL){
                if (etuTemp!=etu && etuTemp->ligne==etu->ligne && etu->tour>etuTemp->tour) {
                    if (etuTemp->position >= etu->position) {
                        etu->position = etuTemp->position+1;
                    }
                }
                etuTemp = etuTemp->next;
            }
            
            
            
        }
        /* FIN de la partie ? */
        if (etu->position <= 0){
            //printf("FIn\n");
            jeu->fin = -1;
            return;
        }
        etu = etu->next;
    }

    /* Faire tirer les Barney (tourelles pour rappel)*/
    barney = jeu->tourelles;

    while (barney != NULL && jeu->fin == 1){
        
        etu = jeu->etudiants;
        Etudiant* etu_prec = NULL;

        // Cas pour chacune des barney (tourelles)
        switch (barney->type){

            // Mine
            case 'u': {
                char explosion='n';
                // Etudiant sur la mine (même ligne et position)
                while (etu != NULL){
                    if (etu->ligne == barney->ligne && etu->position <= barney->position && etu->tour <= jeu->tour){
                        // Explosion 
                        jeu->score += jeu->multiplicateurScore * abs(etu->pointsDeVie - barney->degats);
                        // Dégâts transférés à l'étudiant
                        
                        printf("BOOM !!\n");
                        etu->pointsDeVie -= 5;
                        etu->touche = 'o';
                        

                        if (etu->pointsDeVie <= 0){
                            // Gain d'argent 
                            jeu->cagnotte += 10 * jeu->combo;
                            // Suppression de l'étudiant
                            if (etu_prec == NULL) {
                                jeu->etudiants = etu->next;
                            }
                            else {
                                etu_prec->next = etu->next;
                            }
                            // Mort de l'ennemi et libération de mémoire
                            Etudiant* ennemi_mort = etu;
                            etu = etu->next;
                            if (ennemi_mort) free(ennemi_mort);
                            continue;
                        }
                        else {
                            etu_prec->next = etu->next;
                            etu = etu->next;
                        } 
                        // La mine a explosé donc :
                        explosion='o';
                    }
                if (explosion =='o'){
                    barney->pointsDeVie = 0;

                    /*Libération de mémoire */
                    Tourelle* mineExplosee = barney;
                    barney = barney->next;
                    if (mineExplosee) free(mineExplosee);
                    /* Premier sang donc dialogue */
                    if (jeu->premierSang == 'n') jeu->premierSang = 'o';
                    
                    continue;
                }
                break;
            }
                
            case 's': { // Tourelle de ralentissement adjacentes transperçantes
                char frappeTourelles = 'n';
                while (etu != NULL && frappeTourelles == 'n'){
                    if (etu->ligne == barney->ligne && etu->position <= SPAWN_DISTANCE && etu->position >= 0 && etu->tour <= jeu->tour){
                        // Baisse de vitesse + dégâts infligés + score augmenté
                        etu->vitesse = 1;
                        etu->pointsDeVie -= barney->degats;

                        etu->touche = 'o';
                        jeu->score += jeu->multiplicateurScore * abs(etu->degats);

                        // La tourelle a frappé
                        frappeTourelles = 'o';

                        // Ennemi mort
                        if (etu->pointsDeVie <= 0){
                            // Gain d'argent 
                            jeu->cagnotte += 10 * jeu->combo;
                            // Suppression de l'étudiant
                            if (etu_prec == NULL) {
                                jeu->etudiants = etu->next;
                            } 
                            else {
                                etu_prec->next = etu->next;
                            }
                            // Libération de mémoire
                            Etudiant* ennemi_mort = etu;
                            etu = etu->next;
                            free(ennemi_mort);
                            continue;
                        } else {
                            etu_prec = etu;
                            etu = etu->next;
                        }
                        break;
                    } 
                    else {
                        etu_prec = etu;
                        etu = etu->next;
                    }
                }
            }
                
            case 'x': // Explosive de zone
                    /* Affecte les étudiants sur la même ligne et les lignes adjacentes */
                    for (int i = -1; i <= 1; i++) {
                        int ligneCible = barney->ligne + i;
                        if (ligneCible < 1 || ligneCible > jeu->derniereLigne) {
                            continue; // Ligne en dehors des limites
                        }
    
                        etu = jeu->etudiants;
                        etu_prec = NULL;
    
                        while (etu != NULL){
                            if (etu->ligne == ligneCible && (abs(etu->position - barney->position) <= 5) && (etu->position >= 0) && etu->tour <= jeu->tour){
                                /* Dégâts + Augmentation du score*/
                                etu->pointsDeVie -= barney->degats;
                                etu->touche = 'o';

                                jeu->score += jeu->multiplicateurScore * barney->degats;
    
                                /* Ennemi mort */
                                if (etu->pointsDeVie <= 0){
                                    /* Ajouter de l'argent à la cagnotte */
                                    jeu->cagnotte += 10 * jeu->combo;
    
                                    /* Retirer l'étudiant de la liste */
                                    if (etu_prec == NULL){
                                        jeu->etudiants = etu->next;
                                    } else {
                                        etu_prec->next = etu->next;
                                    }
    
                                    /* Libérer mémoire */
                                    Etudiant* etu_cancel = etu;
                                    etu = etu->next;
                                    free(etu_cancel);
                                    continue;
                                }
                            }
                            etu_prec = etu;
                            etu = etu->next;
                        }
                    }
                    break;
                }
    
            default : // Tourelles à comportement "classiques" dont B (Bouclier mais ses dégâts sont nuls)
                char frappeTourelled = 'n';
                while (etu != NULL && frappeTourelled == 'n'){
                    
                    if (etu->ligne == barney->ligne && etu->position <= SPAWN_DISTANCE && etu->position >= 0 && etu->tour <= jeu->tour){
                        
                        /* Ajoute le score dégâts infligés */
                        jeu->score += jeu->multiplicateurScore * abs(etu->pointsDeVie - barney->degats);
                        /* INflige les dégâts */
                        etu->pointsDeVie -= barney->degats;
                        etu->touche = 'o';

                        frappeTourelled = 'o';
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
                            Etudiant* etu_mort = etu;
                            etu = etu->next;
                            free(etu_mort);
                            continue;
                        } 
                        else {
                            etu_prec = etu;
                            etu = etu->next;
                        }
                    } 
                    else {
                        etu_prec = etu;
                        etu = etu->next;
                    }
                }
                break;
            }
        jeu->combo++;
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
    barney->next = NULL;

    /* Affectation des stats */
    switch (_type) {
        case 't': // Tourelle classique
            barney->pointsDeVie = 3;
            barney->degats = 1;
            barney->prix = 150;
            break;
        case 'b': // Bouclier
            barney->pointsDeVie = 8;
            barney->degats = 0;
            barney->prix = 200;
            break;
        case 'u': // Mine Explosive
            barney->pointsDeVie = 1;
            barney->degats = 0;
            barney->prix = 100;
            break;
        case 's': // Slow (Ralentissement) adjacente transperçante
            barney->pointsDeVie = 2;
            barney->degats = 1;
            barney->prix = 400;
            break;
        case 'x': // Explosive adjacente
            barney->pointsDeVie = 2;
            barney->degats = 4;
            barney->prix = 800;
            break;
        default:
            printf("Type de tourelle inconnu : %c\n", _type);
            if (barney) free(barney);
            return;
    }

    if (barney->prix > jeu->cagnotte){
        printf("Cagnotte insuffisante !\n");
        if (barney) free(barney);
        return;
    }
    jeu->cagnotte -= barney->prix;
    printf("Construction réussie !\nCagnotte restante : %d\n", jeu->cagnotte);
    barney->next = NULL;

    
    /* Mise à jour du multiplicateur de score en fonction des Barney ajoutés (aussi en fonction des étudiants) */
    jeu->multiplicateurScore /= (1 + 0.5 * barney->degats + 0.25 * barney->pointsDeVie);
    
    /* Multiplicateur min */
    if (jeu->multiplicateurScore < 1) jeu->multiplicateurScore = 1;


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

/* Ajoute une tourelle dans le jeu sans cagnotte, force pour la démo*/
void forceTourelle(Jeu* jeu, const int _ligne, const int _position, const char _type){

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
        case 'u': // Mine Explosive
            barney->pointsDeVie = 1;
            barney->degats = 0;
            barney->prix = 100;
            break;
        case 's': // Slow (Ralentissement) adjacente transperçante
            barney->pointsDeVie = 2;
            barney->degats = 1;
            barney->prix = 400;
            break;
        case 'x': // Explosive adjacente
            barney->pointsDeVie = 2;
            barney->degats = 4;
            barney->prix = 800;
            break;
        default:
            printf("Type de tourelle inconnu : %c\n", _type);
            if (barney) free(barney);
            return;
    }

    /* Mise à jour du multiplicateur de score en fonction des Barney ajoutés (aussi en fonction des étudiants) */
    jeu->multiplicateurScore /= (1 + 0.5 * barney->degats + 0.25 * barney->pointsDeVie);
    
    /* Multiplicateur min */
    if (jeu->multiplicateurScore < 1) jeu->multiplicateurScore = 1;


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
    etu->position = SPAWN_DISTANCE+1;
    etu->tour = _tour;
    etu->vitesse = 1;
    etu->touche = 'n';

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
        etu->degats = 1;
    }
    else if (_type == 'M'){ // M pour Medic
        etu->pointsDeVie = 8;
        etu->vitesse = 2;
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
