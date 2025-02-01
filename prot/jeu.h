#ifndef JEU_H
    #define JEU_H
    #define SPAWN_DISTANCE 15

    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <time.h>

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
        int degats;
        struct etudiant* next;
        struct etudiant* next_line;
        struct etudiant* prev_line;
        char type;
    } Etudiant;

    typedef struct jeu{
        Tourelle* tourelles;
        Etudiant* etudiants;
        int fin;
        int cagnotte;
        int tour;
        int dernierTour;
        int derniereLigne;
        int score;
        int scoreMax;
        int combo;
        float multiplicateurScore;
    } Jeu;


/* -- Protoytypes de fonctions -- */

    /* Initialise un jeu vide*/
    void initJeu(Jeu* jeu);
    
    /* Rafraîchit le jeu pour un tour suivant */
    void rafraichirJeu(Jeu* jeu);

    /* Enregistrer score dans le fichier du niveau du dossier ../lb*/
    void enregistrerScore(const char* _fichierDest, int _score, int _codeFin);

    /* Choisit un mot au hasard pour féliciter le joueur à la fin d'une partie */
    char* bravoHasard(int _codeFin);

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

    /* Demande à l'utilisateur s'il veut placer une tourelle*/
    void questionTourelle(Jeu* jeu);

    // Le format des données enregistrées par la sauvegarde influe sur l'ordres des informations à charger avec chargerFichierSave()
    /* Sauvegarder la partie en cours */
    void sauvegarderPartie(Jeu* jeu, const char* destFichier);

    /* Charger une sauvegarde si elle existe : NE MARCHE PAS*/
    int chargerFichierSave(Jeu* jeu, const char* srcFichier);

    /* VIDER LE BUFFER */
    void viderBuffer();

#endif
