#include <stdio.h>
#include <string.h>

#define ZONE_SAFE 6

typedef struct tourelle {
    int type;
    int pointsDeVie;
    int ligne;
    int position;
    int prix;
    struct tourelle* next;
} Tourelle;

typedef struct etudiant {
    int type;
    int pointsDeVie;
    int ligne;
    int position;
    int vitesse;
    int tour;
    struct etudiant* next; //on va utiliser ça pour l'étudiant suivant de la même ligne
    struct etudiant* next_line; //prochain de la ligne d'après
    struct etudiant* prev_line; //William : premier de la ligne d'avant, mais je pense pas qu'on l'utilisera
} Etudiant;

typedef struct {
    Tourelle* tourelles;
    Etudiant* etudiants;
    int cagnotte;
    int tour;
    int lastTour;
    int lastLigne;
} Jeu;

void addEtudiant(Jeu* jeu, const int _tour, const int _ligne, const int _type){
    Etudiant* etu = malloc(sizeof(Etudiant));
    if (etu == NULL) {
        printf("Impossible d'ajouter l'étudiant %d %d %d.\n", _tour, _ligne, _type);
        return;
    }
    etu->type = _type;
    etu->ligne = _ligne;
    etu->position = 0;
    etu->tour = _tour;

    if (_type == 'Z') {
        etu->pointsDeVie = 3;
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

void previewVague(Jeu* jeu){
    if (jeu->etudiants == NULL){
        printf("Pas de vague.\n");
        return;
    }
    char preview[jeu->lastTour][jeu->lastLigne];

    for (int i=0; i<jeu->lastTour; i++){
        for(int j=0; j<jeu->lastLigne; j++){
            preview[i][j]='.';
        }
    }
    
    int ligne = 1;
    Etudiant* etu = jeu->etudiants;

    while (etu != NULL){
        while (ligne < jeu->lastLigne){
            if (etu->ligne == ligne){
                    preview[etu->tour][ligne] = etu->type;
                }
                etu = etu->next;
            }
            ligne++;
    }

    printf("Preview de la vague\n");

    for (int i=0; i<jeu->lastTour; i++){
        printf("%d|\t", i+1);

        for(int j=0; j<jeu->lastLigne; j++){
            printf("%c\t", preview[i][j]);
        }
        printf("\n");
    }

}

void loadFichier(Jeu* jeu, const char* _niveau){
    char niveauTexte[64];
    strcpy(niveauTexte, _niveau);
    strcat(niveauTexte, ".txt"); //pour ajouter .txt

    FILE* niveau = fopen(niveauTexte, "r");
    if (niveau == NULL) {
        printf("Niveau %s introuvable.\n", _niveau);
        return;
    }

    fscanf(niveau, "%d", &jeu->cagnotte);
    int tour, ligne, type;
    int hauteur = 0;

    while (fscanf(niveau, "%d %d %c", &tour, &ligne, &type) == 3) {
        if (ligne > hauteur)
            hauteur = ligne;
        addEtudiant(jeu, tour, ligne, type);
    }
    jeu->lastTour = tour;
    fclose(niveau);

}



int main(int argc, char *argv[]){
    return 0;
}
