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

    for (int i = 1; i < argc; i++){
        printf("Traitement du fichier : %s\n", argv[i]);

        // Allocation de la mémoire pour un nouveau jeu
        Jeu* jeu = malloc(sizeof(Jeu));
        if (jeu == NULL) {
            printf("Erreur : Allocation mémoire pour Jeu\n");
            return 1;
        }

        // Initialisation du jeu
        initJeu(jeu);

        // Chargement des données depuis le fichier
        chargerFichier(jeu, argv[i]);

        /* Prévisualisation des vagues */
        printf("Prévisualisation des vagues\n");
        prevualisationVagues(jeu);
        prevualisationVagues_v(jeu, renderer);

        printf("Appuyez sur Entrée pour jouer...\n");
        getchar();

        printf("Affichage de l'état actuel du jeu\n");
        renduActuelJeu_v(jeu, renderer);  // Assurez-vous que `renderer` est correctement initialisé ici

        // Libération de la mémoire allouée pour le jeu
        free(jeu);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    printf("Fin du traitement des fichiers.\n");
    return 0;
}