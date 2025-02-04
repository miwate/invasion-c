#ifndef REPLAY_H
    #define REPLAY_H

/* -- Définition des structures -- */

    /* Au lieu d'avoir un enregistrement de la partie, on utilise un replay */
    /* Le jeu étant déterministe, sans randomisation (sauf pour dire bravo à la fin du jeu)
    On peut se permettre de faire un replay */
    typedef struct replay{
        int tour;
        int ligne;
        int position;
        int type;
        struct replay* next;
    } Replay;

    /* Définition externe*/
    extern Replay* replay;

    /* Sauvegarde l'entièreté de la partie */
    void sauvegarderReplay(const char _niveau[], Replay* replay);

    /* Voir un replay */
    void voirReplay(const int _fichier);

#endif