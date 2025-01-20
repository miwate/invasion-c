#include "../prot/jeu.h"

/* -- Ce fichier contient les fonctions pour les entrées/sorties (càd fichiers locaux, terminal et affichage) -- */


/* Fonction prenant en paramètre le jeu*/
/* Affiche une prévualisation de la vague d'étudiants */
void previewVagues(Jeu* jeu) {

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

    /* Efface le terminal puis retour à la ligne */
    printf("\033[2J\033[0;0HPreview de la vague\n");

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



/* Charge un fichier contenant un niveau spécifique et met à jour les attributs du jeu donné en paramètres */
void loadFichier(Jeu* jeu, const char* _niveau) {

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
        addEtudiant(jeu, tour, ligne, type);
    }
    
    /* On en aura besoin pour la preview */
    jeu->lastLigne = hauteur;
    jeu->lastTour = tour;

    fclose(niveau);

}
