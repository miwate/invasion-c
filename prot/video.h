#ifndef VIDEO_H

    #define VIDEO_H

    #include <SDL2/SDL.h>
    #include "../prot/jeu.h"

    /* DImensions des fenêtres */
    #define TAILLE_CASE 80
    #define LARGEUR_JEU TAILLE_CASE*SPAWN_AREA
    #define HAUTEUR_JEU TAILLE_CASE*7

    /* Textures pour l'affichage */
    extern SDL_Texture* texFond;

    /* Crée la fenêtre SDL */
    SDL_Window* initSDL();

    /* Fonction pour dessiner la grille et les étudiants */
    void dessinGrille(SDL_Renderer* renderer, Jeu* jeu);

    /* Fonction pour gérer les événements et afficher la fenêtre */
    void afficherGrille(SDL_Window* window, SDL_Renderer* renderer, Jeu* jeu);

#endif
