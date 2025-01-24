#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
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

    SDL_Renderer* rendu = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!rendu) {
        printf("Erreur de création de rendu: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    /* Jeu et initialisation */
    Jeu* jeu = malloc(sizeof(Jeu));
    if (jeu == NULL) {
        printf("Erreur d'allocation mémoire pour le jeu\n");
        SDL_DestroyRenderer(rendu);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    for (int i = 1; i < argc; i++){
        /* INitialisation du jeu*/
        initJeu(jeu);

        /* Chargement du niveau */
        printf("Traitement du fichier : %s\n", argv[i]);
        chargerFichier(jeu, argv[i]);

        /* Prévisualisation des vagues */
        printf("Prévisualisation des vagues - Cagnotte : %d€\n", jeu->cagnotte);
        prevualisationVagues(jeu);
        prevualisationVagues_v(jeu, rendu);

        /* Début d'un compteur pour la prévualisation */
        int compteur = 1;
        printf("Début du jeu dans %d...\n", compteur);

        while (compteur > 0){
            printf("%d\n", compteur);
            sleep(1);
            compteur -= 1;
        }

        
        ajoutTourelle(jeu, 1,2,'T');

        printf("Affichage de l'état actuel du jeu\n");
        renduActuelJeu_v(jeu, rendu);


        printf("Appuyez sur Entrée pour terminer...\n");
        getchar();



    }
    // Libération de la mémoire allouée pour le jeu
    free(jeu);
    SDL_DestroyRenderer(rendu);
    SDL_DestroyWindow(window);
    SDL_Quit();

    printf("Fin du traitement des fichiers.\n");
    return 0;
}