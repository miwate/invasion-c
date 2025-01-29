#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../prot/jeu.h"
#include "../prot/video.h"

/* --Ce fichier démarre une partie et nécessite tous les autres fichiers sources et prototypes pour fonctionner correctement --*/
/* (C'est un fichier main) */

/* Lance une partie */
int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Aucun fichier donné.\n");
        return 1;
    }

    /* INitialisation de la fenêtre SDL */
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

    /* POur tous les fichiers entrées en arguments */
    for (int i = 1; i < argc; i++){

        /* INitialisation du jeu*/
        initJeu(jeu);

        /* Chargement du niveau */
        printf("Chargement du niveau : %s\n", argv[i]);
        chargerFichier(jeu, argv[i]);

        /* Prévisualisation des vagues */
        printf("Prévisualisation des vagues - Cagnotte : %d€\n", jeu->cagnotte);
        prevualisationVagues(jeu);
        prevualisationVagues_v(jeu, rendu);

        /* Début d'un compteur pour la prévualisation */
        int compteur = 2;
        printf("Début du jeu dans %d...\n", compteur);

        while (compteur > 0){
            printf("%d\n", compteur);
            sleep(1);
            compteur -= 1;
        }

        printf("%f\n", jeu->multiplicateurScore);
        ajoutTourelle(jeu, 1,1,'T');
        ajoutTourelle(jeu, 2,1,'T');
        ajoutTourelle(jeu, 3,2,'T');
        ajoutTourelle(jeu, 4,2,'T');
        ajoutTourelle(jeu, 5,2,'T');
        ajoutTourelle(jeu, 6,2,'T');
        ajoutTourelle(jeu, 7,2,'T');


        printf("%f\n", jeu->multiplicateurScore);

        renduActuelJeu_v(jeu, rendu);
        sleep(1);

        while(jeu->fin == 1){

            rafraichirJeu(jeu);
            renduActuelJeu_v(jeu, rendu);
            renduActuelJeu(jeu);
            sleep(0.5);
 
        }

        /* Libération de la mémoire, fermeture des fenêtres */
        SDL_DestroyRenderer(rendu);
        SDL_DestroyWindow(window);
        SDL_Quit();

        char *scoreFichier = "lb/scores.txt";
        enregistrerScore(scoreFichier, jeu->score, jeu->fin);

    }

    printf("Jeu terminé.\n");
    return 0;
}