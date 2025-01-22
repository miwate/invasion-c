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

    /* Affiche une prévualisation des vagues d'étudiants mais avec SDL*/
    void prevualisationVagues_v(Jeu* jeu, SDL_Renderer* renderer);

    /* Affiche l'état actuel du jeu mais avec SDL*/
    void renduActuelJeu_v(Jeu* jeu, SDL_Renderer* renderer);

#endif
