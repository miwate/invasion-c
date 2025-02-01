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
    int H = jeu->derniereLigne;
    int L = jeu->dernierTour;

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

    
    /* Hauteur et Largeur de la double liste */
    int H = jeu->derniereLigne;
    int L = SPAWN_DISTANCE;

    /* On se sert d'une double liste pour afficher la vague ensuite*/
    char*** render = (char***)malloc(H * sizeof(char**));

    /* Remplissage de la double liste avec des points '.' */
    for (int i = 0; i < H; i++){
        render[i] = (char**)malloc(L * sizeof(char*));

        for (int j = 0; j < L; j++){
            render[i][j] = strdup(".");
        }
    }

    /* Récupération des tourelles avec la file */
    Tourelle* barney = jeu->tourelles;

    /* On récupère tous les Barney grâce à la liste chaînée */
    while (barney != NULL){
        if (barney->ligne <= H && barney->position <= L) {

            /* Modifie les points '.' de la double liste par le type de Barney et ses pv*/
            char type[3];
            /*
            type[0] = barney->pointsDeVie;
            type[1] = barney->type;
            type[2] = '\0';
            */
            snprintf(type, sizeof(type), "%d%c", barney->pointsDeVie, barney->type);
            /* Libère l'espace pour réécrire dessus*/
            // free(render[barney->ligne - 1][barney->position - 1]);

            /* MOdifie la liste*/
            render[barney->ligne - 1][barney->position - 1] = strdup(type);
        }
        barney = barney->next;
    }

    /* On récupère tous les étudiants grâce à la liste chaînée */
    Etudiant* etu = jeu->etudiants;
    while (etu != NULL){
        if (etu->position <= SPAWN_DISTANCE && etu->tour <= jeu->tour) {

            /* Modifie les points '.' de la double liste par le type de l'étudiant */
            char type[3];
            snprintf(type, sizeof(type), "%d%c", etu->pointsDeVie, etu->type);
            /* Libère l'espace pour réécrire dessus*/
            // free(render[etu->ligne - 1][etu->position - 1]);

            render[etu->ligne - 1][etu->position - 1] = strdup(type);
        }
        etu = etu->next;
    }

    /* Efface le terminal puis retour à la ligne + Affiche le numéro du tour et la CAgnotte*/
    printf("\033[2J\033[0;0H");
    printf("Tour %d - Cagnotte : %d€ - Score : %d\n", jeu->tour, jeu->cagnotte, jeu->score);

    /* Affiche le rendu selon la liste*/
    for (int i = 0; i < H; i++){
        printf("%d|\t", i + 1);

        for (int j = 0; j < L; j++) {
            printf("%s\t", render[i][j]);
        }
        printf("\n");
    }

    /* Libération de la mémoire */
    for (int i = 0; i < H; i++) {
        for (int j = 0; j<L; j++) {
            free(render[i][j]);
        }
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
    jeu->derniereLigne = hauteur;
    jeu->dernierTour = tour;

    fclose(niveau);

}

/* Choisit un mot au hasard pour féliciter/dire dommage le joueur à la fin d'une partie */
char* bravoHasard(int _codeFin){
    /* Liste des mots */
    char* bravo[] = {"Bravo", "Félicitations", "Hourra", "Excellent", "Parfait", "Dommage", "Malheureusement", "Tristesse", "Déception"};
    
    int i = 0, j = 5;
    if (_codeFin == -1){
        i = 5;
        j = 9;
    }

    /* Mot au hasard */
    srand(time(NULL));
    int n = i-j;
    return bravo[i + rand() % n];
}


/* Enregistre un score demande le nom du joueur */
void enregistrerScore(const char* _fichierDest, int _score, int _codeFin){
    char nom[128] = "Anonyme";
    char* bravo;

    /* Mot au hasard */
    bravo = bravoHasard(_codeFin);

    if (_codeFin == -1){
        printf("%s ! Les carottes sont cuites -  Score : %d\n", bravo, _score);       
    }
    else {
        printf("%s ! Vous avez sauvé Dauphine -  Score : %d\n", bravo, _score);
    }
    printf("Votre nom :\n");
    scanf("%127s", nom);

    /* Enregistre le score dans le fichier lb/scores.txt*/
    FILE* fichier = fopen(_fichierDest, "a");
    if (fichier ==NULL){
        printf("Impossible de lire le fichier lb\n");
        return;
    }
    fprintf(fichier, "%s\t%d\n", nom, _score);
    fclose(fichier);

    printf("Score enregistré !\n");
}

