#include "../prot/jeu.h"
#include <stdio.h>
#include <stdlib.h>

void initJeu(Jeu* jeu) {
    jeu->tourelles = NULL;
    jeu->etudiants = NULL;
    jeu->cagnotte = 0;
    jeu->tour = 0;
    jeu->lastTour = 0;
    jeu->lastLigne = 0;
}

/* Charge le niveau du fichier texte dans la structure jeu donnée en paramètre*/
void loadFichier(Jeu* jeu, const char* _niveau) {

    FILE* niveau = fopen(_niveau, "r");
    if (niveau == NULL) {
        printf("Niveau %s introuvable.\n", _niveau);
        return;
    }

    printf("Lecture de %s\n", _niveau);
    fscanf(niveau, "%d", &jeu->cagnotte);

    int tour, ligne;
    int hauteur = 0;
    char type;

    while (fscanf(niveau, "%d %d %c", &tour, &ligne, &type) == 3) {
        if (ligne > hauteur) {
            hauteur = ligne;
        }
        addEtudiant(jeu, tour, ligne, type);
    }

    jeu->lastLigne = hauteur;
    jeu->lastTour = tour;
    fclose(niveau);
}

void previewVagues(Jeu* jeu) {
    if (jeu->etudiants == NULL) {
        printf("Pas de vague.\n");
        return;
    }
    int H = jeu->lastLigne;
    int L = jeu->lastTour;

    char** preview = (char**)malloc(H * sizeof(char*));
    for (int i = 0; i < H; i++) {
        preview[i] = (char*)malloc(L * sizeof(char));
    }
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < L; j++) {
            preview[i][j] = '.';
        }
    }

    Etudiant* etu = jeu->etudiants;
    while (etu != NULL){
        if (etu->ligne <= H && etu->tour <= L) {
            preview[etu->ligne - 1][etu->tour - 1] = etu->type;
        }
        etu = etu->next;
    }

    printf("\033[2J");
    printf("\033[0;0H");
    printf("Preview de la vague\n");

    for (int i=0; i<H; i++) {
        printf("%d|\t", i + 1);
        for (int j = 0; j < L; j++) {
            printf("%c\t", preview[i][j]);
        }
        printf("\n");
    }

    for (int i=0; i<H; i++) {
        free(preview[i]);
    }
    free(preview);
}
