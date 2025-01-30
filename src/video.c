#include "video.h"
#include <stdio.h>

/* -- Ce fichier contient les fonctions utilisées pour afficher graphiquement l'état du jeu à l'aide de SDL, il complète le fichier io.c --*/

/* Initialise la fenêtre et crée la frenêtre SDL */
SDL_Window* initSDL() {

    if (SDL_Init(SDL_INIT_VIDEO) < 0){
        printf("La fenêtre SDL ne s'initialise pas\n");
        return NULL;
    }
    SDL_Window* fenetre = SDL_CreateWindow("INVASION !!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, LARGEUR_JEU, HAUTEUR_JEU, SDL_WINDOW_SHOWN);
    
    return fenetre;
}

/* Charge une texture depuis le dossier /tex */
SDL_Texture* chargerTexture(const char* _fichier, SDL_Renderer* rendu) {
    SDL_Surface* tex = IMG_Load(_fichier);
    if (!tex) {
        printf("Erreur chargement de l'image %s:\n", _fichier);
        return NULL;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(rendu, tex);
    SDL_FreeSurface(tex);

    if (!texture) {
        printf("Erreur création de texture\n");
    }

    return texture;
}

/* Affiche une prévualisation des vagues avec SDL */
void prevualisationVagues_v(Jeu* jeu, SDL_Renderer* rendu){

    if (jeu->etudiants == NULL) {
        printf("Pas de vague.\n");
        return;
    }

    /* Chargement des textures */
    SDL_Texture* fondTexture = chargerTexture("tex/fond.png", rendu);
    
    int H = jeu->derniereLigne;
    int L = jeu->dernierTour;

    int hauteurCase = HAUTEUR_JEU / H;
    int largeurCase = hauteurCase;

    int fondLargeur, fondHauteur;
    SDL_QueryTexture(fondTexture, NULL, NULL, &fondLargeur, &fondHauteur);

    /* Efface l'écran */
    SDL_RenderClear(rendu);

    /* Affiche le fond */
    for (int y = 0; y < HAUTEUR_JEU; y += fondHauteur){

        for (int x = 0; x < LARGEUR_JEU; x += fondLargeur) {

            SDL_Rect fondRect = {x, y, fondLargeur, fondHauteur};
            SDL_RenderCopy(rendu, fondTexture, NULL, &fondRect);

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
            char etuTex[32];
            snprintf(etuTex, sizeof(etuTex), "tex/%c.png", etu->type);
            SDL_Texture* etudiantTex = chargerTexture(etuTex, rendu);

            SDL_RenderCopy(rendu, etudiantTex, NULL, &rect);

        }
        etu = etu->next;

    }

    /* Affiche les changements */
    SDL_RenderPresent(rendu);
}

/* Affiche un étudiant selon la position la texture et ses points de vie */
void afficherEtudiant(SDL_Renderer* rendu, SDL_Texture* etudiantTexture, SDL_Texture* pointVieTexture, int x, int y, int largeurCase, int hauteurCase, int pointsDeVie) {
    SDL_Rect rect = {x, y, largeurCase, hauteurCase};
    SDL_RenderCopy(rendu, etudiantTexture, NULL, &rect);

    /* affichage des points de vie*/
    if (pointsDeVie > 0 && pointsDeVie <= 9) {

        SDL_Rect pointRect = {x, y, largeurCase / 2, hauteurCase};
        SDL_RenderCopy(rendu, pointVieTexture, NULL, &pointRect);

        /* LIbération de la mémoire */
        SDL_DestroyTexture(pointVieTexture);

    }
    /* LIbération de la mémoire */
    SDL_DestroyTexture(etudiantTexture);


}

/* Affiche le score du jeu en haut à droite de la fenêtre */
void afficherScore(SDL_Renderer* rendu, int score, int x, int y){

    /* Convertit le score (int) en chaîne de charactères */
    char scoreCh[32];
    snprintf(scoreCh, sizeof(scoreCh), "%d", score);

    /* Affiche chiffre par chiffre le score */
    for (int i=0; i < strlen(scoreCh); i++) {
        char chiffre = scoreCh[i];

        /* Récupération de la texture du chiffre */
        char chiffreTex[32];
        snprintf(chiffreTex, sizeof(chiffreTex), "tex/score-%c.png", chiffre);
        SDL_Texture* _chiffreTex = chargerTexture(chiffreTex, rendu);

        SDL_Rect chiffreRect = {x +(i*20), y, 20, 30};

        SDL_RenderCopy(rendu, _chiffreTex, NULL, &chiffreRect);

        /* LIbération de la mémoire */
        SDL_DestroyTexture(_chiffreTex);
    }
}

/* Affiche la cagnotte du jeu en haut à droite de la fenêtre */
void afficherCagnotte(SDL_Renderer* rendu, int _cagnotte, int x, int y){

    /* Convertit la cagnotte (int) en chaîne de charactères */
    char cagnotteCh[32];
    snprintf(cagnotteCh, sizeof(cagnotteCh), "%d", _cagnotte);

    /* Affiche l'icône EURO */
    SDL_Texture* _euroTex = chargerTexture("tex/EURO.png", rendu);
    SDL_Rect euroRect = {x - 35, y, 20, 30};
    SDL_RenderCopy(rendu, _euroTex, NULL, &euroRect);

    /* Affiche chiffre par chiffre le score */
    for (int i = 0; i < strlen(cagnotteCh) ; i++) {
        char chiffre = cagnotteCh[i];

        /* Récupération de la texture du chiffre */
        char chiffreTex[32];
        snprintf(chiffreTex, sizeof(chiffreTex), "tex/score-%c.png", chiffre);
        SDL_Texture* _chiffreTex = chargerTexture(chiffreTex, rendu);

        SDL_Rect chiffreRect = {x+(i*20), y, 20, 30};

        SDL_RenderCopy(rendu, _chiffreTex, NULL, &chiffreRect);

        /* LIbération de la mémoire */
        SDL_DestroyTexture(_chiffreTex);
    }
    /* LIbération de la mémoire */
    SDL_DestroyTexture(_euroTex);
}


/* Affiche la fenêtre et affiche l'état actuel du jeu */
void renduActuelJeu_v(Jeu* jeu, SDL_Renderer* rendu) {
    if (jeu == NULL) {
        printf("Jeu vide.\n");
        return;
    }

    int H = jeu->derniereLigne;
    int L = SPAWN_DISTANCE;
    int largeurCase = LARGEUR_JEU / L;
    int hauteurCase = HAUTEUR_JEU / H;

    /* Efface l'écran */
    SDL_RenderClear(rendu);


    SDL_Texture* fondTexture = chargerTexture("tex/fond.png", rendu);
    SDL_Texture* tourelleTexture = chargerTexture("tex/turret.png", rendu);


    int fondLargeur, fondHauteur;
    SDL_QueryTexture(fondTexture, NULL, NULL, &fondLargeur, &fondHauteur);

    /* Affiche le fond */
    for (int y = 0; y < HAUTEUR_JEU; y += fondHauteur){

        for (int x = 0; x < LARGEUR_JEU; x += fondLargeur) {

            SDL_Rect fondRect = {x, y, fondLargeur, fondHauteur};
            SDL_RenderCopy(rendu, fondTexture, NULL, &fondRect);

        }

    }


    /* Affiche les tourelles */
    Tourelle* barney = jeu->tourelles;

    while (barney != NULL){

        if (barney->ligne <= H && barney->position <= L)
        {
            /* Position */
            int x = (barney->position - 1) * largeurCase;
            int y = (barney->ligne - 1) * hauteurCase;
            
            // Affichage de la tourelle
            char tourelleTex[32];
            snprintf(tourelleTex, sizeof(tourelleTex), "tex/%c.png", barney->type);
            SDL_Texture* tourelleTexture = chargerTexture(tourelleTex, rendu);
            SDL_Rect rect = {x, y, largeurCase, hauteurCase};
            SDL_RenderCopy(rendu, tourelleTexture, NULL, &rect);
            
            /* Selon le type et affichage */
            char pvTex[32];
            snprintf(pvTex, sizeof(pvTex), "tex/default-%d.png", barney->pointsDeVie);
            SDL_Texture* pvTexture = chargerTexture(pvTex, rendu);

            SDL_Rect rect = {x, y, largeurCase, hauteurCase};
            SDL_RenderCopy(rendu, tourelleTexture, NULL, &rect);
            SDL_Rect pointRect = {x, y, largeurCase / 2, hauteurCase};
            SDL_RenderCopy(rendu, pvTexture, NULL, &pointRect);

            // Libération mémoire - peut-être pas nécessaire vu que la boucle de texture pour les étudiants marche sans libérer
            SDL_DestroyTexture(tourelleTexture);
            SDL_DestroyTexture(pvTexture);
        }
        barney = barney->next;

    }

    Etudiant* etu = jeu->etudiants;

    /* Actualisation des textures pour les étudiants */
    while (etu != NULL) {
    
        if (etu->tour == jeu->tour){
 
            int x = (etu->position-1) * largeurCase;
            int y = (etu->ligne-1) * hauteurCase;

            /* Selon le type, affichage des textures et des points de vie */
            char pvTex[32];
            snprintf(pvTex, sizeof(pvTex), "tex/default-%d.png", etu->pointsDeVie);
            SDL_Texture* pvTexture = chargerTexture(pvTex, rendu);

            char etuTex[32];
            snprintf(etuTex, sizeof(etuTex), "tex/%c.png", etu->type);
            SDL_Texture* etudiantTex = chargerTexture(etuTex, rendu);

            afficherEtudiant(rendu, etudiantTex, pvTexture, x, y, largeurCase, hauteurCase, etu->pointsDeVie);


        }
        etu = etu->next;

    }
    /* Affiche le score */
    afficherScore(rendu, jeu->score, LARGEUR_JEU - 250, 10);

    /* Affiche la cagnotte */
    afficherCagnotte(rendu, jeu->cagnotte, LARGEUR_JEU - 250, 50);
    
    /* Affiche les changements */
    SDL_RenderPresent(rendu);

}

