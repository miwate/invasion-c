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

    SDL_Renderer* rendu = SDL_CreateRenderer(window, -1, 2);
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

    /* CHargement police */
    TTF_Init();
    TTF_Font* police = TTF_OpenFont("tex/montserrat.ttf", 30);


    /* JOuer la musique */
    Mix_OpenAudio(20000, MIX_DEFAULT_FORMAT, 2, 4096);
    Mix_Music* theme = Mix_LoadMUS("audio/invasion.mp3");
    Mix_PlayMusic(theme, -1);

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
        printf("Multiplicateur de score : %f\n", jeu->multiplicateurScore);

        /* Partie dialogue */
        int dialogueSkip = 1;
        if (dialogueSkip == 1){
            dialogue(rendu, "me_zany", "Miwate : Salut !", police);
            printf("Miwate : Salut !\n");
            sleep(3);

            prevualisationVagues_v(jeu, rendu);
            dialogue(rendu, "me_sleepy", "Miwate : Je suis celui qui dort en cours.", police);
            printf("Miwate : Je suis celui qui dort en cours.\n");
            sleep(6);
        }
        

        renduActuelJeu_v(jeu, rendu);
        sleep(1);

        // Sauvegarde
        printf("Si vous voulez sauvegarder l'avancement au cours de la partie, entrez S pour sauvegarder !\n");
        int sauver = 0;
        while(jeu->fin == 1){
            questionTourelle(jeu, &sauver);
            if (sauver == 1){
                SDL_DestroyRenderer(rendu);
                SDL_DestroyWindow(window);
                SDL_Quit();
                printf("Sauvegarde effectuée -> Fin du programme.\n");
                return 0;
            }
            
            //printf("Appel à rafraichirJeu\n");
            rafraichirJeu(jeu);
            //printf("Appel à renduActuelJeu_v\n");
            renduActuelJeu_v(jeu, rendu);
            //printf("Appel à renduActuelJeu\n");
            renduActuelJeu(jeu);
            // SDL_Delay(500); // Pause de 500 millisecondes
        }

        /* Libération de la mémoire, fermeture des fenêtres */
        SDL_DestroyRenderer(rendu);
        SDL_DestroyWindow(window);
        SDL_Quit();
        TTF_CloseFont(police);
        TTF_Quit();

        char *scoreFichier = "lb/scores.txt";
        enregistrerScore(scoreFichier, jeu->score, jeu->fin);

    }

    /* Libération de la musique */
    Mix_FreeMusic(theme);
    Mix_CloseAudio();

    printf("Jeu terminé.\n");
    return 0;
}
