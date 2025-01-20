#ifndef JEU_H
    #define JEU_H

    #include <stdio.h>
    #include <stdlib.h>

/* -- Définitions des structures -- */
    typedef struct tourelle{
        int pointsDeVie;
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
    } Jeu;


/* -- Protoytypes de fonctions -- */

    /* Initialise un jeu vide*/
    void initJeu(Jeu* jeu);

    /* Ajoute un étudiant dans la file en fonction de son type*/
    void addEtudiant(Jeu* jeu, const int _tour, const int _ligne, const char _type);

    /* Prévualise la vague d'étudiants dans le terminal*/
    void previewVagues(Jeu* jeu);

    /* Charge un fichier contenant un niveau spécifique et met à jour les attributs du jeu*/
    void loadFichier(Jeu* jeu, const char* _niveau);

#endif