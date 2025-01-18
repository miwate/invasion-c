#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ZONE_SAFE 6

typedef struct tourelle {
    int pointsDeVie;
    int ligne;
    int position;
    int prix;
    struct tourelle* next;
    char type;
} Tourelle;

typedef struct etudiant {
    int pointsDeVie;
    int ligne;
    int position;
    int vitesse;
    int tour;
    struct etudiant* next; //on va utiliser ça pour l'étudiant suivant de la même ligne
    struct etudiant* next_line; //prochain de la ligne d'après
    struct etudiant* prev_line; //remier de la ligne d'avant, mais je pense pas qu'on l'utilisera
    char type;
} Etudiant;

typedef struct {
    Tourelle* tourelles;
    Etudiant* etudiants;
    int cagnotte;
    int tour;
    int lastTour;
    int lastLigne;
} Jeu;

void initJeu(Jeu* jeu) {
    jeu->tourelles = NULL;
    jeu->etudiants = NULL;
    jeu->cagnotte = 0;
    jeu->tour = 0;
    jeu->lastTour = 0;
    jeu->lastLigne = 0;
}


void addEtudiant(Jeu* jeu, const int _tour, const int _ligne, const char _type){
    Etudiant* etu = malloc(sizeof(Etudiant));
    if (etu == NULL) {
        printf("Impossible d'ajouter l'étudiant %d %d %c.\n", _tour, _ligne, _type);
        return;
    }

    //printf("Ajout du zombie %d %d %c\n", _tour, _ligne, _type); //degub
    
    etu->type = _type;
    etu->ligne = _ligne;
    etu->position = 0;
    etu->tour = _tour;

    if (_type == 'Z') {
        etu->pointsDeVie = 5;
        etu->vitesse = 1;
    }
    else { // on va ajouter d'autres types de zombies
        etu->pointsDeVie = 1;
        etu->vitesse = 1;
    }

    etu->next = NULL;
    etu->next_line = NULL;
    etu->prev_line = NULL;

    if (jeu->etudiants == NULL){
        //printf("si ça marche pas je pète un câble\n"); //debug
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

    //printf("Zombie %d %d %c ajouté\n", _tour, _ligne, _type); //degub
    
}

void previewVagues(Jeu* jeu){
    if (jeu->etudiants == NULL){
        printf("Pas de vague.\n");
        return;
    }
    int H, L;
    H = jeu->lastLigne;
    L = jeu->lastTour;
    printf("H%d, L%d\n", H, L); //debug

    char** preview = (char**)malloc(H*sizeof(char*));
    for (int i=0; i<H; i++){
        preview[i] = (char*)malloc(L*sizeof(char));
    }
    for (int i=0; i<H; i++){
        for (int j=0; j<L; j++){
            preview[i][j] = '.';
        }
    }
    Etudiant* etu = jeu->etudiants;
    while (etu != NULL){
        if (etu->ligne<=H && etu->tour<=L) {
            preview[etu->ligne-1][etu->tour-1] = etu->type;
        }
        //printf("%c", etu->type); //debgub
        etu = etu->next;
    }

    printf("\033[2J"); // on efface tout
    printf("\033[0;0H"); // on se place en haut à gauche
    printf("Preview de la vague\n");

    for(int i=0; i<H; i++){
        printf("%d|\t", i+1);
        for (int j=0; j<=L; j++) {
            printf("%c\t", preview[i][j]);
        }
        printf("\n");
    }

    for (int i=0; i<H; i++) {
        free(preview[i]);
    }
    free(preview);
}


void loadFichier(Jeu* jeu, const char* _niveau){

    FILE* niveau = fopen(_niveau, "r");

    if (niveau == NULL) {
        printf("Niveau %s introuvable.\n", _niveau);
        return;
    }

    printf("Lecture de %s\n", _niveau);
    fscanf(niveau, "%d", &jeu->cagnotte);
    
    //printf("Cagnotte %d\n", jeu->cagnotte); //debug

    int tour, ligne;
    int hauteur = 0;
    char type;

    while (fscanf(niveau, "%d %d %c", &tour, &ligne, &type) == 3) {

        //printf("%d %d %c \n", tour, ligne, type);//debug

        if (ligne > hauteur){
            hauteur = ligne;
        }
        addEtudiant(jeu, tour, ligne, type);
    }

    jeu->lastLigne = hauteur;
    jeu->lastTour = tour;

    fclose(niveau);
    //printf("Lecture de %s terminée\n", _niveau);

}



int main(int argc, char *argv[]){
    if (argc <2) {
        printf("Aucun fichier donné.\n");
        return 1;
    }

    for(int i=1; i<argc; i++){
        printf("%s\n", argv[i]);

        Jeu* jeu = malloc(sizeof(Jeu));
        if (jeu == NULL) {
            printf("Erreur : Jeu\n");
            return 1;
        }

        initJeu(jeu);
        loadFichier(jeu, argv[i]);
        previewVagues(jeu);
        free(jeu);
    }
    printf("fin");
    return 0;
}
