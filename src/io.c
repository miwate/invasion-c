#include "../prot/jeu.h"

/* -- Ce fichier contient les fonctions pour les entrées/sorties (càd fichiers locaux, terminal MAIS pas la partie graphique) -- */
/* La partie graphique ayant été ajoutée par la suite, certaines fonctions d'affichage serviront pour debug */

/* Affiche une prévualisation de la vague d'étudiants */
void prevualisationVagues(Jeu* jeu){

    /* Vague vide */
    if (jeu->etudiants == NULL){
        printf("Pas de vague.\n");
        return;
    }

    /* Hauteur et Largeur de la double liste */
    int H = jeu->lastLigne;
    int L = jeu->lastTour;

    /* On se sert d'une double liste pour afficher la vague ensuite*/
    char** preview = (char**)malloc(H * sizeof(char*));

    /* Remplissage de la double liste avec des points '.' */
    for (int i = 0; i < H; i++){
        preview[i] = (char*)malloc(L * sizeof(char));

        for (int j = 0; j < L; j++){
            preview[i][j] = '.';
        }
    }

    /* On récupère le premier étudiant */
    Etudiant* etu = jeu->etudiants;

    /* On récupère tous les étudiants grâce à la liste chaînée */
    while (etu != NULL){
        if (etu->ligne <= H && etu->tour <= L) {

            /* Modifie les points '.' de la double liste par le type de l'étudiant */
            preview[etu->ligne - 1][etu->tour - 1] = etu->type;
        }
        etu = etu->next;
    }

    /* Efface le terminal puis retour à la ligne et affichage de la cagnotte */
    printf("\033[2J\033[0;0HVoici ce qui vous attend ! Cagnotte : %d€\n", jeu->cagnotte);

    /* Affiche la vague d'étudiants selon la liste*/
    for (int i = 0; i < H; i++){
        printf("%d|\t", i + 1);

        for (int j = 0; j < L; j++) {
            printf("%c\t", preview[i][j]);
        }
        printf("\n");
    }

    /* Libération de la mémoire */
    for (int i = 0; i < H; i++) {
        free(preview[i]);
    }
    free(preview);
}



/* Affiche l'état actuel du jeu -*/
void renduActuelJeu(Jeu* jeu){

    if (jeu == NULL){
        printf("Jeu vide.\n");
        return;
    }

    /* Affichage du numéro du tour */
    printf("Tour %d\n", jeu->tour);
    
    /* Hauteur et Largeur de la double liste */
    int H = jeu->lastLigne;
    int L = SPAWN_AREA;

    /* On se sert d'une double liste pour afficher la vague ensuite*/
    char** render = (char**)malloc(H * sizeof(char*));

    /* Remplissage de la double liste avec des points '.' */
    for (int i = 0; i < H; i++){
        render[i] = (char*)malloc(L * sizeof(char));

        for (int j = 0; j < L; j++){
            render[i][j] = '.';
        }
    }

    /* Récupération des tourelles avec la file */
    Tourelle* barney = jeu->tourelles;

    /* On récupère tous les Barney grâce à la liste chaînée */
    while (barney != NULL){
        if (barney->ligne <= H && barney->position <= L) {

            /* Modifie les points '.' de la double liste par le type de Barney */
            render[barney->ligne - 1][barney->position - 1] = barney->type;
        }
        barney = barney->next;
    }

    /* On récupère tous les étudiants grâce à la liste chaînée */
    Etudiant* etu = jeu->etudiants;
    while (etu != NULL){
        if (etu->position <= SPAWN_AREA && etu->tour <= jeu->tour) {

            /* Modifie les points '.' de la double liste par le type de l'étudiant */
            render[etu->ligne - 1][etu->position - 1] = etu->type;
        }
        etu = etu->next;
    }

    /* Efface le terminal puis retour à la ligne + Affiche le numéro du tour et la CAgnotte*/
    printf("\033[2J\033[0;0HTour %d - Cagnotte : $%d\n", jeu->tour, jeu->cagnotte);

    /* Affiche le rendu selon la liste*/
    for (int i = 0; i < H; i++){
        printf("%d|\t", i + 1);

        for (int j = 0; j < L; j++) {
            printf("%c\t", render[i][j]);
        }
        printf("\n");
    }

    /* Libération de la mémoire */
    for (int i = 0; i < H; i++) {
        free(render[i]);
    }
    free(render);

}


/* Charge un fichier contenant un niveau spécifique et met à jour les attributs du jeu donné en paramètres */
void chargerFichier(Jeu* jeu, const char* _niveau) {

    FILE* niveau = fopen(_niveau, "r");

    if (niveau == NULL) {
        printf("Niveau %s introuvable.\n", _niveau);
        return;
    }

    /* Lecture de la cagnotte */
    printf("Lecture de %s\n", _niveau);
    fscanf(niveau, "%d", &jeu->cagnotte);

    int tour, ligne, hauteur = 0;
    char type;

    /* Lecture de la vague*/
    while (fscanf(niveau, "%d %d %c", &tour, &ligne, &type) == 3){
        if (ligne > hauteur) hauteur = ligne;
        ajoutEtudiant(jeu, tour, ligne, type);
    }
    
    /* On en aura besoin pour la preview */
    jeu->lastLigne = hauteur;
    jeu->lastTour = tour;

    fclose(niveau);

}
