#include "video.h"
#include <stdio.h>

/* -- Ce fichier contient les fonctions utilisées pour afficher graphiquement l'état du jeu, à l'aide de SDL il complète le fichier io.c --*/


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

/* Charge une texture depuis le dossier /tex */
SDL_Texture* chargerTexture(const char* _fichier, SDL_Renderer* renderer) {
    SDL_Surface* surface = IMG_Load(_fichier);  // Charger l'image dans une surface
    if (!surface) {
        printf("Erreur lors du chargement de l'image %s: %s\n", _fichier, IMG_GetError());
        return NULL;
    }

    // Créer une texture à partir de la surface
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);  // Libérer la surface après la création de la texture

    if (!texture) {
        printf("Erreur de création de la texture: %s\n", SDL_GetError());
    }

    return texture;
}

/* Affiche une prévualisation des vagues avec SDL */
void prevualisationVagues_v(Jeu* jeu, SDL_Renderer* renderer){

    if (jeu->etudiants == NULL) {
        printf("Pas de vague.\n");
        return;
    }

    /* Chargement des textures */
    SDL_Texture* fondTexture = chargerTexture("tex/fond.png", renderer);
    SDL_Texture* zTexture = chargerTexture("tex/Z.png", renderer);
    SDL_Texture* zombie = chargerTexture("tex/zombie.png", renderer);

    int H = jeu->lastLigne;
    int L = jeu->lastTour;

    int hauteurCase = HAUTEUR_JEU / H;
    int largeurCase = hauteurCase;

    int fondWidth, fondHeight;
    SDL_QueryTexture(fondTexture, NULL, NULL, &fondWidth, &fondHeight);

    /* Efface l'écran */
    SDL_RenderClear(renderer);

    // Affiche les tuiles du fond (tilemap)
    for (int y = 0; y < HAUTEUR_JEU; y += fondHeight){

        for (int x = 0; x < LARGEUR_JEU; x += fondWidth) {

            SDL_Rect fondRect = {x, y, fondWidth, fondHeight};
            SDL_RenderCopy(renderer, fondTexture, NULL, &fondRect);

        }

    }

    Etudiant* etu = jeu->etudiants;

    /* Actualisation des textures pour les étudiants */
    while (etu != NULL) {

        if (etu->ligne <= H && etu->tour <= L) {
 
            int x = (etu->tour - 1) * largeurCase;
            int y = (etu->ligne - 1) * hauteurCase;

            SDL_Rect rect = {x, y, largeurCase, hauteurCase};

            /* Selon le type */
            if (etu->type == 'Z'){
                SDL_RenderCopy(renderer, zTexture, NULL, &rect);
            }
            else {
                SDL_RenderCopy(renderer, zombie, NULL, &rect);
            }
            
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

    while (barney != NULL){

        if (barney->ligne <= H && barney->position <= L)
        {
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

