#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include "../prot/jeu.h"
#include "video.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Aucun fichier donné.\n");
        return 1;
    }

    // Initialisation de SDL et de la fenêtre
    SDL_Window* window = initSDL();
    if (!window) {
        return 1;
    }
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Erreur de création de renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Jeu et initialisation
    Jeu* jeu = malloc(sizeof(Jeu));
    if (jeu == NULL) {
        printf("Erreur d'allocation mémoire pour le jeu\n");
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    initJeu(jeu);

    // Charger le fichier de niveau
    chargerFichier(jeu, argv[1]);

    afficherGrille(window, renderer, jeu);

    // Libération des ressources
    free(jeu);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