void sauvegarderPartie(Jeu* jeu, const char* destFichier){
    FILE* sauvegarde = fopen(destFichier, "w");

    if (sauvegarde == NULL){
        printf("Sauvegarde échouée !\n");
        return;
    }

    // Collecte des données de la structure Jeu
    fprintf(sauvegarde, "%d %d %d\n", jeu->tour, jeu->cagnotte, jeu->dernierTour);

    // Collecte des données des Tourelles
    Tourelle* barney = jeu->tourelles;

    // Récupération de toutes les tourelles
    while (barney != NULL){
        fprintf(sauvegarde, "T %d %d %d %d %c\n", barney->ligne, barney->position, barney->pointsDeVie, barney->degats, barney->type);
        barney = barney->next;
    }
    
    // Collecte des données des étudiants
    Etudiant* etu = jeu->etudiants;
    // Récupération des données des étudiants 
    while (etu != NULL){
        fprintf(sauvegarde, "%d %d %d %d %d %c\n", etu->ligne, etu->position, etu->pointsDeVie, etu->vitesse, etu->tour, etu->type);
        etu = etu->next;
    }

    fclose(sauvegarde);
    printf("Partie sauvegardée dans le fichier %s !\n", destFichier);
    return;
}

// Ne marche pas
int chargerFichierSave(Jeu* jeu, const char* srcFichier){
    FILE* sauvegarde = fopen(srcFichier, "r");

    if (sauvegarde == NULL){
        printf("Sauvegarde %s introuvable.\n", srcFichier);
        return 0;
    }

    // Provoque une erreur si le jeu n'est pas initialisé
    initJeu(jeu);

    /* Lecture des données de la structure Jeu */
    fscanf(sauvegarde, "%d %d %d", &jeu->tour, &jeu->cagnotte, &jeu->dernierTour);

    int tour, ligne, position, pointsDeVie, degats, vitesse;
    char type;

    /* Lecture des données des tourelles */
    /* Sort de la boucle en cas d'échec <=> Ligne vide <=> Fin du fichier */
    while (fscanf(sauvegarde, "T %d %d %d %d %c", &ligne, &position, &pointsDeVie, &degats, &type) == 5){
        ajoutTourelle(jeu, ligne, position, type);
    }

    /* Lecture des données des étudiants */
    /* Idem */
    while (fscanf(sauvegarde, "%d %d %d %d %d %c", &ligne, &position, &pointsDeVie, &vitesse, &tour, &type) == 6){
        ajoutEtudiant(jeu, tour, ligne, type);
    }

    fclose(sauvegarde);
    printf("Sauvegarde %s chargée.\n", srcFichier);
    return 1;
}

void viderBuffer(){
    int videBuffer;
    while ((videBuffer = getchar()) != '\n' && videBuffer != EOF);

}

void questionTourelle(Jeu* jeu, int* sauver){
    int condition = 1;
    int ligne, position;
    char type;
    // On n'en sort que si le joueur ne veut pas construire de tourelle (Sa réponse est 'N').
    while (condition){ 
        printf("Voulez-vous construire une tourelle ?\nN pour non, n'importe quoi d'autre si oui\n");
        char reponseTourelle;
        scanf("%c", &reponseTourelle);
        viderBuffer();

        if (reponseTourelle == 'N'){
            printf("Aucune tourelle n'est construite ce tour.\n");
            condition = 0;
            return;
        }

        else if (reponseTourelle == 'S'){
            sauvegarderPartie(jeu, "sauvegarde.txt");
            *sauver = 1;
            return;
        }

        else {  
            // Récupération de la ligne avec garde fous 
            int ligneCorrecte = 0;
            while (ligneCorrecte == 0) {
                printf("En quelle ligne voulez-vous placer la tourelle ?\n");
                if (scanf("%d", &ligne) != 1) {
                    printf("Entrée invalide. Veuillez saisir un nombre entier.\n");
                    viderBuffer();
                    continue;
                }
                if (ligne < 1 || ligne > 7)
                    printf("Ligne invalide.-> Recommencez et pensez à entrer une ligne entre 1 et %d : \n", jeu->derniereLigne);
                else 
                    ligneCorrecte = 1;
            }

            // Récupération de la position avec garde fous 
            int posCorrecte = 0;
            while (posCorrecte == 0) {
                printf("A quelle position voulez-vous placer la tourelle ?\n");
                if (scanf("%d", &position) != 1) {
                    printf("Entrée invalide. Veuillez saisir un nombre entier.\n");
                    viderBuffer();
                    continue;
                }
                if (position < 1 || position > 10)
                    printf("Position invalide.-> Recommencez et pensez à entrer une position entre 1 et 10 : \n");
                else 
                    posCorrecte = 1;
                
            }

            // Récupération du type avec garde fous 
            printf("Choisissez un type de tourelle : \n");
            scanf(" %c", &type);
            viderBuffer();
            while (type != 't' && type != 's' && type != 'm' && type != 'b' && type != 'x'){
                printf("Type inconnu.\nRecommencez et pensez à entrer un des types suivants :\nt (Tourelle classique : 3PV, 1ATQ, Prix : 150) \ns (tourelle de ralentissement : 2PV, 1ATQ, Prix : 120) \nm (Mine infligeant des dégâts au contact d'un ennemi : 1PV, 5ATQ, Prix : 100) \nb (Bouclier : 6PV, 0ATQ, Prix : 200) \nx (Tourelle Explosive Adjacente : 2PV, 4ATQ, Prix : 800)\n");
                scanf(" %c", &type);
                viderBuffer();
            }
            ajoutTourelle(jeu, ligne, position, type);
        }
    }
}
