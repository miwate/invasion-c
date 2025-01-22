#include "video.h"
#include <stdio.h>

/* Fonction pour initialiser SDL et créer la fenêtre */
SDL_Window* initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Erreur SDL: %s\n", SDL_GetError());
        return NULL;
    }
    SDL_Window* window = SDL_CreateWindow("Jeu Vague d'Etudiants",
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          LARGEUR_JEU, HAUTEUR_JEU, SDL_WINDOW_SHOWN);
    if (!window) {
        printf("Erreur de création de fenêtre: %s\n", SDL_GetError());
        SDL_Quit();
        return NULL;
    }
    return window;
}

/* Fonction pour dessiner la grille et les étudiants */
void dessinGrille(SDL_Renderer* renderer, Jeu* jeu) {
    int H = jeu->lastLigne;
    int L = jeu->lastTour;

    // Dessiner la grille
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < L; j++) {
            SDL_Rect case_ = { j * TAILLE_CASE, i * TAILLE_CASE, TAILLE_CASE, TAILLE_CASE };
            // Colorier les cases en blanc
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderFillRect(renderer, &case_);

            // Dessiner une bordure
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderDrawRect(renderer, &case_);
        }
    }

    // Dessiner les étudiants dans la grille
    Etudiant* etu = jeu->etudiants;
    while (etu != NULL) {
        if (etu->ligne <= H && etu->tour <= L) {
            SDL_Rect case_ = { (etu->tour - 1) * TAILLE_CASE, (etu->ligne - 1) * TAILLE_CASE, TAILLE_CASE, TAILLE_CASE };
            if (etu->type == 'A') { // Exemple : type 'A' pour un étudiant particulier
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // Rouge
            } else {
                SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);  // Vert
            }
            SDL_RenderFillRect(renderer, &case_);
        }
        etu = etu->next;
    }
}

/* Fonction pour gérer les événements et afficher la fenêtre */
void afficherGrille(SDL_Window* window, SDL_Renderer* renderer, Jeu* jeu) {
    int quitter = 0;
    SDL_Event event;

    while (!quitter) {
        // Gestion des événements
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quitter = 1;
            }
        }

        // Effacer l'écran
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // Noir
        SDL_RenderClear(renderer);

        // Dessiner la grille et les étudiants
        dessinGrille(renderer, jeu);

        // Mettre à jour l'écran
        SDL_RenderPresent(renderer);
    }
}
