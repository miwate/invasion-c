#include "video.h"
#include <stdio.h>

/* Initialise la fenêtre et crée la frenêtre SDL */
SDL_Window* initSDL() {

    if (SDL_Init(SDL_INIT_VIDEO) < 0){
        printf("La fenêtre SDL ne s'initialise pas %s\n", SDL_GetError());
        return NULL;
    }

    SDL_Window* fenetre = SDL_CreateWindow("Jeu Vague d'Etudiants", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, LARGEUR_JEU, HAUTEUR_JEU, SDL_WINDOW_SHOWN);
    
    if (!fenetre){
        printf("La fenêtre SDL n'existe pas %s\n", SDL_GetError());
        SDL_Quit();
        return NULL;

    }
    return fenetre;
}

void prevualisationVagues_v(Jeu* jeu, SDL_Renderer* renderer) {
    if (jeu->etudiants == NULL){
        printf("Pas de vague.\n");
        return;
    }

    int H = jeu->lastLigne;
    int L = jeu->lastTour;
    int largeurCase = LARGEUR_JEU / L;
    int hauteurCase = HAUTEUR_JEU / H;

    /* Efface l'écran */
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // Noir
    SDL_RenderClear(renderer);

    Etudiant* etu = jeu->etudiants;

    while (etu != NULL) {
        if (etu->ligne <= H && etu->tour <= L) {
            /* Position et couleur */
            int x = (etu->tour - 1) * largeurCase;
            int y = (etu->ligne - 1) * hauteurCase;

            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // Rouge pour les étudiants
            SDL_Rect rect = {x, y, largeurCase, hauteurCase};
            SDL_RenderFillRect(renderer, &rect);
        }
        etu = etu->next;
    }

    /* Affiche les changements */
    SDL_RenderPresent(renderer);
}


void renduActuelJeu_v(Jeu* jeu, SDL_Renderer* renderer) {
    if (jeu == NULL) {
        printf("Jeu vide.\n");
        return;
    }

    int H = jeu->lastLigne;
    int L = SPAWN_AREA;  // Remplacez par la valeur appropriée
    int largeurCase = LARGEUR_JEU / L;
    int hauteurCase = HAUTEUR_JEU / H;

    /* Efface l'écran */
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // Noir
    SDL_RenderClear(renderer);

    Tourelle* barney = jeu->tourelles;

    while (barney != NULL) {
        if (barney->ligne <= H && barney->position <= L) {
            /* Position et couleur */
            int x = (barney->position - 1) * largeurCase;
            int y = (barney->ligne - 1) * hauteurCase;

            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);  // Vert pour les tourelles
            SDL_Rect rect = {x, y, largeurCase, hauteurCase};
            SDL_RenderFillRect(renderer, &rect);
        }
        barney = barney->next;
    }

    /* Affiche les changements */
    SDL_RenderPresent(renderer);
}
