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
    struct etudiant* prev_line; //premier de la ligne d'avant
} Etudiant;

typedef struct {
    Tourelle* tourelles;
    Etudiant* etudiants;
    int cagnotte;
    int tour;
    int toursMax;
} Jeu;

void addEtudiant(Jeu* jeu, const int _tour, const int _ligne, const int _type){
    Etudiant* etu = malloc(sizeof(Etudiant));
    if (etu == NULL) {
        printf("Impossible d'ajouter l'étudiant %d %d %d.\n", _tour, _ligne, _type);
        return;
    }
    etu->type = _type;
    etu->ligne = _ligne;
    etu->position = ZONE_SAFE;
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

    etu->type = _type;
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

    while (fscanf(niveau, "%d %d %c", &tour, &ligne, &type) == 3) {

    }

}

int main(){
    return 0;
}
