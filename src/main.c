#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../prot/jeu.h"
#include "../prot/video.h"

/* --Ce fichier démarre une partie et nécessite tous les autres fichiers sources et prototypes pour fonctionner correctement --*/
/* (C'est un fichier main) */


/* Demo placer des tourelles sans demander */
void forceTourellesDEMO(Jeu* jeu){
    
    if (jeu->tour == 4) {
    forceTourelle(jeu, 2,2,'t');  
    forceTourelle(jeu, 5,6,'x');
    forceTourelle(jeu, 7,1,'t');
    forceTourelle(jeu, 1,1,'x');
    forceTourelle(jeu, 5,1,'t');
    }

    else if (jeu->tour == 8){
    forceTourelle(jeu, 2,3,'u');
    forceTourelle(jeu, 7,2,'t');
    forceTourelle(jeu, 3,4,'u');
    forceTourelle(jeu, 3,1,'t');
    forceTourelle(jeu, 3,2,'s');
    }

    else if (jeu->tour == 1) {
    forceTourelle(jeu, 4,5,'u');
    forceTourelle(jeu, 2,1,'t');
    forceTourelle(jeu, 2,7,'u');
    forceTourelle(jeu, 7,7,'u');
    forceTourelle(jeu, 1,5,'t'); 
    forceTourelle(jeu, 5,7,'b');    
    forceTourelle(jeu, 3,6,'b');
    forceTourelle(jeu, 3,5,'t');
    }
}

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
    TTF_Font* police = TTF_OpenFont("tex/liberation.ttf", 25);


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

        /* DEMO ? */
        int DEMO = 0;
        if (jeu->cagnotte < 0) DEMO = 1;
        if (jeu->cagnotte < 0) jeu->cagnotte = 10;

        /* Prévisualisation des vagues */
        printf("Prévisualisation des vagues - Cagnotte : %d€\n", jeu->cagnotte);
        prevualisationVagues(jeu);
        prevualisationVagues_v(jeu, rendu);

        /* Partie dialogue | 1 normal, 0 pour skip */
        int dialogueSkip = 1;

        if (dialogueSkip == 1){
            dialogue(rendu, "me_zany", "Dev  : Salut !", police);
            printf("Dev : Salut !\n");
            SDL_Delay(1000);

            prevualisationVagues_v(jeu, rendu);
            dialogue(rendu, "me_sleepy", "Dev : Je suis celui qui dort en cours.", police);
            printf("Dev : Je suis celui qui dort en cours.\n");
            SDL_Delay(2500);

            prevualisationVagues_v(jeu, rendu);
            dialogue(rendu, "me_sleepy", "Dev : Attention ! Zzzz...", police);
            printf("Dev : Attention ! Zzzz...\n");
            SDL_Delay(2500);
        }
        

        renduActuelJeu_v(jeu, rendu);
        SDL_Delay(1000);

        // Sauvegarde
        printf("\033[2J\033[0;0H");
        printf("Si vous voulez sauvegarder l'avancement au cours de la partie, entrez S pour sauvegarder !\n");
        int sauver = 0;

        /* Déroulé du jeu */
        SDL_Event event;
        int enMarche = 1;

        while(jeu->fin == 1 && enMarche){
            SDL_PollEvent(&event);

            /* Démonstration */
            if (DEMO == 1) forceTourellesDEMO(jeu);

            
            renduActuelJeu_v(jeu, rendu);
            renduActuelJeu(jeu);
            rafraichirJeu(jeu);


            if (DEMO != 1) questionTourelle(jeu, &sauver);

            if (dialogueSkip == 1 && jeu->premierSang == 'o'){

                dialogue(rendu, "me_sleepy", "Dev : Premier sang pour toi, attention !!", police);
                printf("Dev : Premier sang pour toi, attention !!\n");
                jeu->premierSang = 'f';
                SDL_Delay(3000);
            }
            
            if (sauver == 1){
                if (rendu) SDL_DestroyRenderer(rendu);
                if (window) SDL_DestroyWindow(window);
                SDL_Quit();
                printf("Sauvegarde effectuée -> Fin du programme.\n");
                return 0;
            }
                        
            renduActuelJeu_v(jeu, rendu);
            SDL_Delay(1000);
        }

        if (dialogueSkip == 1){
            char* bravo;
            bravo = bravoHasard(jeu->fin);
            char texte[128];

            if (jeu->fin == -1) {
                sprintf(texte, "Dev : %s, tu as perdu. On se reverra.", bravo);
            }
            else {
                sprintf(texte, "Dev : %s, tu n'as pas perdu. On se reverra.", bravo);
            }
            dialogue(rendu, "me_zany", texte, police);
            printf("%s\n", texte);
            SDL_Delay(5000);
        }

        /* Libération de la mémoire, fermeture des fenêtres */
        if (rendu) SDL_DestroyRenderer(rendu);
        if (window) SDL_DestroyWindow(window);
        SDL_Quit();
        TTF_CloseFont(police);
        TTF_Quit();

    }

    char *scoreFichier = "lb/scores.txt";
    enregistrerScore(scoreFichier, jeu->score, jeu->fin);
    triScores(scoreFichier);

    /* Libération de la musique */
    Mix_FreeMusic(theme);
    Mix_CloseAudio();

    printf("Jeu terminé.\n");
    return 0;
}
