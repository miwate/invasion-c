#ifndef JEU_H
    #define JEU_H
    #define SPAWN_AREA 15
    #define REPOS 5

    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>

/* -- Définitions des structures -- */
    typedef struct tourelle{
        int pointsDeVie;
        int degats;
        int ligne;
        int position;
        int prix;
        struct tourelle* next;
        char type;
    } Tourelle;

    typedef struct etudiant{
        int pointsDeVie;
        int ligne;
        int position;
        int vitesse;
        int tour;
        struct etudiant* next;
        struct etudiant* next_line;
        struct etudiant* prev_line;
        char type;
    } Etudiant;

    typedef struct jeu{
        Tourelle* tourelles;
        Etudiant* etudiants;
        int cagnotte;
        int tour;
        int lastTour;
        int lastLigne;
        int score;
    } Jeu;


/* -- Protoytypes de fonctions -- */

    /* Initialise un jeu vide*/
    void initJeu(Jeu* jeu);
    
    /* Rafraîchit le jeu pour un tour suivant */
    void rafraichirJeu(Jeu* jeu);

    /* Ajoute un étudiant dans la file en fonction de son type*/
    void ajoutEtudiant(Jeu* jeu, const int _tour, const int _ligne, const char _type);

    /* Ajoute une tourelle dans la file */
    void ajoutTourelle(Jeu* jeu, const int _ligne, const int _position, const char _type);

    /* Affiche l'état actuel du jeu (tourelles + étudiants)*/
    void renduActuelJeu(Jeu* jeu);

    /* Prévualise la vague d'étudiants dans le terminal*/
    void prevualisationVagues(Jeu* jeu);

    /* Charge un fichier contenant un niveau spécifique et met à jour les attributs du jeu*/
    void chargerFichier(Jeu* jeu, const char* _niveau);

#endif